inline static bool encode_CS_ECHO_TEST_REQ( BufferSerializer& serializer, const int& data1_, const char * data2_, const unsigned short& data2Size_, IEncodeIterator* data3_ )
{
	PACKET_HEADER* header = serializer.GetTypePointer<PACKET_HEADER>();
	header->protocol_ = CS_ECHO_TEST_REQ;

	serializer.put_data( data1_ );
	serializer.put_data( data2_, data2Size_ );
	unsigned short* count = serializer.GetTypePointer<unsigned short>();
	if( count == nullptr )
		return false;
	(*count) = 0;
	for( data3_->begin(); data3_->hasNext(); data3_->next() )
	{
		data3_->fill( serializer );
		++(*count);
	}

	header->size_ = static_cast<unsigned short>( serializer.GetSize() );

	return true;
}
inline static bool encode_SC_ECHO_TEST_ACK( BufferSerializer& serializer, const int& data1_, const char * data2_, const unsigned short& data2Size_, IEncodeIterator* data3_ )
{
	PACKET_HEADER* header = serializer.GetTypePointer<PACKET_HEADER>();
	header->protocol_ = SC_ECHO_TEST_ACK;

	serializer.put_data( data1_ );
	serializer.put_data( data2_, data2Size_ );
	unsigned short* count = serializer.GetTypePointer<unsigned short>();
	if( count == nullptr )
		return false;
	(*count) = 0;
	for( data3_->begin(); data3_->hasNext(); data3_->next() )
	{
		data3_->fill( serializer );
		++(*count);
	}

	header->size_ = static_cast<unsigned short>( serializer.GetSize() );

	return true;
}
inline static bool encode_CS_DB_TEST_REQ( BufferSerializer& serializer )
{
	PACKET_HEADER* header = serializer.GetTypePointer<PACKET_HEADER>();
	header->protocol_ = CS_DB_TEST_REQ;

	header->size_ = static_cast<unsigned short>( serializer.GetSize() );

	return true;
}
inline static bool encode_SC_PING( BufferSerializer& serializer, const unsigned int& sequence_ )
{
	PACKET_HEADER* header = serializer.GetTypePointer<PACKET_HEADER>();
	header->protocol_ = SC_PING;

	serializer.put_data( sequence_ );

	header->size_ = static_cast<unsigned short>( serializer.GetSize() );

	return true;
}
inline static bool encode_CS_PONG( BufferSerializer& serializer, const unsigned int& sequence_ )
{
	PACKET_HEADER* header = serializer.GetTypePointer<PACKET_HEADER>();
	header->protocol_ = CS_PONG;

	serializer.put_data( sequence_ );

	header->size_ = static_cast<unsigned short>( serializer.GetSize() );

	return true;
}
