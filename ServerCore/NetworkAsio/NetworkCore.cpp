#pragma warning ( disable : 4996 )

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
#include "../Utility/BoostObjectPoolWrapper.h"

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

	networkImpl_->serverApp_->OnClose( session );
	networkImpl_->sessionManager_->RestoreSession( session );

	session->Close();
}

void NetworkCore::ShutdownSession( Session* session )
{
	if( session == nullptr )
		return;

	session->Shutdown();
}

ServerApp* NetworkCore::GetServerApp()
{
	return networkImpl_->serverApp_.get();
}

bool NetworkCore::EncodePacket( const char* src, int srcSize, Packet* packet )
{
	return networkImpl_->parser_->encodeMessage( src, srcSize, packet );
}

bool NetworkCore::DecodePacket( const char* src, int srcSize, Packet* packet )
{
	return networkImpl_->parser_->decodeMessage( src, srcSize, packet );
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
		networkImpl_->serverCommand_.insert( std::pair< unsigned int, CommandFunction_t >( protocol, command ) );
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

// 윈도우 서비스
#ifdef WIN32

HANDLE					gServerEvent = INVALID_HANDLE_VALUE;
DWORD					gCurrentStatus = 0;
SERVICE_STATUS_HANDLE	gServiceStatus;
char					gServiceName[100] = {0};


void SetStatus( SERVICE_STATUS_HANDLE serviceHandle, DWORD dwState, DWORD dwAccept = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE, DWORD dwCheckPoint = 0, DWORD dwWaitHint = 0, DWORD dwWin32ExitCode = 0 )
{
	SERVICE_STATUS ServiceStatus;
	ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ServiceStatus.dwCurrentState = dwState;
	ServiceStatus.dwControlsAccepted = dwAccept;
	ServiceStatus.dwWin32ExitCode = dwWin32ExitCode;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = dwCheckPoint;
	ServiceStatus.dwWaitHint = dwWaitHint;

	// 현재 상태를 보관해 둔다.
	gCurrentStatus = dwState;

	SetServiceStatus( serviceHandle, &ServiceStatus );
}

void ServiceHandler( DWORD opCode )
{
	// 현재 상태와 같은 제어 코드일 경우는 처리할 필요 없다.	
	if ( opCode ==  gCurrentStatus )
		return;

	switch ( opCode ) 
	{
	case SERVICE_CONTROL_STOP:
		SetStatus( gServiceStatus, SERVICE_STOP_PENDING, 0, 1, 1000 );
		WSASetEvent( gServerEvent );
		break;
	case SERVICE_CONTROL_INTERROGATE:
	default:
		SetStatus( gServiceStatus, gCurrentStatus, SERVICE_ACCEPT_STOP );
		break;
	}

}

void ServiceMain(DWORD argc, LPTSTR *argv)
{
	// register service handler
	gServiceStatus = RegisterServiceCtrlHandler( gServiceName, (LPHANDLER_FUNCTION)ServiceHandler );

	// notice start pending
	SetStatus( gServiceStatus, SERVICE_START_PENDING );
	SetStatus( gServiceStatus, SERVICE_RUNNING );
}

#endif
void NetworkCore::MakeDaemon( bool debug, char* serviceName )
{
#ifdef WIN32

	memset( gServiceName, 0, 100 );

	if( serviceName )
		strncpy( gServiceName, serviceName, 99 );
	else
		strcpy( gServiceName, "Server" );

	SERVICE_TABLE_ENTRY lpServiceTableEntry[]=
	{
		{ gServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL, NULL }
	};

	// start service
	BOOL ret = StartServiceCtrlDispatcher( lpServiceTableEntry );

	int err = 0;
	if( !ret )
	{
		err = GetLastError();
	}

#else

	pid_t pid;

	if( ( pid = fork() ) < 0 ) 
	{
		log( DBG, " Daemon start fork failed");              
		exit( 0 );
	}
	else if( pid != 0 )
	{
		exit( 0 );
	}

	if( !debug )
	{
		//! stdin, stdout close       
		int fd0, fd1, fd2;
		fd0 = open("/dev/null", O_RDWR);
		dup2( fd0, 1 );
		dup2( fd0, 2 );
		close( fd0 );
	}


	setsid();
#endif
}
