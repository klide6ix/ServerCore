#include <stdio.h>

#include "Socket.h"
#include "Application.h"



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

	if( accepter_.InitAccepter() == false )
		return false;

	return true;
}

bool Application::AddAcceptPort( int port )
{
	return accepter_.AddAcceptPort( port );
}

void Application::StartServer()
{
	accepter_.StartThread();

	accepter_.JoinThread();
}
