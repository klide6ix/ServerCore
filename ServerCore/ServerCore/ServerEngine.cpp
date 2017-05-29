#include <stdio.h>

#include "Socket.h"
#include "NetworkModel.h"
#include "SessionManager.h"

#include "Accepter.h"
#include "WorkThread.h"
#include "NetworkThread.h"

#include "IOCPModel.h"
#include "SelectModel.h"

#include "ServerEngine.h"


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

bool ServerEngine::InitApplication( SERVER_MODEL serverModel )
{
	try
	{
		accepter_ = std::make_shared<Accepter>();
		workThread_ = std::make_shared<WorkThread>();
		networkThread_ = std::make_shared<NetworkThread>();

		sessionManager_ = std::make_shared<SessionManager>();
		//networkModel_ = std::make_shared<IOCPModel>();
		networkModel_ = std::make_shared<SelectModel>();
	}
	catch( std::bad_alloc& )
	{
		return false;
	}

	if( accepter_->InitAccepter() == false )
		return false;

	if( networkModel_->InitNetworkModel() == false )
		return false;

	workThread_->SetThreadCount(4);
	networkThread_->SetThreadCount(1);

	return true;
}

bool ServerEngine::AddAcceptPort( int port )
{
	return accepter_->AddAcceptPort( port );
}

void ServerEngine::StartServer()
{
	accepter_->StartThread();
	networkThread_->StartThread();
	workThread_->StartThread();

	accepter_->JoinThread();
	networkThread_->JoinThread();
	workThread_->JoinThread();
}

void ServerEngine::AddSession( Session* newSession )
{
	networkModel_->AddSession( newSession );
}

void ServerEngine::RemoveSession( Session* session )
{
	networkModel_->RemoveSession( session );
}

void ServerEngine::SelectSession()
{
	networkModel_->SelectSession();
}