inline bool decode_CS_ECHO_TEST_REQ( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_CS_ECHO_TEST_REQ> packet = std::make_shared<PCK_CS_ECHO_TEST_REQ>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->data_, buffer, sizeof( char ) * 2048 );
	buffer += ( sizeof( char ) * 2048 );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SC_ECHO_TEST_ACK( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SC_ECHO_TEST_ACK> packet = std::make_shared<PCK_SC_ECHO_TEST_ACK>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->data_, buffer, sizeof( char ) * 2048 );
	buffer += ( sizeof( char ) * 2048 );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_CS_DB_TEST_REQ( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_CS_DB_TEST_REQ> packet = std::make_shared<PCK_CS_DB_TEST_REQ>();
	if( packet == nullptr )
		return false;

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_SC_PING( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_SC_PING> packet = std::make_shared<PCK_SC_PING>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->sequence_, buffer, sizeof( unsigned int ) );
	buffer += sizeof( unsigned int );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool decode_CS_PONG( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )
{
	buffer += sizeof( PACKET_HEADER );

	std::shared_ptr<PCK_CS_PONG> packet = std::make_shared<PCK_CS_PONG>();
	if( packet == nullptr )
		return false;

	memcpy( &packet->sequence_, buffer, sizeof( unsigned int ) );
	buffer += sizeof( unsigned int );

	pck = std::static_pointer_cast<PACKET_HEADER>( packet );

	return true;
};

inline bool DecodeTestProtocol(EnumTestProtocol protocol, std::shared_ptr<PACKET_HEADER>& pck, char* buffer)
{
	switch( protocol )
	{
	case CS_ECHO_TEST_REQ :
		return decode_CS_ECHO_TEST_REQ( pck, buffer );
	case SC_ECHO_TEST_ACK :
		return decode_SC_ECHO_TEST_ACK( pck, buffer );
	case CS_DB_TEST_REQ :
		return decode_CS_DB_TEST_REQ( pck, buffer );
	case SC_PING :
		return decode_SC_PING( pck, buffer );
	case CS_PONG :
		return decode_CS_PONG( pck, buffer );
	default :
		break;
	}

		return false;

}
