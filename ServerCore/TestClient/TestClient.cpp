#include <iostream>
#include <string>
#include <array>

#include "stdafx.h"

#include "../Utility/Parser.h"

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

#include "TestProtocolEncode.h"
#include "TestProtocolDecode.h"

#define SERVER_PORT 1500

class ParserTest : public IParser
{
public:
	ParserTest() {}
	virtual ~ParserTest() {}

	virtual int ParseStream( const char* src, int srcSize, Command* command )
	{
		if( command == nullptr )
			return 0;

		if( sizeof(PACKET_HEADER) > srcSize )
			return 0;

		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>(src);
		if( static_cast<int>(header->size_) > srcSize )
			return 0;

		command->cmdID_ = header->protocol_;
		command->cmdBuffer_.initializeBuffer( src, header->size_ );

		return header->size_;
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
	NetworkCore::GetInstance();

	NetworkCore::GetInstance().InitializeEngine( new ServerApp );
	NetworkCore::GetInstance().InitializeParser( new ParserTest );

	NetworkCore::GetInstance().AddServerCommand( SC_ECHO_TEST_ACK, [] ( Command* cmd ) -> unsigned int
	{
		PCK_SC_ECHO_TEST_ACK pck;
		decode_SC_ECHO_TEST_ACK( cmd->cmdBuffer_, pck );

		//printf("%d, %s, %f\n", pck.data1_, pck.data2_, pck.data3_[0] );

		return 0;
	} );

	NetworkCore::GetInstance().AddServerCommand( SC_PING, [] ( Command* cmd ) -> unsigned int
	{
		BufferSerializer buffer;
		encode_SC_PING( buffer, 0 );
		cmd->cmdSession_->SendBuffer( buffer );

		return 0;
	} );

	Session* newSession = NetworkCore::GetInstance().CreateSession();

	if( newSession == nullptr )
		return 0;

	if( newSession->ConnectTo( "127.0.0.1", SERVER_PORT ) == false )
	{
		NetworkCore::GetInstance().StopServer();
		return 0;
	}

	NetworkCore::GetInstance().AddSession( newSession, 0 );

	// 패킷 테스트
	int data1 = 999;
	std::array<char, 2048> data2;
	data2.fill( 'a' );
	std::vector<float> data3;
	data3.push_back( 1.1f );
	data3.push_back( 2.1f );
	data3.push_back( 3.1f );

	// 에코 테스트
	while( true )
	{
		BufferSerializer buffer;
		TestIterator testItr(data3);
		encode_CS_ECHO_TEST_REQ( buffer, data1, data2.data(), 2048, &testItr );
		newSession->SendBuffer( buffer );
		
		// 1초에 1000번
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}

	
	// DB 연결 테스트
	/*while( true )
	{
		BufferSerializer buffer;
		encode_CS_DB_TEST_REQ( buffer );
		newSession->SendBuffer( buffer );

		Sleep(1);
	}*/

	NetworkCore::GetInstance().StopServer();

	return 0;
}

