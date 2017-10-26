#include "ClientProtocolDecode.h"

ClientProtocolDecoder::ClientProtocolDecoder()
{
	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_CONNECT_REQ, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != CS_CONNECT_REQ )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_CS_CONNECT_REQ* packet = static_cast<PCK_CS_CONNECT_REQ*>( pck );

		memcpy( &packet->nickName_, buffer, sizeof( char ) * PLAYER_NICKNAME_LENGTH );
		buffer += ( sizeof( char ) * PLAYER_NICKNAME_LENGTH );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_CONNECT_ACK, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != SC_CONNECT_ACK )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_SC_CONNECT_ACK* packet = static_cast<PCK_SC_CONNECT_ACK*>( pck );

		memcpy( &packet->memberInfo_, buffer, sizeof( MEMBER_INFO ) );
		buffer += sizeof( MEMBER_INFO );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_LOBBY_PLAYER_LIST, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != SC_LOBBY_PLAYER_LIST )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_SC_LOBBY_PLAYER_LIST* packet = static_cast<PCK_SC_LOBBY_PLAYER_LIST*>( pck );

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

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_ROOM_PLAYER_JOIN_REQ, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != CS_ROOM_PLAYER_JOIN_REQ )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_CS_ROOM_PLAYER_JOIN_REQ* packet = static_cast<PCK_CS_ROOM_PLAYER_JOIN_REQ*>( pck );

		memcpy( &packet->roomId_, buffer, sizeof( unsigned int ) );
		buffer += sizeof( unsigned int );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_ROOM_PLAYER_JOIN_ACK, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != SC_ROOM_PLAYER_JOIN_ACK )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_SC_ROOM_PLAYER_JOIN_ACK* packet = static_cast<PCK_SC_ROOM_PLAYER_JOIN_ACK*>( pck );

		memcpy( &packet->success_, buffer, sizeof( bool ) );
		buffer += sizeof( bool );
		memcpy( &packet->errorCode_, buffer, sizeof( int ) );
		buffer += sizeof( int );
		memcpy( &packet->roomId_, buffer, sizeof( unsigned int ) );
		buffer += sizeof( unsigned int );
		memcpy( &packet->team_, buffer, sizeof( ENUM_TEAM_TYPE ) );
		buffer += sizeof( ENUM_TEAM_TYPE );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_ROOM_PLAYER_CHANGE_REQ, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != CS_ROOM_PLAYER_CHANGE_REQ )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_CS_ROOM_PLAYER_CHANGE_REQ* packet = static_cast<PCK_CS_ROOM_PLAYER_CHANGE_REQ*>( pck );

		memcpy( &packet->team_, buffer, sizeof( unsigned char ) );
		buffer += sizeof( unsigned char );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_ROOM_PLAYER_CHANGE_ACK, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != SC_ROOM_PLAYER_CHANGE_ACK )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_SC_ROOM_PLAYER_CHANGE_ACK* packet = static_cast<PCK_SC_ROOM_PLAYER_CHANGE_ACK*>( pck );

		memcpy( &packet->success_, buffer, sizeof( bool ) );
		buffer += sizeof( bool );
		memcpy( &packet->errorCode_, buffer, sizeof( int ) );
		buffer += sizeof( int );
		memcpy( &packet->team_, buffer, sizeof( ENUM_TEAM_TYPE ) );
		buffer += sizeof( ENUM_TEAM_TYPE );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_ROOM_PLAYER_LEAVE_REQ, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != CS_ROOM_PLAYER_LEAVE_REQ )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_CS_ROOM_PLAYER_LEAVE_REQ* packet = static_cast<PCK_CS_ROOM_PLAYER_LEAVE_REQ*>( pck );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_ROOM_PLAYER_LEAVE_ACK, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != SC_ROOM_PLAYER_LEAVE_ACK )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_SC_ROOM_PLAYER_LEAVE_ACK* packet = static_cast<PCK_SC_ROOM_PLAYER_LEAVE_ACK*>( pck );

		memcpy( &packet->success_, buffer, sizeof( bool ) );
		buffer += sizeof( bool );
		memcpy( &packet->errorCode_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_ROOM_PLAYER_LIST, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != SC_ROOM_PLAYER_LIST )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_SC_ROOM_PLAYER_LIST* packet = static_cast<PCK_SC_ROOM_PLAYER_LIST*>( pck );

		memcpy( &packet->blueList_, buffer, sizeof( MEMBER_INFO ) * MAX_TEAM_PLAYER );
		buffer += ( sizeof( MEMBER_INFO ) * MAX_TEAM_PLAYER );
		memcpy( &packet->redList_, buffer, sizeof( MEMBER_INFO ) * MAX_TEAM_PLAYER );
		buffer += ( sizeof( MEMBER_INFO ) * MAX_TEAM_PLAYER );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_ROOM_CHAT_REQ, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != CS_ROOM_CHAT_REQ )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_CS_ROOM_CHAT_REQ* packet = static_cast<PCK_CS_ROOM_CHAT_REQ*>( pck );

		memcpy( &packet->chatMsg_, buffer, sizeof( char ) * 1024 );
		buffer += ( sizeof( char ) * 1024 );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_ROOM_CHAT_MSG, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != SC_ROOM_CHAT_MSG )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_SC_ROOM_CHAT_MSG* packet = static_cast<PCK_SC_ROOM_CHAT_MSG*>( pck );

		memcpy( &packet->sender_, buffer, sizeof( MEMBER_INFO ) );
		buffer += sizeof( MEMBER_INFO );
		memcpy( &packet->chatMsg_, buffer, sizeof( char ) * 1024 );
		buffer += ( sizeof( char ) * 1024 );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_GAME_START_REQ, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != CS_GAME_START_REQ )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_CS_GAME_START_REQ* packet = static_cast<PCK_CS_GAME_START_REQ*>( pck );

		memcpy( &packet->levelName_, buffer, sizeof( char ) * LEVEL_NAME_LENGTH );
		buffer += ( sizeof( char ) * LEVEL_NAME_LENGTH );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_GAME_START_ACK, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != SC_GAME_START_ACK )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_SC_GAME_START_ACK* packet = static_cast<PCK_SC_GAME_START_ACK*>( pck );

		memcpy( &packet->success_, buffer, sizeof( bool ) );
		buffer += sizeof( bool );
		memcpy( &packet->errorCode_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_LOAD_GAME_NOT, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != SC_LOAD_GAME_NOT )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_SC_LOAD_GAME_NOT* packet = static_cast<PCK_SC_LOAD_GAME_NOT*>( pck );

		memcpy( &packet->dedicateIp_, buffer, sizeof( char ) * 20 );
		buffer += ( sizeof( char ) * 20 );
		memcpy( &packet->dedicatePort_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_GAME_READY_REQ, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != CS_GAME_READY_REQ )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_CS_GAME_READY_REQ* packet = static_cast<PCK_CS_GAME_READY_REQ*>( pck );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_LEAVE_GAME_NOT, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != SC_LEAVE_GAME_NOT )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_SC_LEAVE_GAME_NOT* packet = static_cast<PCK_SC_LEAVE_GAME_NOT*>( pck );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_LOBBY_PLAYER_LIST_REQ, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != CS_LOBBY_PLAYER_LIST_REQ )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_CS_LOBBY_PLAYER_LIST_REQ* packet = static_cast<PCK_CS_LOBBY_PLAYER_LIST_REQ*>( pck );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_ROOM_PLAYER_LIST_REQ, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != CS_ROOM_PLAYER_LIST_REQ )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_CS_ROOM_PLAYER_LIST_REQ* packet = static_cast<PCK_CS_ROOM_PLAYER_LIST_REQ*>( pck );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_ROOM_LIST_REQ, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != CS_ROOM_LIST_REQ )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_CS_ROOM_LIST_REQ* packet = static_cast<PCK_CS_ROOM_LIST_REQ*>( pck );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_ROOM_LIST_NOT, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != SC_ROOM_LIST_NOT )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_SC_ROOM_LIST_NOT* packet = static_cast<PCK_SC_ROOM_LIST_NOT*>( pck );

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

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_CREATE_ROOM_REQ, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != CS_CREATE_ROOM_REQ )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_CS_CREATE_ROOM_REQ* packet = static_cast<PCK_CS_CREATE_ROOM_REQ*>( pck );

		memcpy( &packet->roomName_, buffer, sizeof( char ) * ROOM_NAME_LENGTH );
		buffer += ( sizeof( char ) * ROOM_NAME_LENGTH );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_CREATE_ROOM_ACK, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != SC_CREATE_ROOM_ACK )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_SC_CREATE_ROOM_ACK* packet = static_cast<PCK_SC_CREATE_ROOM_ACK*>( pck );

		memcpy( &packet->success_, buffer, sizeof( bool ) );
		buffer += sizeof( bool );
		memcpy( &packet->errorCode_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( DS_CONNECT_REQ, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != DS_CONNECT_REQ )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_DS_CONNECT_REQ* packet = static_cast<PCK_DS_CONNECT_REQ*>( pck );

		memcpy( &packet->dedicateIp_, buffer, sizeof( char ) * 20 );
		buffer += ( sizeof( char ) * 20 );
		memcpy( &packet->dedicatePort_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SD_CONNECT_ACK, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != SD_CONNECT_ACK )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_SD_CONNECT_ACK* packet = static_cast<PCK_SD_CONNECT_ACK*>( pck );

		memcpy( &packet->success_, buffer, sizeof( bool ) );
		buffer += sizeof( bool );
		memcpy( &packet->errorCode_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SD_LOAD_GAME_NOT, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != SD_LOAD_GAME_NOT )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_SD_LOAD_GAME_NOT* packet = static_cast<PCK_SD_LOAD_GAME_NOT*>( pck );

		memcpy( &packet->levelName_, buffer, sizeof( char ) * LEVEL_NAME_LENGTH );
		buffer += ( sizeof( char ) * LEVEL_NAME_LENGTH );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( DS_GAME_READY_REQ, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != DS_GAME_READY_REQ )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_DS_GAME_READY_REQ* packet = static_cast<PCK_DS_GAME_READY_REQ*>( pck );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SD_START_GAME_NOT, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != SD_START_GAME_NOT )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_SD_START_GAME_NOT* packet = static_cast<PCK_SD_START_GAME_NOT*>( pck );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( DS_END_GAME_REQ, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != DS_END_GAME_REQ )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_DS_END_GAME_REQ* packet = static_cast<PCK_DS_END_GAME_REQ*>( pck );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SD_END_GAME_ACK, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != SD_END_GAME_ACK )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_SD_END_GAME_ACK* packet = static_cast<PCK_SD_END_GAME_ACK*>( pck );

		memcpy( &packet->success_, buffer, sizeof( bool ) );
		buffer += sizeof( bool );
		memcpy( &packet->errorCode_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( SC_PING, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != SC_PING )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_SC_PING* packet = static_cast<PCK_SC_PING*>( pck );

		memcpy( &packet->sequence_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		return true;
	} ));

	decodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( CS_PONG, [] ( PACKET_HEADER* pck, char* buffer ) -> bool
	{
		const PACKET_HEADER* header = reinterpret_cast<const PACKET_HEADER*>( buffer );

		if( pck->protocol_ != CS_PONG )
			return false;

		buffer += sizeof( PACKET_HEADER );

		PCK_CS_PONG* packet = static_cast<PCK_CS_PONG*>( pck );

		memcpy( &packet->sequence_, buffer, sizeof( int ) );
		buffer += sizeof( int );

		return true;
	} ));

}

ClientProtocolDecoder::~ClientProtocolDecoder()
{
	decodeFunction_.clear();
}

bool ClientProtocolDecoder::DecodeClientProtocol( EnumClientProtocol protocol, PACKET_HEADER* pck, char* buffer )
{
	if( pck == nullptr )
		return false;

	if( buffer == nullptr )
		return false;

	if( decodeFunction_.find( protocol ) == decodeFunction_.end() )
		return false;

	return decodeFunction_[protocol]( pck, buffer );
}
