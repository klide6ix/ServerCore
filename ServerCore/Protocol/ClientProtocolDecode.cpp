#include "ClientProtocolDecode.h"

ClientProtocolDecoder::ClientProtocolDecoder()
{
	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_CONNECT_REQ, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_CS_CONNECT_REQ* packet = new (std::nothrow) PCK_CS_CONNECT_REQ;
		if( packet == nullptr )
			return false;

		memcpy( &packet->nickName_, buffer, sizeof( char ) * PLAYER_NICKNAME_LENGTH );
		buffer += ( sizeof( char ) * PLAYER_NICKNAME_LENGTH );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_CONNECT_ACK, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SC_CONNECT_ACK* packet = new (std::nothrow) PCK_SC_CONNECT_ACK;
		if( packet == nullptr )
			return false;

		memcpy( &packet->memberInfo_, buffer, sizeof( MEMBER_INFO ) );
		buffer += sizeof( MEMBER_INFO );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_LOBBY_PLAYER_LIST, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SC_LOBBY_PLAYER_LIST* packet = new (std::nothrow) PCK_SC_LOBBY_PLAYER_LIST;
		if( packet == nullptr )
			return false;

		unsigned short count = 0;
		memcpy( &count, buffer, sizeof( unsigned short ) );
		buffer += sizeof( unsigned short );

		for( unsigned short i = 0; i < count; ++i )
		{
			MEMBER_INFO memberList;
			memcpy( &memberList, buffer, sizeof( MEMBER_INFO ) );
			buffer += sizeof( MEMBER_INFO );

			packet->memberList_.push_back( memberList );
		}

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_ROOM_PLAYER_JOIN_REQ, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_CS_ROOM_PLAYER_JOIN_REQ* packet = new (std::nothrow) PCK_CS_ROOM_PLAYER_JOIN_REQ;
		if( packet == nullptr )
			return false;

		memcpy( &packet->roomId_, buffer, sizeof( unsigned int ) );
		buffer += sizeof( unsigned int );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_ROOM_PLAYER_JOIN_ACK, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SC_ROOM_PLAYER_JOIN_ACK* packet = new (std::nothrow) PCK_SC_ROOM_PLAYER_JOIN_ACK;
		if( packet == nullptr )
			return false;

		memcpy( &packet->success_, buffer, sizeof( bool ) );
		buffer += sizeof( bool );
		memcpy( &packet->errorCode_, buffer, sizeof( int ) );
		buffer += sizeof( int );
		memcpy( &packet->roomId_, buffer, sizeof( unsigned int ) );
		buffer += sizeof( unsigned int );
		memcpy( &packet->team_, buffer, sizeof( ENUM_TEAM_TYPE ) );
		buffer += sizeof( ENUM_TEAM_TYPE );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_ROOM_PLAYER_CHANGE_REQ, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_CS_ROOM_PLAYER_CHANGE_REQ* packet = new (std::nothrow) PCK_CS_ROOM_PLAYER_CHANGE_REQ;
		if( packet == nullptr )
			return false;

		memcpy( &packet->team_, buffer, sizeof( unsigned char ) );
		buffer += sizeof( unsigned char );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_ROOM_PLAYER_CHANGE_ACK, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SC_ROOM_PLAYER_CHANGE_ACK* packet = new (std::nothrow) PCK_SC_ROOM_PLAYER_CHANGE_ACK;
		if( packet == nullptr )
			return false;

		memcpy( &packet->success_, buffer, sizeof( bool ) );
		buffer += sizeof( bool );
		memcpy( &packet->errorCode_, buffer, sizeof( int ) );
		buffer += sizeof( int );
		memcpy( &packet->team_, buffer, sizeof( ENUM_TEAM_TYPE ) );
		buffer += sizeof( ENUM_TEAM_TYPE );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_ROOM_PLAYER_LEAVE_REQ, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_CS_ROOM_PLAYER_LEAVE_REQ* packet = new (std::nothrow) PCK_CS_ROOM_PLAYER_LEAVE_REQ;
		if( packet == nullptr )
			return false;

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_ROOM_PLAYER_LEAVE_ACK, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SC_ROOM_PLAYER_LEAVE_ACK* packet = new (std::nothrow) PCK_SC_ROOM_PLAYER_LEAVE_ACK;
		if( packet == nullptr )
			return false;

		memcpy( &packet->success_, buffer, sizeof( bool ) );
		buffer += sizeof( bool );
		memcpy( &packet->errorCode_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_ROOM_PLAYER_LIST, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SC_ROOM_PLAYER_LIST* packet = new (std::nothrow) PCK_SC_ROOM_PLAYER_LIST;
		if( packet == nullptr )
			return false;

		memcpy( &packet->blueList_, buffer, sizeof( MEMBER_INFO ) * MAX_TEAM_PLAYER );
		buffer += ( sizeof( MEMBER_INFO ) * MAX_TEAM_PLAYER );
		memcpy( &packet->redList_, buffer, sizeof( MEMBER_INFO ) * MAX_TEAM_PLAYER );
		buffer += ( sizeof( MEMBER_INFO ) * MAX_TEAM_PLAYER );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_ROOM_CHAT_REQ, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_CS_ROOM_CHAT_REQ* packet = new (std::nothrow) PCK_CS_ROOM_CHAT_REQ;
		if( packet == nullptr )
			return false;

		memcpy( &packet->chatMsg_, buffer, sizeof( char ) * CHAT_MSG_LENGTH );
		buffer += ( sizeof( char ) * CHAT_MSG_LENGTH );
		memcpy( &packet->chatSize_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_ROOM_CHAT_MSG, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SC_ROOM_CHAT_MSG* packet = new (std::nothrow) PCK_SC_ROOM_CHAT_MSG;
		if( packet == nullptr )
			return false;

		memcpy( &packet->sender_, buffer, sizeof( MEMBER_INFO ) );
		buffer += sizeof( MEMBER_INFO );
		memcpy( &packet->chatMsg_, buffer, sizeof( char ) * CHAT_MSG_LENGTH );
		buffer += ( sizeof( char ) * CHAT_MSG_LENGTH );
		memcpy( &packet->chatSize_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_GAME_START_REQ, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_CS_GAME_START_REQ* packet = new (std::nothrow) PCK_CS_GAME_START_REQ;
		if( packet == nullptr )
			return false;

		memcpy( &packet->levelName_, buffer, sizeof( char ) * LEVEL_NAME_LENGTH );
		buffer += ( sizeof( char ) * LEVEL_NAME_LENGTH );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_GAME_START_ACK, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SC_GAME_START_ACK* packet = new (std::nothrow) PCK_SC_GAME_START_ACK;
		if( packet == nullptr )
			return false;

		memcpy( &packet->success_, buffer, sizeof( bool ) );
		buffer += sizeof( bool );
		memcpy( &packet->errorCode_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_LOAD_GAME_NOT, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SC_LOAD_GAME_NOT* packet = new (std::nothrow) PCK_SC_LOAD_GAME_NOT;
		if( packet == nullptr )
			return false;

		memcpy( &packet->dedicateIp_, buffer, sizeof( char ) * 20 );
		buffer += ( sizeof( char ) * 20 );
		memcpy( &packet->dedicatePort_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_GAME_READY_REQ, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_CS_GAME_READY_REQ* packet = new (std::nothrow) PCK_CS_GAME_READY_REQ;
		if( packet == nullptr )
			return false;

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_LEAVE_GAME_NOT, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SC_LEAVE_GAME_NOT* packet = new (std::nothrow) PCK_SC_LEAVE_GAME_NOT;
		if( packet == nullptr )
			return false;

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_LOBBY_PLAYER_LIST_REQ, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_CS_LOBBY_PLAYER_LIST_REQ* packet = new (std::nothrow) PCK_CS_LOBBY_PLAYER_LIST_REQ;
		if( packet == nullptr )
			return false;

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_ROOM_PLAYER_LIST_REQ, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_CS_ROOM_PLAYER_LIST_REQ* packet = new (std::nothrow) PCK_CS_ROOM_PLAYER_LIST_REQ;
		if( packet == nullptr )
			return false;

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_ROOM_LIST_REQ, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_CS_ROOM_LIST_REQ* packet = new (std::nothrow) PCK_CS_ROOM_LIST_REQ;
		if( packet == nullptr )
			return false;

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_ROOM_LIST_NOT, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SC_ROOM_LIST_NOT* packet = new (std::nothrow) PCK_SC_ROOM_LIST_NOT;
		if( packet == nullptr )
			return false;

		unsigned short count = 0;
		memcpy( &count, buffer, sizeof( unsigned short ) );
		buffer += sizeof( unsigned short );

		for( unsigned short i = 0; i < count; ++i )
		{
			ROOM_INFO roomList;
			memcpy( &roomList, buffer, sizeof( ROOM_INFO ) );
			buffer += sizeof( ROOM_INFO );

			packet->roomList_.push_back( roomList );
		}

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_CREATE_ROOM_REQ, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_CS_CREATE_ROOM_REQ* packet = new (std::nothrow) PCK_CS_CREATE_ROOM_REQ;
		if( packet == nullptr )
			return false;

		memcpy( &packet->roomName_, buffer, sizeof( char ) * ROOM_NAME_LENGTH );
		buffer += ( sizeof( char ) * ROOM_NAME_LENGTH );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_CREATE_ROOM_ACK, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SC_CREATE_ROOM_ACK* packet = new (std::nothrow) PCK_SC_CREATE_ROOM_ACK;
		if( packet == nullptr )
			return false;

		memcpy( &packet->success_, buffer, sizeof( bool ) );
		buffer += sizeof( bool );
		memcpy( &packet->errorCode_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( DS_CONNECT_REQ, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_DS_CONNECT_REQ* packet = new (std::nothrow) PCK_DS_CONNECT_REQ;
		if( packet == nullptr )
			return false;

		memcpy( &packet->dedicateIp_, buffer, sizeof( char ) * 20 );
		buffer += ( sizeof( char ) * 20 );
		memcpy( &packet->dedicatePort_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SD_CONNECT_ACK, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SD_CONNECT_ACK* packet = new (std::nothrow) PCK_SD_CONNECT_ACK;
		if( packet == nullptr )
			return false;

		memcpy( &packet->success_, buffer, sizeof( bool ) );
		buffer += sizeof( bool );
		memcpy( &packet->errorCode_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SD_LOAD_GAME_NOT, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SD_LOAD_GAME_NOT* packet = new (std::nothrow) PCK_SD_LOAD_GAME_NOT;
		if( packet == nullptr )
			return false;

		memcpy( &packet->levelName_, buffer, sizeof( char ) * LEVEL_NAME_LENGTH );
		buffer += ( sizeof( char ) * LEVEL_NAME_LENGTH );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( DS_GAME_READY_REQ, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_DS_GAME_READY_REQ* packet = new (std::nothrow) PCK_DS_GAME_READY_REQ;
		if( packet == nullptr )
			return false;

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SD_START_GAME_NOT, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SD_START_GAME_NOT* packet = new (std::nothrow) PCK_SD_START_GAME_NOT;
		if( packet == nullptr )
			return false;

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( DS_END_GAME_REQ, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_DS_END_GAME_REQ* packet = new (std::nothrow) PCK_DS_END_GAME_REQ;
		if( packet == nullptr )
			return false;

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SD_END_GAME_ACK, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SD_END_GAME_ACK* packet = new (std::nothrow) PCK_SD_END_GAME_ACK;
		if( packet == nullptr )
			return false;

		memcpy( &packet->success_, buffer, sizeof( bool ) );
		buffer += sizeof( bool );
		memcpy( &packet->errorCode_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_PING, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_SC_PING* packet = new (std::nothrow) PCK_SC_PING;
		if( packet == nullptr )
			return false;

		memcpy( &packet->sequence_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		pck = packet;

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_PONG, [] ( PACKET_HEADER*& pck, char* buffer ) -> bool
	{
		buffer += sizeof( PACKET_HEADER );

		PCK_CS_PONG* packet = new (std::nothrow) PCK_CS_PONG;
		if( packet == nullptr )
			return false;

		memcpy( &packet->sequence_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		pck = packet;

		return true;
	} ));

}

ClientProtocolDecoder::~ClientProtocolDecoder()
{
	decodeFunction_.clear();
}

bool ClientProtocolDecoder::DecodeClientProtocol( EnumClientProtocol protocol, PACKET_HEADER*& pck, char* buffer )
{
	if( buffer == nullptr )
		return false;

	if( decodeFunction_.find( protocol ) == decodeFunction_.end() )
		return false;

	return decodeFunction_[protocol]( pck, buffer );
}
