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

#include "TestProtocolDecode2.h"

#define SERVER_PORT 1500

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

		if( sizeof(PACKET_HEADER) > srcSize )
			return false;

		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>(src);
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
	NetworkCore::GetInstance();

	NetworkCore::GetInstance().InitializeEngine( new ServerApp );
	NetworkCore::GetInstance().InitializeParser( new ParserTest );

	NetworkCore::GetInstance().AddServerCommand( SC_ECHO_TEST_ACK, [] ( Command& cmd ) -> unsigned int
	{
		Packet* packet = static_cast<Packet*>(cmd.cmdMessage_);
		//printf("Recv : %s\n", packet->GetPacketData() );
		return 0;
	} );

	NetworkCore::GetInstance().AddServerCommand( SC_PING, [] ( Command& cmd ) -> unsigned int
	{
		Packet packet;
		PACKET_HEADER header;
		header.protocol_ = CS_PONG;
		header.size_ = static_cast<unsigned short>(sizeof( PACKET_HEADER ));
		packet.AddPacketData( (const char*)&header, sizeof( PACKET_HEADER ) );

		cmd.cmdSession_->SendPacket( packet );

		return 0;
	} );

	Session* newSession = NetworkCore::GetInstance().CreateSession();

	if( newSession == nullptr )
		return 0;

	if( newSession->ConnectTo( "127.0.0.1", SERVER_PORT ) == false )
		return 0;

	NetworkCore::GetInstance().AddSession( newSession, 0 );

	// 패킷 테스트
	std::array<char, 2048> message;
	message.fill( 'a' );
	Packet packet;
	PACKET_HEADER header;
	header.protocol_ = CS_ECHO_TEST_REQ;
	header.size_ = static_cast<unsigned short>(sizeof( PACKET_HEADER ) + message.size());
	packet.AddPacketData( (const char*)&header, sizeof( PACKET_HEADER ) );
	packet.AddPacketData( message.data(), static_cast<unsigned short>(message.size()) );
	while( true )
	{
		newSession->SendPacket( packet );
		
		// 1초에 1000번
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// 에코 테스트
	/*
	while( true )
	{
		std::string msg;
		std::cout << "Message : ";
		std::cin >> msg;

		Packet packet;

		packet.AddPacketData( msg.c_str(), static_cast<unsigned short>(msg.size()) );
		newSession->SendPacket( packet );
	}
	*/

	// DB 연결 테스트
	/*while( true )
	{
		Packet packet;
		packet.SetProtocol( (PROTOCOL_TYPE)1 );
		newSession->SendPacket( packet );

		Sleep(1);
	}*/

	NetworkCore::GetInstance().StopServer();

	return 0;
}

