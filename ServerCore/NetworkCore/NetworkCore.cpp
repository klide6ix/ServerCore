#include <map>
#include <queue>
#include <vector>
#include <stdio.h>
#include <tuple>
#include <chrono>

#include <signal.h>

#include "ServerApp.h"

#include "EpollModel.h"
#include "IOCPModel.h"
#include "SelectModel.h"

#include "Command.h"
#include "NetworkCore.h"
#include "SessionManager.h"
#include "Accepter.h"
#include "Socket.h"
#include "WorkThread.h"
#include "NetworkThread.h"
#include "TimerThread.h"

#include "../Utility/ObjectPool.h"
#include "../Utility/Parser.h"

#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

class NetworkImplement
{
public:

	std::map< COMMAND_ID, CommandFunction_t > serverCommand_;
	std::map< TIMER_ID, TimerFunction_t >	  timerCommand_;

	std::shared_ptr<IParser>			parser_ = nullptr;
	std::shared_ptr<ServerApp>			serverApp_ = nullptr;
	std::shared_ptr<SessionManager>		sessionManager_ = nullptr;
	std::shared_ptr<Accepter>			accepter_ = nullptr;
	std::shared_ptr<NetworkModel>		networkModel_ = nullptr;

	std::shared_ptr<CommandQueue>		workQueue_ = nullptr;
	std::shared_ptr<CommandQueue>		dbQueue_ = nullptr;
	std::shared_ptr<EventTimer>			timerQueue_ = nullptr;

	ObjectPool<Command>					commandPool_;

	std::vector<std::shared_ptr<NetworkThread>>		networkThreads_;
	std::vector<std::shared_ptr<WorkThread>>		workThreads_;

	std::shared_ptr<TimerThread>					timerThread_;

	typedef std::tuple<Session*, std::chrono::system_clock::time_point> TypeRetrySession_t;
	std::priority_queue< TypeRetrySession_t, std::vector<TypeRetrySession_t>, std::greater<TypeRetrySession_t> > retrySessions_;
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
		networkImpl_->timerQueue_ = std::make_shared<EventTimer>();

		networkImpl_->sessionManager_ = std::make_shared<SessionManager>();

#ifdef _WIN32
		networkImpl_->networkModel_ = std::make_shared<IOCPModel>();
#else
		networkImpl_->networkModel_ = std::make_shared<EpollModel>();
#endif

		if( networkThreadCount_ == 0 )
			networkThreadCount_ = std::thread::hardware_concurrency();

		for( unsigned int i = 0; i < networkThreadCount_; ++i )
		{
			networkImpl_->networkThreads_.push_back( std::make_shared<NetworkThread>() );
		}

		if( workThreadCount_ == 0 )
			workThreadCount_ = std::thread::hardware_concurrency();
		
		for( unsigned int i = 0; i < workThreadCount_; ++i )
		{
			networkImpl_->workThreads_.push_back( std::make_shared<WorkThread>() );
		}

		networkImpl_->timerThread_ = std::make_shared<TimerThread>();
	}
	catch( std::bad_alloc& )
	{
		return false;
	}

	if( networkImpl_->networkModel_->InitNetworkModel() == false )
		return false;

	networkImpl_->commandPool_.SetMaxPoolSize( 32 * 10 );
	if( networkImpl_->commandPool_.Init() == false )
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
	try
	{
		networkImpl_->accepter_ = std::make_shared<Accepter>();

		if( networkImpl_->accepter_->InitAccepter() == false )
			return false;

		networkImpl_->accepter_->StartThread();
	}
	catch( std::bad_alloc& )
	{
		return false;
	}

	return true;
}

bool NetworkCore::AddAcceptPort( int port )
{
	if( networkImpl_->accepter_ == nullptr )
		return false;

	return networkImpl_->accepter_->AddAcceptPort( port );
}

Session* NetworkCore::CreateSession()
{
	return networkImpl_->sessionManager_->CreateSession();
}

void NetworkCore::AddSession( Session* newSession, int acceptPort )
{
	networkImpl_->networkModel_->AddSession( newSession );

	networkImpl_->serverApp_->OnAccept( acceptPort, newSession );
}

void NetworkCore::CloseSession( Session* session )
{
	if( session == nullptr )
		return;

	networkImpl_->networkModel_->RemoveSession( session );
	networkImpl_->sessionManager_->RestoreSession( session );

	session->Close();

	networkImpl_->serverApp_->OnClose( session );
}

void NetworkCore::SelectSession( std::vector<SessionEvent>& sessionList )
{
	networkImpl_->networkModel_->SelectSession( sessionList );
}

ServerApp* NetworkCore::GetServerApp()
{
	return networkImpl_->serverApp_.get();
}

int NetworkCore::ParsePacket( const char* src, int srcSize, Command* command )
{
	return networkImpl_->parser_->ParseStream( src, srcSize, command );
}

Command* NetworkCore::AllocateCommand()
{
	return networkImpl_->commandPool_.Alloc();
}

void NetworkCore::DeallocateCommand( Command* obj )
{
	networkImpl_->commandPool_.Free( obj );
}

void NetworkCore::PushCommand( Command* cmd )
{
	networkImpl_->workQueue_->PushCommand( cmd );
}

Command* NetworkCore::PopCommand()
{
	return networkImpl_->workQueue_->PopCommand();
}

void NetworkCore::AddServerCommand( COMMAND_ID protocol, CommandFunction_t command )
{
	if( networkImpl_->serverCommand_.find( protocol ) == networkImpl_->serverCommand_.end() )
	{
		networkImpl_->serverCommand_.insert( std::pair< TIMER_ID, CommandFunction_t >( protocol, command ) );
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

void NetworkCore::PushTimerMessage( TIMER_ID id, int workCount, int workTime, void* object )
{
	networkImpl_->timerQueue_->PushTimer( id, workCount, workTime, object );
}

bool NetworkCore::PopTimerMessage( TimerObject*& timerObj )
{
	return networkImpl_->timerQueue_->PopTimer( timerObj );
}

void NetworkCore::AddTimerCommand( TIMER_ID protocol, TimerFunction_t command )
{
	if( networkImpl_->timerCommand_.find( protocol ) == networkImpl_->timerCommand_.end() )
	{
		networkImpl_->timerCommand_.insert( std::pair< TIMER_ID, TimerFunction_t >( protocol, command ) );
	}
	else
	{
		networkImpl_->timerCommand_[protocol] = command;
	}
}

TimerFunction_t NetworkCore::GetTimerCommand( TIMER_ID protocol )
{
	if( networkImpl_->timerCommand_.find( protocol ) == networkImpl_->timerCommand_.end() )
	{
		return nullptr;
	}

	return networkImpl_->timerCommand_[protocol];
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
	networkImpl_->accepter_->StopThread();

	for( auto thread : networkImpl_->networkThreads_ )
	{
		if( thread != nullptr )
			thread->StopThread();
	}

	for( auto thread : networkImpl_->workThreads_ )
	{
		if( thread != nullptr )
			thread->StopThread();
	}

	networkImpl_->timerThread_->StopThread();
}
void NetworkCore::RecvRetryProcess( Session* session )
{
	networkImpl_->networkModel_->RecvRetry( session );
}
