#include <iostream>
#include <string>

#include "stdafx.h"

#include "Parser.h"
#include "Session.h"
#include "ServerEngine.h"
#include "ServerApp.h"

#define SERVER_PORT 1500

int main()
{
	ServerEngine::GetInstance();

	ServerEngine::GetInstance().InitializeEngine( MODEL_IOCP );
	ServerEngine::GetInstance().InitializeParser( new ParserDefault );
	ServerEngine::GetInstance().InitializeApplication( new ServerApp );

	Socket socket;
	socket.CreateSocket();

	if( socket.Connect( "127.0.0.1", SERVER_PORT ) == false )
		return 0;

	Session* newSession = ServerEngine::GetInstance().CreateSession( socket );

	if( newSession == nullptr )
		return 0;

	ServerEngine::GetInstance().AddSession( newSession, 0 );

	while( true )
	{
		std::string msg;
		std::cout << "Message : ";
		std::cin >> msg;
		
		newSession->SendPacket( msg.c_str(), static_cast<int>(msg.length()) );
	}
    
	return 0;
}

