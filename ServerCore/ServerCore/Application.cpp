#include <stdio.h>

#include "Socket.h"
#include "NetworkModel.h"
#include "SessionManager.h"

#include "Accepter.h"
#include "WorkThread.h"
#include "NetworkThread.h"

#include "IOCPModel.h"
#include "SelectModel.h"

#include "Application.h"


std::unique_ptr<Application> Application::instance_;
std::once_flag Application::onceFlag_;

Application& Application::GetInstance()
{
	std::call_once( onceFlag_, [] 
	{
		instance_.reset(new Application);
	});

	return *instance_.get();
}

Application::Application()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

Application::~Application()
{
	WSACleanup();
}

bool Application::InitApplication( SERVER_MODEL serverModel )
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

	workThread_->SetThreadCount(4);
	networkThread_->SetThreadCount(4);

	return true;
}

bool Application::AddAcceptPort( int port )
{
	return accepter_->AddAcceptPort( port );
}

void Application::StartServer()
{
	accepter_->StartThread();
	networkThread_->StartThread();
	workThread_->StartThread();

	accepter_->JoinThread();
	networkThread_->JoinThread();
	workThread_->JoinThread();
}

void Application::AddSession( Session* newSession )
{
	networkModel_->AddSession( newSession );
}

void Application::RemoveSession( Session* session )
{
	networkModel_->RemoveSession( session );
}

void Application::SelectSession()
{
	networkModel_->SelectSession();
}