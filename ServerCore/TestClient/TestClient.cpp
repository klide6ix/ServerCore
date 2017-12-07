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

#include "TestProtocolEncode.h"
#include "TestProtocolDecode.h"

#include "../NetworkUDP/NetworkUdp.h"
#include "../NetworkUDP/UdpSession.h"

#ifdef _WIN32
#pragma comment(lib, "NetworkUdp.lib")
#endif


#define SERVER_PORT 1500
#define UDP_SERVER_PORT 2500
#define UDP_CLIENT_PORT 2501

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

	/*
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
	*/

	
	// DB 연결 테스트
	/*while( true )
	{
		BufferSerializer buffer;
		encode_CS_DB_TEST_REQ( buffer );
		newSession->SendBuffer( buffer );

		Sleep(1);
	}*/

	// UDP 테스트
	NetworkUdp::GetInstance().InitializeEngine();
	NetworkUdp::GetInstance().AddServerCommand( 0, [] ( UdpCommand* cmd ) -> unsigned int
	{
		printf( "Echo Data (%s)\n", cmd->cmdBuffer_.GetBuffer() );
		return 0;
	} );

	UdpSession* udpSession = NetworkUdp::GetInstance().CreateUdpSession();
	if( udpSession == nullptr )
		return 0;

	udpSession->InitializeUdpSession( UDP_CLIENT_PORT );
	boost::asio::ip::udp::endpoint serverPoint;
	serverPoint = boost::asio::ip::udp::endpoint( boost::asio::ip::address_v4::from_string("127.0.0.1"), UDP_SERVER_PORT );
	int udpIdx = 0;
	while( true )
	{
		BufferSerializer buffer;
		char msg[128] = {};

		sprintf( msg, "UdpEchoTest[%d]", udpIdx++ );
		buffer.put_data( msg, static_cast<unsigned short>(strlen(msg)) );

		//printf( "%s\n", msg );
		
		udpSession->SendDatagram( serverPoint, buffer );

		// 1초에 1000번
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
	NetworkCore::GetInstance().StopServer();

	return 0;
}

