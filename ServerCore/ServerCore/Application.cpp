#include <stdio.h>

#include "Socket.h"
#include "NetworkModel.h"
#include "Accepter.h"
#include "SessionManager.h"

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
	printf("ServerModel : %d", serverModel );

	try
	{
		accepter_ = std::make_shared<Accepter>();
		sessionManager_ = std::make_shared<SessionManager>();
	}
	catch( std::bad_alloc& )
	{
		return false;
	}

	if( accepter_->InitAccepter() == false )
		return false;

	return true;
}

bool Application::AddAcceptPort( int port )
{
	return accepter_->AddAcceptPort( port );
}

void Application::StartServer()
{
	accepter_->StartThread();

	accepter_->JoinThread();
}
