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

	ServerEngine::GetInstance().AddServerCommand( 0, [] ( PROTOCOL_TYPE protocol, Packet* packet ) -> unsigned int
	{
		printf("Recv : %s\n", packet->GetPacketData() );
		return 0;
	} );

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

		Packet packet;
		
		packet.AddPacketData( msg.c_str(), static_cast<unsigned short>(msg.size()) );
		newSession->SendPacket( packet );
	}
    
	return 0;
}

