#include <map>
#include <stdio.h>

#include "Session.h"
#include "ServerApp.h"
#include "ServerEngine.h"

#include "Socket.h"
#include "NetworkModel.h"
#include "SessionManager.h"
#include "Parser.h"
#include "WorkQueue.h"

#include "Accepter.h"
#include "WorkThread.h"
#include "NetworkThread.h"

#include "IOCPModel.h"
#include "SelectModel.h"

class ServerImplement
{
public:
	std::map< PROTOCOL_TYPE, CommandFunction_t > serverCommand_;

	std::shared_ptr<SessionManager>			sessionManager_;
	std::shared_ptr<NetworkModel>			networkModel_;
	std::shared_ptr<IParser>				parser_;
	std::shared_ptr<ServerApp>				serverApp_;

	std::shared_ptr<Accepter>				accepter_;
	std::shared_ptr<WorkThread>				workThread_;
	std::shared_ptr<NetworkThread>			networkThread_;

	std::shared_ptr<WorkQueue>				workQueue_;
};

std::unique_ptr<ServerEngine> ServerEngine::instance_;
std::once_flag ServerEngine::onceFlag_;

ServerEngine& ServerEngine::GetInstance()
{
	std::call_once( onceFlag_, [] 
	{
		instance_.reset(new ServerEngine);
	});

	return *instance_.get();
}

ServerEngine::ServerEngine()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

ServerEngine::~ServerEngine()
{
	if( serverImpl_ != nullptr )
		delete serverImpl_;

	WSACleanup();
}

bool ServerEngine::InitializeEngine( SERVER_MODEL serverModel )
{
	try
	{
		serverImpl_ = new ServerImplement();
		serverImpl_->workThread_ = std::make_shared<WorkThread>();
		serverImpl_->networkThread_ = std::make_shared<NetworkThread>();
		serverImpl_->sessionManager_ = std::make_shared<SessionManager>();
		serverImpl_->workQueue_ = std::make_shared<WorkQueue>();
		
		if( serverModel == MODEL_IOCP )
			serverImpl_->networkModel_ = std::make_shared<IOCPModel>();
		else
			serverImpl_->networkModel_ = std::make_shared<SelectModel>();
	}
	catch( std::bad_alloc& )
	{
		return false;
	}

	if( serverImpl_->workQueue_->Init() == false )
		return false;

	if( serverImpl_->networkModel_->InitNetworkModel() == false )
		return false;

	serverImpl_->workThread_->SetThreadCount(4);
	serverImpl_->networkThread_->SetThreadCount(4);

	serverImpl_->networkThread_->StartThread();
	serverImpl_->workThread_->StartThread();

	return true;
}

bool ServerEngine::InitializeAccepter()
{
	try
	{
		serverImpl_->accepter_ = std::make_shared<Accepter>();
	}
	catch( std::bad_alloc& )
	{
		return false;
	}

	if( serverImpl_->accepter_->InitAccepter() == false )
		return false;

	return true;
}

bool ServerEngine::InitializeApplication( ServerApp* application )
{
	if( application == nullptr )
		return false;

	try
	{
		serverImpl_->serverApp_.reset( application );
	}
	catch( std::bad_alloc& )
	{
		return false;
	}

	return true;
}

bool ServerEngine::InitializeParser( IParser* parser )
{
	if( parser == nullptr )
		return false;

	try
	{
		serverImpl_->parser_.reset( parser );
	}
	catch( std::bad_alloc& )
	{
		return false;
	}

	return true;
}

ServerApp* ServerEngine::GetServerApp()
{
	return serverImpl_->serverApp_.get();
}

bool ServerEngine::AddAcceptPort( int port )
{
	return serverImpl_->accepter_->AddAcceptPort( port );
}

void ServerEngine::StartServer()
{
	serverImpl_->accepter_->JoinThread();
	serverImpl_->networkThread_->JoinThread();
	serverImpl_->workThread_->JoinThread();
}

void ServerEngine::StartAccepter()
{
	serverImpl_->accepter_->StartThread();
}

Session* ServerEngine::CreateSession( Socket& socket )
{
	return serverImpl_->sessionManager_->CreateSession( socket.GetSocket() );
}

void ServerEngine::AddSession( Session* newSession, int acceptPort )
{
	serverImpl_->networkModel_->AddSession( newSession );

	serverImpl_->serverApp_->OnAccept( acceptPort, newSession );
}

void ServerEngine::CloseSession( Session* session )
{
	serverImpl_->networkModel_->RemoveSession( session );
	serverImpl_->sessionManager_->RestoreSession( session );

	session->CleanUp();

	serverImpl_->serverApp_->OnClose( session );
}

void ServerEngine::SelectSession()
{
	serverImpl_->networkModel_->SelectSession();
}

bool ServerEngine::EncodePacket( Packet* packet, char* dest, int& destSize )
{
	return serverImpl_->parser_->encodeMessage( packet, dest, destSize );
}

bool ServerEngine::DecodePacket( const char* src, int srcSize, Packet* packet )
{
	return serverImpl_->parser_->decodeMessage( src, srcSize, packet );
}

Packet* ServerEngine::AllocPacket()
{
	return serverImpl_->workQueue_->AllocObject();
}

void ServerEngine::FreePacket( Packet* obj )
{
	serverImpl_->workQueue_->RestoreObject( obj );
}

void ServerEngine::PushPacket( Packet* obj )
{
	serverImpl_->workQueue_->Push( obj );
}

Packet* ServerEngine::PopPacket()
{
	return serverImpl_->workQueue_->Pop();
}

void ServerEngine::AddServerCommand( PROTOCOL_TYPE protocol, CommandFunction_t command )
{
	if( serverImpl_->serverCommand_.find( protocol ) == serverImpl_->serverCommand_.end() )
	{
		serverImpl_->serverCommand_.insert( std::pair< PROTOCOL_TYPE, CommandFunction_t >( protocol, command ) );
	}
	else
	{
		serverImpl_->serverCommand_[protocol] = command;
	}
}

CommandFunction_t ServerEngine::GetServerCommand( PROTOCOL_TYPE protocol )
{
	if( serverImpl_->serverCommand_.find( protocol ) == serverImpl_->serverCommand_.end() )
	{
		return nullptr;
	}

	return serverImpl_->serverCommand_[protocol];
}