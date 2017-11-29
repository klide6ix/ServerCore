#include <list>
#include <chrono>

#include "stdafx.h"

#include "../Utility/Parser.h"
#include "../Utility/LogWriter.h"

#include "../DatabaseConnector/DatabaseCore.h"

#ifdef _WIN32
#pragma comment(lib, "DatabaseConnector.lib")
#endif

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

#include "../NetworkUDP/NetworkUdp.h"

#ifdef _WIN32
#pragma comment(lib, "NetworkUdp.lib")
#endif


#define SERVER_PORT 1500

#include "TestProtocolEncode.h"
#include "TestProtocolDecode.h"


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

	virtual bool IsCompletePacket( const char* src, int srcSize )
	{
		if( srcSize < sizeof(PACKET_HEADER) )
			return false;

		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>(src);
		if( srcSize < static_cast<int>(header->size_) )
			return false;

		return true;
	}

	virtual int ParseStream( const char* src, int srcSize, Command* command )
	{
		if( command == nullptr )
			return 0;

		if( IsCompletePacket( src, srcSize ) == false )
			return 0;

		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>(src);

		command->cmdID_ = header->protocol_;
		command->cmdBuffer_.InitializeBuffer( src, header->size_ );

		return header->size_;
	}

	virtual int ParseDatagram( const char* src, int srcSize, Command* command )
	{
		return srcSize;
	}
};

class TestIterator : public IEncodeIterator
{
	std::vector<float>& list;
	std::vector<float>::iterator itr;

public:

	TestIterator(std::vector<float>& data) : list( data ), itr( list.begin() ) {}

	virtual void begin()
	{
		itr = list.begin();
	}
	virtual void next()
	{
		++itr;
	}
	virtual bool hasNext()
	{
		return itr != list.end();
	}
	virtual bool fill( BufferSerializer& serializer )
	{
		return serializer.put_data( (*itr) );
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

	NetworkCore::GetInstance().AddServerCommand( CS_ECHO_TEST_REQ, [] ( Command* cmd ) -> unsigned int
	{
		static int _packetCount = 0;
		static std::chrono::system_clock::time_point before;		

		auto start = std::chrono::system_clock::now();
		ServerTest* serverApp = dynamic_cast<ServerTest*>(NetworkCore::GetInstance().GetServerApp());

		if( serverApp == nullptr )
			return __LINE__;

		PCK_CS_ECHO_TEST_REQ pck;
		decode_CS_ECHO_TEST_REQ( cmd->cmdBuffer_, pck );

		BufferSerializer broadcastBuffer;
		TestIterator testItr(pck.vectorTest_);
		encode_SC_ECHO_TEST_ACK( broadcastBuffer, pck.intTest_, pck.arrayTest_, 2048, &testItr );

		for( auto session : serverApp->GetClientList() )
		{
			session->SendBuffer( broadcastBuffer );
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

	NetworkCore::GetInstance().AddServerCommand( CS_DB_TEST_REQ, [] ( Command* cmd ) -> unsigned int
	{
		char* query = "select * from city where Name like '%SE%';";
		DatabaseCore::GetInstance()->PushQuery( query, strlen(query)  );
		return 0;
	} );

	NetworkCore::GetInstance().AddServerCommand( CS_PONG, [] ( Command* cmd ) -> unsigned int
	{
		printf("Pong Recv\n");
		return 0;
	} );

	// Timer
	NetworkCore::GetInstance().AddTimerCommand( 0, [] ( TimerObject& obj ) -> unsigned int
	{
		ServerTest* serverApp = dynamic_cast<ServerTest*>(NetworkCore::GetInstance().GetServerApp());

		if( serverApp == nullptr )
			return __LINE__;

		if( serverApp->GetClientList().empty() == true )
			return 0;

		//printf("Send Ping\n");

		BufferSerializer broadcastBuffer;
		unsigned int seq = 9999;
		encode_SC_PING( broadcastBuffer, seq );

		for( auto session : serverApp->GetClientList() )
		{
			session->SendBuffer( broadcastBuffer );
		}

		return 0;
	} );

	NetworkCore::GetInstance().PushTimerMessage( 0, TIMER_INFINITE, 10, nullptr );

	// Set Database
	/*
	const char* connectStr = "DRIVER={MySQL ODBC 3.51 Driver};SERVER=127.0.0.1;USER=admin;PASSWORD=admin;Trusted_Connection=yes;Database=world";
	DatabaseCore::GetInstance()->InitDatabaseCore( connectStr );
	DatabaseCore::GetInstance()->StartDatabase();

	DatabaseCore::GetInstance()->InitRedisClient();
	DatabaseCore::GetInstance()->StartRedisClient();
	*/

	// Set UDP
	NetworkUdp::GetInstance().InitializeEngine();
	NetworkUdp::GetInstance().StartUdp();


	// Join Server
	NetworkCore::GetInstance().StartServer();

	return 0;
}

