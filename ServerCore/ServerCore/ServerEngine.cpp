#include <stdio.h>

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
	WSACleanup();
}

bool ServerEngine::InitializeEngine( SERVER_MODEL serverModel )
{
	try
	{
		workThread_ = std::make_shared<WorkThread>();
		networkThread_ = std::make_shared<NetworkThread>();
		parser_ = std::make_shared<ParserDefault>();

		sessionManager_ = std::make_shared<SessionManager>();

		workQueue_ = std::make_shared<WorkQueue>();
		
		if( serverModel == MODEL_IOCP )
			networkModel_ = std::make_shared<IOCPModel>();
		else
			networkModel_ = std::make_shared<SelectModel>();
	}
	catch( std::bad_alloc& )
	{
		return false;
	}

	if( workQueue_->Init() == false )
		return false;

	if( networkModel_->InitNetworkModel() == false )
		return false;

	workThread_->SetThreadCount(4);
	networkThread_->SetThreadCount(4);

	networkThread_->StartThread();
	workThread_->StartThread();

	return true;
}

bool ServerEngine::InitializeAccepter()
{
	try
	{
		accepter_ = std::make_shared<Accepter>();
	}
	catch( std::bad_alloc& )
	{
		return false;
	}

	if( accepter_->InitAccepter() == false )
		return false;

	return true;
}

bool ServerEngine::AddAcceptPort( int port )
{
	return accepter_->AddAcceptPort( port );
}

void ServerEngine::StartServer()
{
	accepter_->JoinThread();
	networkThread_->JoinThread();
	workThread_->JoinThread();
}

void ServerEngine::StartAccepter()
{
	accepter_->StartThread();
}

void ServerEngine::AddSession( Session* newSession )
{
	networkModel_->AddSession( newSession );
}

void ServerEngine::CloseSession( Session* session )
{
	networkModel_->RemoveSession( session );
	sessionManager_->RestoreSession( session );

	session->CleanUp();
}

void ServerEngine::SelectSession()
{
	networkModel_->SelectSession();
}

bool ServerEngine::EncodePacket( const char* src, int srcSize, char* dest, int& destSize )
{
	return parser_->encodeMessage( src, srcSize, dest, destSize );
}

bool ServerEngine::DecodePacket( const char* src, int srcSize, char* dest, int& destSize )
{
	return parser_->decodeMessage( src, srcSize, dest, destSize );
}

MessageObject* ServerEngine::GetMessageObject()
{
	return workQueue_->AllocObject();
}

void ServerEngine::ReturnMessageObject( MessageObject* obj )
{
	workQueue_->RestoreObject( obj );
}

void ServerEngine::PushMessageObject( MessageObject* obj )
{
	workQueue_->Push( obj );
}

MessageObject* ServerEngine::PopMessageObject()
{
	return workQueue_->Pop();
}
