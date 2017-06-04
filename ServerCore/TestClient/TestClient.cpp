#include <iostream>
#include <string>

#include "stdafx.h"

#include "Socket.h"
#include "Session.h"
#include "SessionManager.h"

#include "ServerEngine.h"

#define SERVER_PORT 1500

int main()
{
	ServerEngine::GetInstance();

	ServerEngine::GetInstance().InitializeEngine( MODEL_IOCP );

	Socket socket;
	socket.CreateSocket();

	if( socket.Connect( "127.0.0.1", SERVER_PORT ) == false )
		return 0;

	Session* newSession = ServerEngine::GetInstance().getSessionManager()->CreateSession( socket.GetSocket() );

	if( newSession == nullptr )
		return 0;

	ServerEngine::GetInstance().AddSession( newSession );

	while( true )
	{
		std::string msg;
		std::cout << "Message : ";
		std::cin >> msg;
		
		newSession->SendPacket( msg.c_str(), static_cast<int>(msg.length()) );
	}
    
	return 0;
}

