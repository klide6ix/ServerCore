inline bool decode_CS_CONNECT_REQ( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_CS_CONNECT_REQ> packet = std::make_shared<PCK_CS_CONNECT_REQ>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->nickName_, buffer, sizeof( char ) * PLAYER_NICKNAME_LENGTH );
	buffer += ( sizeof( char ) * PLAYER_NICKNAME_LENGTH );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SC_CONNECT_ACK( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SC_CONNECT_ACK> packet = std::make_shared<PCK_SC_CONNECT_ACK>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->memberInfo_, buffer, sizeof( MEMBER_INFO ) );
	buffer += sizeof( MEMBER_INFO );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SC_LOBBY_PLAYER_LIST( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SC_LOBBY_PLAYER_LIST> packet = std::make_shared<PCK_SC_LOBBY_PLAYER_LIST>();
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

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_CS_ROOM_PLAYER_JOIN_REQ( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_CS_ROOM_PLAYER_JOIN_REQ> packet = std::make_shared<PCK_CS_ROOM_PLAYER_JOIN_REQ>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->roomId_, buffer, sizeof( unsigned int ) );
	buffer += sizeof( unsigned int );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SC_ROOM_PLAYER_JOIN_ACK( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SC_ROOM_PLAYER_JOIN_ACK> packet = std::make_shared<PCK_SC_ROOM_PLAYER_JOIN_ACK>();
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

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_CS_ROOM_PLAYER_CHANGE_REQ( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_CS_ROOM_PLAYER_CHANGE_REQ> packet = std::make_shared<PCK_CS_ROOM_PLAYER_CHANGE_REQ>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->team_, buffer, sizeof( unsigned char ) );
	buffer += sizeof( unsigned char );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SC_ROOM_PLAYER_CHANGE_ACK( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SC_ROOM_PLAYER_CHANGE_ACK> packet = std::make_shared<PCK_SC_ROOM_PLAYER_CHANGE_ACK>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->success_, buffer, sizeof( bool ) );
	buffer += sizeof( bool );
	memcpy( &packet->errorCode_, buffer, sizeof( int ) );
	buffer += sizeof( int );
	memcpy( &packet->team_, buffer, sizeof( ENUM_TEAM_TYPE ) );
	buffer += sizeof( ENUM_TEAM_TYPE );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_CS_ROOM_PLAYER_LEAVE_REQ( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_CS_ROOM_PLAYER_LEAVE_REQ> packet = std::make_shared<PCK_CS_ROOM_PLAYER_LEAVE_REQ>();
	if( packet == nullptr )
		return false;

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SC_ROOM_PLAYER_LEAVE_ACK( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SC_ROOM_PLAYER_LEAVE_ACK> packet = std::make_shared<PCK_SC_ROOM_PLAYER_LEAVE_ACK>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->success_, buffer, sizeof( bool ) );
	buffer += sizeof( bool );
	memcpy( &packet->errorCode_, buffer, sizeof( int ) );
	buffer += sizeof( int );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SC_ROOM_PLAYER_LIST( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SC_ROOM_PLAYER_LIST> packet = std::make_shared<PCK_SC_ROOM_PLAYER_LIST>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->blueList_, buffer, sizeof( MEMBER_INFO ) * MAX_TEAM_PLAYER );
	buffer += ( sizeof( MEMBER_INFO ) * MAX_TEAM_PLAYER );
	memcpy( &packet->redList_, buffer, sizeof( MEMBER_INFO ) * MAX_TEAM_PLAYER );
	buffer += ( sizeof( MEMBER_INFO ) * MAX_TEAM_PLAYER );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_CS_ROOM_CHAT_REQ( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_CS_ROOM_CHAT_REQ> packet = std::make_shared<PCK_CS_ROOM_CHAT_REQ>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->chatMsg_, buffer, sizeof( char ) * CHAT_MSG_LENGTH );
	buffer += ( sizeof( char ) * CHAT_MSG_LENGTH );
	memcpy( &packet->chatSize_, buffer, sizeof( int ) );
	buffer += sizeof( int );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SC_ROOM_CHAT_MSG( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SC_ROOM_CHAT_MSG> packet = std::make_shared<PCK_SC_ROOM_CHAT_MSG>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->sender_, buffer, sizeof( MEMBER_INFO ) );
	buffer += sizeof( MEMBER_INFO );
	memcpy( &packet->chatMsg_, buffer, sizeof( char ) * CHAT_MSG_LENGTH );
	buffer += ( sizeof( char ) * CHAT_MSG_LENGTH );
	memcpy( &packet->chatSize_, buffer, sizeof( int ) );
	buffer += sizeof( int );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_CS_GAME_START_REQ( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_CS_GAME_START_REQ> packet = std::make_shared<PCK_CS_GAME_START_REQ>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->levelName_, buffer, sizeof( char ) * LEVEL_NAME_LENGTH );
	buffer += ( sizeof( char ) * LEVEL_NAME_LENGTH );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SC_GAME_START_ACK( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SC_GAME_START_ACK> packet = std::make_shared<PCK_SC_GAME_START_ACK>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->success_, buffer, sizeof( bool ) );
	buffer += sizeof( bool );
	memcpy( &packet->errorCode_, buffer, sizeof( int ) );
	buffer += sizeof( int );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SC_LOAD_GAME_NOT( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SC_LOAD_GAME_NOT> packet = std::make_shared<PCK_SC_LOAD_GAME_NOT>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->dedicateIp_, buffer, sizeof( char ) * 20 );
	buffer += ( sizeof( char ) * 20 );
	memcpy( &packet->dedicatePort_, buffer, sizeof( int ) );
	buffer += sizeof( int );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_CS_GAME_READY_REQ( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_CS_GAME_READY_REQ> packet = std::make_shared<PCK_CS_GAME_READY_REQ>();
	if( packet == nullptr )
		return false;

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SC_LEAVE_GAME_NOT( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SC_LEAVE_GAME_NOT> packet = std::make_shared<PCK_SC_LEAVE_GAME_NOT>();
	if( packet == nullptr )
		return false;

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_CS_LOBBY_PLAYER_LIST_REQ( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_CS_LOBBY_PLAYER_LIST_REQ> packet = std::make_shared<PCK_CS_LOBBY_PLAYER_LIST_REQ>();
	if( packet == nullptr )
		return false;

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_CS_ROOM_PLAYER_LIST_REQ( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_CS_ROOM_PLAYER_LIST_REQ> packet = std::make_shared<PCK_CS_ROOM_PLAYER_LIST_REQ>();
	if( packet == nullptr )
		return false;

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_CS_ROOM_LIST_REQ( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_CS_ROOM_LIST_REQ> packet = std::make_shared<PCK_CS_ROOM_LIST_REQ>();
	if( packet == nullptr )
		return false;

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SC_ROOM_LIST_NOT( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SC_ROOM_LIST_NOT> packet = std::make_shared<PCK_SC_ROOM_LIST_NOT>();
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

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_CS_CREATE_ROOM_REQ( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_CS_CREATE_ROOM_REQ> packet = std::make_shared<PCK_CS_CREATE_ROOM_REQ>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->roomName_, buffer, sizeof( char ) * ROOM_NAME_LENGTH );
	buffer += ( sizeof( char ) * ROOM_NAME_LENGTH );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SC_CREATE_ROOM_ACK( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SC_CREATE_ROOM_ACK> packet = std::make_shared<PCK_SC_CREATE_ROOM_ACK>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->success_, buffer, sizeof( bool ) );
	buffer += sizeof( bool );
	memcpy( &packet->errorCode_, buffer, sizeof( int ) );
	buffer += sizeof( int );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_DS_CONNECT_REQ( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_DS_CONNECT_REQ> packet = std::make_shared<PCK_DS_CONNECT_REQ>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->dedicateIp_, buffer, sizeof( char ) * 20 );
	buffer += ( sizeof( char ) * 20 );
	memcpy( &packet->dedicatePort_, buffer, sizeof( int ) );
	buffer += sizeof( int );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SD_CONNECT_ACK( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SD_CONNECT_ACK> packet = std::make_shared<PCK_SD_CONNECT_ACK>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->success_, buffer, sizeof( bool ) );
	buffer += sizeof( bool );
	memcpy( &packet->errorCode_, buffer, sizeof( int ) );
	buffer += sizeof( int );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SD_LOAD_GAME_NOT( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SD_LOAD_GAME_NOT> packet = std::make_shared<PCK_SD_LOAD_GAME_NOT>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->levelName_, buffer, sizeof( char ) * LEVEL_NAME_LENGTH );
	buffer += ( sizeof( char ) * LEVEL_NAME_LENGTH );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_DS_GAME_READY_REQ( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_DS_GAME_READY_REQ> packet = std::make_shared<PCK_DS_GAME_READY_REQ>();
	if( packet == nullptr )
		return false;

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SD_START_GAME_NOT( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SD_START_GAME_NOT> packet = std::make_shared<PCK_SD_START_GAME_NOT>();
	if( packet == nullptr )
		return false;

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_DS_END_GAME_REQ( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_DS_END_GAME_REQ> packet = std::make_shared<PCK_DS_END_GAME_REQ>();
	if( packet == nullptr )
		return false;

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SD_END_GAME_ACK( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SD_END_GAME_ACK> packet = std::make_shared<PCK_SD_END_GAME_ACK>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->success_, buffer, sizeof( bool ) );
	buffer += sizeof( bool );
	memcpy( &packet->errorCode_, buffer, sizeof( int ) );
	buffer += sizeof( int );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SC_PING( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SC_PING> packet = std::make_shared<PCK_SC_PING>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->sequence_, buffer, sizeof( int ) );
	buffer += sizeof( int );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_CS_PONG( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_CS_PONG> packet = std::make_shared<PCK_CS_PONG>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->sequence_, buffer, sizeof( int ) );
	buffer += sizeof( int );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool DecodeClientProtocol(EnumClientProtocol protocol, std::shared_ptr<PACKET_HEADER>& pck, char* buffer)
{
	switch( protocol )
	{
	case CS_CONNECT_REQ :
		return decode_CS_CONNECT_REQ( pck, buffer );
	case SC_CONNECT_ACK :
		return decode_SC_CONNECT_ACK( pck, buffer );
	case SC_LOBBY_PLAYER_LIST :
		return decode_SC_LOBBY_PLAYER_LIST( pck, buffer );
	case CS_ROOM_PLAYER_JOIN_REQ :
		return decode_CS_ROOM_PLAYER_JOIN_REQ( pck, buffer );
	case SC_ROOM_PLAYER_JOIN_ACK :
		return decode_SC_ROOM_PLAYER_JOIN_ACK( pck, buffer );
	case CS_ROOM_PLAYER_CHANGE_REQ :
		return decode_CS_ROOM_PLAYER_CHANGE_REQ( pck, buffer );
	case SC_ROOM_PLAYER_CHANGE_ACK :
		return decode_SC_ROOM_PLAYER_CHANGE_ACK( pck, buffer );
	case CS_ROOM_PLAYER_LEAVE_REQ :
		return decode_CS_ROOM_PLAYER_LEAVE_REQ( pck, buffer );
	case SC_ROOM_PLAYER_LEAVE_ACK :
		return decode_SC_ROOM_PLAYER_LEAVE_ACK( pck, buffer );
	case SC_ROOM_PLAYER_LIST :
		return decode_SC_ROOM_PLAYER_LIST( pck, buffer );
	case CS_ROOM_CHAT_REQ :
		return decode_CS_ROOM_CHAT_REQ( pck, buffer );
	case SC_ROOM_CHAT_MSG :
		return decode_SC_ROOM_CHAT_MSG( pck, buffer );
	case CS_GAME_START_REQ :
		return decode_CS_GAME_START_REQ( pck, buffer );
	case SC_GAME_START_ACK :
		return decode_SC_GAME_START_ACK( pck, buffer );
	case SC_LOAD_GAME_NOT :
		return decode_SC_LOAD_GAME_NOT( pck, buffer );
	case CS_GAME_READY_REQ :
		return decode_CS_GAME_READY_REQ( pck, buffer );
	case SC_LEAVE_GAME_NOT :
		return decode_SC_LEAVE_GAME_NOT( pck, buffer );
	case CS_LOBBY_PLAYER_LIST_REQ :
		return decode_CS_LOBBY_PLAYER_LIST_REQ( pck, buffer );
	case CS_ROOM_PLAYER_LIST_REQ :
		return decode_CS_ROOM_PLAYER_LIST_REQ( pck, buffer );
	case CS_ROOM_LIST_REQ :
		return decode_CS_ROOM_LIST_REQ( pck, buffer );
	case SC_ROOM_LIST_NOT :
		return decode_SC_ROOM_LIST_NOT( pck, buffer );
	case CS_CREATE_ROOM_REQ :
		return decode_CS_CREATE_ROOM_REQ( pck, buffer );
	case SC_CREATE_ROOM_ACK :
		return decode_SC_CREATE_ROOM_ACK( pck, buffer );
	case DS_CONNECT_REQ :
		return decode_DS_CONNECT_REQ( pck, buffer );
	case SD_CONNECT_ACK :
		return decode_SD_CONNECT_ACK( pck, buffer );
	case SD_LOAD_GAME_NOT :
		return decode_SD_LOAD_GAME_NOT( pck, buffer );
	case DS_GAME_READY_REQ :
		return decode_DS_GAME_READY_REQ( pck, buffer );
	case SD_START_GAME_NOT :
		return decode_SD_START_GAME_NOT( pck, buffer );
	case DS_END_GAME_REQ :
		return decode_DS_END_GAME_REQ( pck, buffer );
	case SD_END_GAME_ACK :
		return decode_SD_END_GAME_ACK( pck, buffer );
	case SC_PING :
		return decode_SC_PING( pck, buffer );
	case CS_PONG :
		return decode_CS_PONG( pck, buffer );
	default :
		break;
	}

		return false;

}
