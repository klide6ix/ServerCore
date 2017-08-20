#include <map>
#include <vector>
#include <stdio.h>
#include <signal.h>

#include "ServerApp.h"
#include "CommandQueue.h"
#include "NetworkCore.h"
#include "SessionManager.h"
#include "Accepter.h"
#include "WorkThread.h"
#include "NetworkThread.h"

#include "../Utility/ObjectPool.h"
#include "../Utility/Parser.h"
#include "../Utility/Packet.h"

class NetworkImplement
{
public:

	std::map< COMMAND_ID, CommandFunction_t > serverCommand_;
	
	boost::asio::io_service							ioService_;
	std::shared_ptr<boost::asio::io_service::work>	ioWork_ = nullptr;

	std::shared_ptr<IParser>			parser_ = nullptr;
	std::shared_ptr<ServerApp>			serverApp_ = nullptr;
	std::shared_ptr<SessionManager>		sessionManager_ = nullptr;

	std::shared_ptr<CommandQueue>		workQueue_ = nullptr;
	std::shared_ptr<CommandQueue>		dbQueue_ = nullptr;

	ObjectPool<Packet>					packetPool_;

	std::vector<std::shared_ptr<Accepter>>			accepters_;
	std::vector<std::shared_ptr<NetworkThread>>		networkThreads_;
	std::vector<std::shared_ptr<WorkThread>>		workThreads_;
};

NetworkCore::NetworkCore()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}


NetworkCore::~NetworkCore()
{
	WSACleanup();

	if( networkImpl_ != nullptr )
		delete networkImpl_;
}

std::unique_ptr<NetworkCore> NetworkCore::instance_;
std::once_flag NetworkCore::onceFlag_;

NetworkCore& NetworkCore::GetInstance()
{
	std::call_once( onceFlag_, [] 
	{
		instance_.reset(new NetworkCore);
	});

	return *instance_;
}

bool NetworkCore::InitializeEngine( ServerApp* application )
{
	if( application == nullptr )
		return false;

	try
	{
		networkImpl_ = new NetworkImplement();

		networkImpl_->serverApp_.reset( application );

		networkImpl_->workQueue_ = std::make_shared<CommandQueue>();
		networkImpl_->dbQueue_ = std::make_shared<CommandQueue>();
		networkImpl_->sessionManager_ = std::make_shared<SessionManager>();
		networkImpl_->ioWork_ = std::make_shared<boost::asio::io_service::work>(networkImpl_->ioService_);

		for( unsigned int i = 0; i < std::thread::hardware_concurrency(); ++i )
		{
			networkImpl_->workThreads_.push_back( std::make_shared<WorkThread>() );
			networkImpl_->networkThreads_.push_back( std::make_shared<NetworkThread>() );
		}
	}
	catch( std::bad_alloc& )
	{
		return false;
	}

	networkImpl_->packetPool_.SetMaxPoolSize( 32 * 10 );
	if( networkImpl_->packetPool_.Init() == false )
		return false;

	for( auto thread : networkImpl_->networkThreads_ )
	{
		thread->StartThread();
	}

	for( auto thread : networkImpl_->workThreads_ )
	{
		thread->StartThread();
	}

	signal( SIGABRT, [] (int param) 
	{
		printf("abort\n"); 
	} );

	signal( SIGINT, [] (int param) 
	{
		printf("abort\n"); 
	} );

	return true;
}

bool NetworkCore::InitializeParser( IParser* parser )
{
	if( parser == nullptr )
		return false;

	try
	{
		networkImpl_->parser_.reset( parser );
	}
	catch( std::bad_alloc& )
	{
		return false;
	}

	return true;
}

bool NetworkCore::InitializeAccepter()
{
	return true;
}

bool NetworkCore::AddAcceptPort( int port )
{
	auto accepter = std::make_shared<Accepter>( GetIoService(), port );
	networkImpl_->accepters_.push_back( accepter );

	accepter->StartAccept();

	return true;
}

Session* NetworkCore::CreateSession()
{
	return networkImpl_->sessionManager_->CreateSession();
}

void NetworkCore::AddSession( Session* newSession, int acceptPort )
{
	if( newSession->RecvPost() == false )
	{
		CloseSession( newSession );
		return;
	}

	networkImpl_->serverApp_->OnAccept( acceptPort, newSession );
}

void NetworkCore::CloseSession( Session* session )
{
	if( session == nullptr )
		return;

	networkImpl_->sessionManager_->RestoreSession( session );

	session->CleanUp();

	networkImpl_->serverApp_->OnClose( session );
}

ServerApp* NetworkCore::GetServerApp()
{
	return networkImpl_->serverApp_.get();
}

bool NetworkCore::EncodePacket( const char* src, int srcSize, char* dest, int& destSize )
{
	return networkImpl_->parser_->encodeMessage( src, srcSize, dest, destSize );
}

bool NetworkCore::DecodePacket( const char* src, int srcSize, char* dest, int& destSize )
{
	return networkImpl_->parser_->decodeMessage( src, srcSize, dest, destSize );
}

Packet* NetworkCore::AllocatePacket()
{
	return networkImpl_->packetPool_.Alloc();
}

void NetworkCore::FreePacket( Packet* obj )
{
	networkImpl_->packetPool_.Free( obj );
}

void NetworkCore::PushCommand( Command& cmd )
{
	networkImpl_->workQueue_->Push( cmd );
}

bool NetworkCore::PopCommand( Command& cmd )
{
	return networkImpl_->workQueue_->Pop( cmd );
}

void NetworkCore::AddServerCommand( COMMAND_ID protocol, CommandFunction_t command )
{
	if( networkImpl_->serverCommand_.find( protocol ) == networkImpl_->serverCommand_.end() )
	{
		networkImpl_->serverCommand_.insert( std::pair< PROTOCOL_TYPE, CommandFunction_t >( protocol, command ) );
	}
	else
	{
		networkImpl_->serverCommand_[protocol] = command;
	}
}

CommandFunction_t NetworkCore::GetServerCommand( COMMAND_ID protocol )
{
	if( networkImpl_->serverCommand_.find( protocol ) == networkImpl_->serverCommand_.end() )
	{
		return nullptr;
	}

	return networkImpl_->serverCommand_[protocol];
}

void NetworkCore::StartServer()
{
	for( auto thread : networkImpl_->networkThreads_ )
	{
		if( thread != nullptr )
			thread->JoinThread();
	}
}

void NetworkCore::StopServer()
{
	for( auto thread : networkImpl_->networkThreads_ )
	{
		if( thread != nullptr )
			thread->StopThread();
	}
}

boost::asio::io_service& NetworkCore::GetIoService()
{
	return networkImpl_->ioService_;
}
