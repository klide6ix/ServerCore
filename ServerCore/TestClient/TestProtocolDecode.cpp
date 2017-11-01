#include "TestProtocolDecode.h"

TestProtocolDecoder::TestProtocolDecoder()
{
	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_ECHO_TEST_REQ, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_CS_ECHO_TEST_REQ* packet = new (std::nothrow) PCK_CS_ECHO_TEST_REQ;
		if( packet == nullptr )
			return false;

		memcpy( &packet->data_, buffer, sizeof( char ) * 2048 );
		buffer += ( sizeof( char ) * 2048 );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_ECHO_TEST_ACK, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SC_ECHO_TEST_ACK* packet = new (std::nothrow) PCK_SC_ECHO_TEST_ACK;
		if( packet == nullptr )
			return false;

		memcpy( &packet->data_, buffer, sizeof( char ) * 2048 );
		buffer += ( sizeof( char ) * 2048 );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_DB_TEST_REQ, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_CS_DB_TEST_REQ* packet = new (std::nothrow) PCK_CS_DB_TEST_REQ;
		if( packet == nullptr )
			return false;

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_PING, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SC_PING* packet = new (std::nothrow) PCK_SC_PING;
		if( packet == nullptr )
			return false;

		memcpy( &packet->sequence_, buffer, sizeof( unsigned int ) );
		buffer += sizeof( unsigned int );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_PONG, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_CS_PONG* packet = new (std::nothrow) PCK_CS_PONG;
		if( packet == nullptr )
			return false;

		memcpy( &packet->sequence_, buffer, sizeof( unsigned int ) );
		buffer += sizeof( unsigned int );

		pck = packet;

		return true;
	} ));

}

TestProtocolDecoder::~TestProtocolDecoder()
{
	decodeFunction_.clear();
}

bool TestProtocolDecoder::DecodeClientProtocol( EnumClientProtocol protocol, PACKET_HEADER*& pck, char* buffer )
{
	if( buffer == nullptr )
		return false;

	if( decodeFunction_.find( protocol ) == decodeFunction_.end() )
		return false;

	return decodeFunction_[protocol]( pck, buffer );
}
