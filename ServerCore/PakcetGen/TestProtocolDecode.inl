inline static bool decode_CS_ECHO_TEST_REQ( BufferSerializer& serializer, PCK_CS_ECHO_TEST_REQ& pck )
{
	PACKET_HEADER* header = serializer.GetTypePointer<PACKET_HEADER>();
	if( header->protocol_ != CS_ECHO_TEST_REQ )
		return false;

	serializer.get_data( pck.data1_ );
	unsigned short size = 0;
	serializer.get_data( pck.data2_, size );
	unsigned short* count = serializer.GetTypePointer<unsigned short>();
	if( count == nullptr )
		return false;

	for( unsigned short i = 0; i < (*count); ++i )
	{
		float temp;
		serializer.get_data( temp );
		pck.data3_.push_back( temp );
	}


	return true;
}
inline static bool decode_SC_ECHO_TEST_ACK( BufferSerializer& serializer, PCK_SC_ECHO_TEST_ACK& pck )
{
	PACKET_HEADER* header = serializer.GetTypePointer<PACKET_HEADER>();
	if( header->protocol_ != SC_ECHO_TEST_ACK )
		return false;

	serializer.get_data( pck.data1_ );
	unsigned short size = 0;
	serializer.get_data( pck.data2_, size );
	unsigned short* count = serializer.GetTypePointer<unsigned short>();
	if( count == nullptr )
		return false;

	for( unsigned short i = 0; i < (*count); ++i )
	{
		float temp;
		serializer.get_data( temp );
		pck.data3_.push_back( temp );
	}


	return true;
}
inline static bool decode_CS_DB_TEST_REQ( BufferSerializer& serializer, PCK_CS_DB_TEST_REQ& pck )
{
	PACKET_HEADER* header = serializer.GetTypePointer<PACKET_HEADER>();
	if( header->protocol_ != CS_DB_TEST_REQ )
		return false;


	return true;
}
inline static bool decode_SC_PING( BufferSerializer& serializer, PCK_SC_PING& pck )
{
	PACKET_HEADER* header = serializer.GetTypePointer<PACKET_HEADER>();
	if( header->protocol_ != SC_PING )
		return false;

	serializer.get_data( pck.sequence_ );


	return true;
}
inline static bool decode_CS_PONG( BufferSerializer& serializer, PCK_CS_PONG& pck )
{
	PACKET_HEADER* header = serializer.GetTypePointer<PACKET_HEADER>();
	if( header->protocol_ != CS_PONG )
		return false;

	serializer.get_data( pck.sequence_ );


	return true;
}
