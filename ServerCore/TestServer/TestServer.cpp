#include <list>
#include "stdafx.h"

#include "ServerEngine.h"
#include "ServerApp.h"
#include "Parser.h"
#include "Session.h"

#define SERVER_PORT 1500

class ServerTest : public ServerApp
{
	std::list<Session*>	clientList_;

public:
	virtual void OnAccept( int port, Session* session )
	{
		printf( "Accept Session [%d]\n", port );
		clientList_.push_back( session );
	}
	virtual void OnClose( Session* session )
	{
		printf( "Close Session\n" );
		clientList_.remove( session );
	}

	std::list<Session*>& GetClientList() { return clientList_; }
};

int main()
{
	ServerEngine::GetInstance().InitializeEngine( MODEL_IOCP );
	ServerEngine::GetInstance().InitializeParser( new ParserDefault );
	ServerEngine::GetInstance().InitializeApplication( new ServerTest );

	ServerEngine::GetInstance().InitializeAccepter();
	ServerEngine::GetInstance().AddAcceptPort( SERVER_PORT );

	ServerEngine::GetInstance().AddServerCommand( 0, [] ( PROTOCOL_TYPE protocol, Packet* packet ) -> unsigned int
	{
		ServerTest* serverApp = dynamic_cast<ServerTest*>(ServerEngine::GetInstance().GetServerApp());

		Packet echoPacket;
		echoPacket.AddPacketData( packet->GetPacketData(), packet->GetPacketDataSize() );
		for( auto session : serverApp->GetClientList() )
		{
			session->SendPacket( echoPacket );
		}
		return 0;
	} );
	
	ServerEngine::GetInstance().StartAccepter();
	ServerEngine::GetInstance().StartServer();

    return 0;
}

