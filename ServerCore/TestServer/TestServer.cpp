#include <list>
#include <chrono>

#include "stdafx.h"

#include "../Utility/Parser.h"
#include "../Utility/LogWriter.h"

#include "../DatabaseConnector/DatabaseCore.h"

#define USE_BOOST_ASIO
#ifdef USE_BOOST_ASIO

#include "../NetworkAsio/ServerApp.h"
#include "../NetworkAsio/Session.h"
#include "../NetworkAsio/NetworkCore.h"

#ifdef _WIN32
#pragma comment(lib, "NetworkAsio.lib")
#endif

#else

#include "../NetworkCore/ServerApp.h"
#include "../NetworkCore/Session.h"
#include "../NetworkCore/NetworkCore.h"

#ifdef _WIN32
#pragma comment(lib, "NetworkCore.lib")
#endif

#endif

#ifdef _WIN32
#pragma comment(lib, "DatabaseConnector.lib")
#endif

#define SERVER_PORT 1500

struct _PACKET_HEADER
{
	unsigned short protocol_ = 0;
	unsigned short size_ = 0;

	void setProtocol( unsigned short protocol ) { protocol_ = protocol; }
	void setSize( unsigned short size ) { size_ = size; }
};

class ServerTest : public ServerApp
{
	std::list<Session*>	clientList_;

public:

	ServerTest() {}
	~ServerTest() {}

	virtual void OnAccept( int port, void* session ) override
	{
		printf( "Accept Session [%d]\n", port );
		clientList_.push_back( static_cast<Session*>(session) );
	}

	virtual void OnClose( void* session ) override
	{
		printf( "Close Session\n" );
		clientList_.remove( static_cast<Session*>(session) );
	}

	std::list<Session*>& GetClientList() { return clientList_; }
};

class ParserTest : public IParser
{
public:
	ParserTest() {}
	virtual ~ParserTest() {}

	virtual bool encodeMessage( const char* src, int srcSize, Packet* packet )
	{
		packet->SetPacketSize( srcSize );
		memcpy( packet->GetPacketBuffer(), src, srcSize );

		return true;
	}
	virtual bool decodeMessage( const char* src, int srcSize, Packet* packet )
	{
		if( packet == nullptr )
			return false;

		if( sizeof(_PACKET_HEADER) > srcSize )
			return false;

		const _PACKET_HEADER* header = reinterpret_cast<const _PACKET_HEADER*>(src);
		if( static_cast<int>(header->size_) > srcSize )
			return false;

		packet->SetPacketSize( header->size_ );
		packet->SetProtocol( header->protocol_ );
		
		memcpy( packet->GetPacketBuffer(), src, header->size_ );

		return true;
	}
};

int main()
{
	Logger::GetInstance().Initialize( "Log", "Old_Log", "TestServer" );
	Logger::GetInstance().Log( "Start Server" );
	Logger::GetInstance().Log( "Server Port [%d]", SERVER_PORT );

	NetworkCore::GetInstance().InitializeEngine( new ServerTest );
	NetworkCore::GetInstance().InitializeParser( new ParserTest );

	NetworkCore::GetInstance().InitializeAccepter();
	NetworkCore::GetInstance().AddAcceptPort( SERVER_PORT );

	NetworkCore::GetInstance().AddServerCommand( 0, [] ( Command& cmd ) -> unsigned int
	{
		static int _packetCount = 0;
		static std::chrono::system_clock::time_point before;		

		auto start = std::chrono::system_clock::now();
		Packet* packet = static_cast<Packet*>(cmd.cmdMessage_);

		ServerTest* serverApp = dynamic_cast<ServerTest*>(NetworkCore::GetInstance().GetServerApp());

		if( serverApp == nullptr )
			return __LINE__;

		for( auto session : serverApp->GetClientList() )
		{
			session->SendPacket( *packet );
		}
		
		++_packetCount;

		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		if( std::chrono::duration_cast<std::chrono::seconds>(now - before).count() > 1 )
		{
			printf("RecvPacket (%d)\n", _packetCount );
			before = now;
			_packetCount = 0;
		}

		return 0;
	} );

	NetworkCore::GetInstance().AddServerCommand( 1, [] ( Command& cmd ) -> unsigned int
	{
		char* query = "select * from city where Name like '%SE%';";
		DatabaseCore::GetInstance()->PushQuery( query, strlen(query)  );
		return 0;
	} );

	// Set Database
	/*
	const char* connectStr = "DRIVER={MySQL ODBC 3.51 Driver};SERVER=127.0.0.1;USER=admin;PASSWORD=admin;Trusted_Connection=yes;Database=world";
	DatabaseCore::GetInstance()->InitDatabaseCore( connectStr );
	DatabaseCore::GetInstance()->StartDatabase();

	DatabaseCore::GetInstance()->InitRedisClient();
	DatabaseCore::GetInstance()->StartRedisClient();
	*/

	// Join Server
	NetworkCore::GetInstance().StartServer();

	return 0;
}

