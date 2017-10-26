inline static bool encode_CS_CONNECT_REQ( BufferSerializer& serializer, const char * nickName_, unsigned short nickNameSize_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = CS_CONNECT_REQ;

	serializer.put_data( nickName_, nickNameSize_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_SC_CONNECT_ACK( BufferSerializer& serializer, const MEMBER_INFO& memberInfo_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = SC_CONNECT_ACK;

	serializer.put_data( memberInfo_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_SC_LOBBY_PLAYER_LIST( BufferSerializer& serializer, IEncodeIterator* memberList_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = SC_LOBBY_PLAYER_LIST;

	unsigned short* count = serializer.getTypePointer<unsigned short>();
	if( count == nullptr )
		return false;

	for( memberList_->begin(); memberList_->hasNext(); memberList_->next() )
	{
		memberList_->fill( serializer );
		++(*count);
	}

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_CS_ROOM_PLAYER_JOIN_REQ( BufferSerializer& serializer, const unsigned int& roomId_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = CS_ROOM_PLAYER_JOIN_REQ;

	serializer.put_data( roomId_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_SC_ROOM_PLAYER_JOIN_ACK( BufferSerializer& serializer, const bool& success_, const int& errorCode_, const unsigned int& roomId_, const ENUM_TEAM_TYPE& team_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = SC_ROOM_PLAYER_JOIN_ACK;

	serializer.put_data( success_ );
	serializer.put_data( errorCode_ );
	serializer.put_data( roomId_ );
	serializer.put_data( team_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_CS_ROOM_PLAYER_CHANGE_REQ( BufferSerializer& serializer, const unsigned char& team_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = CS_ROOM_PLAYER_CHANGE_REQ;

	serializer.put_data( team_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_SC_ROOM_PLAYER_CHANGE_ACK( BufferSerializer& serializer, const bool& success_, const int& errorCode_, const ENUM_TEAM_TYPE& team_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = SC_ROOM_PLAYER_CHANGE_ACK;

	serializer.put_data( success_ );
	serializer.put_data( errorCode_ );
	serializer.put_data( team_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_CS_ROOM_PLAYER_LEAVE_REQ( BufferSerializer& serializer )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = CS_ROOM_PLAYER_LEAVE_REQ;

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_SC_ROOM_PLAYER_LEAVE_ACK( BufferSerializer& serializer, const bool& success_, const int& errorCode_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = SC_ROOM_PLAYER_LEAVE_ACK;

	serializer.put_data( success_ );
	serializer.put_data( errorCode_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_SC_ROOM_PLAYER_LIST( BufferSerializer& serializer, const MEMBER_INFO * blueList_, unsigned short blueListSize_, const MEMBER_INFO * redList_, unsigned short redListSize_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = SC_ROOM_PLAYER_LIST;

	serializer.put_data( blueList_, blueListSize_ );
	serializer.put_data( redList_, redListSize_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_CS_ROOM_CHAT_REQ( BufferSerializer& serializer, const char * chatMsg_, unsigned short chatMsgSize_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = CS_ROOM_CHAT_REQ;

	serializer.put_data( chatMsg_, chatMsgSize_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_SC_ROOM_CHAT_MSG( BufferSerializer& serializer, const MEMBER_INFO& sender_, const char * chatMsg_, unsigned short chatMsgSize_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = SC_ROOM_CHAT_MSG;

	serializer.put_data( sender_ );
	serializer.put_data( chatMsg_, chatMsgSize_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_CS_GAME_START_REQ( BufferSerializer& serializer, const char * levelName_, unsigned short levelNameSize_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = CS_GAME_START_REQ;

	serializer.put_data( levelName_, levelNameSize_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_SC_GAME_START_ACK( BufferSerializer& serializer, const bool& success_, const int& errorCode_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = SC_GAME_START_ACK;

	serializer.put_data( success_ );
	serializer.put_data( errorCode_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_SC_LOAD_GAME_NOT( BufferSerializer& serializer, const char * dedicateIp_, unsigned short dedicateIpSize_, const int& dedicatePort_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = SC_LOAD_GAME_NOT;

	serializer.put_data( dedicateIp_, dedicateIpSize_ );
	serializer.put_data( dedicatePort_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_CS_GAME_READY_REQ( BufferSerializer& serializer )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = CS_GAME_READY_REQ;

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_SC_LEAVE_GAME_NOT( BufferSerializer& serializer )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = SC_LEAVE_GAME_NOT;

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_CS_LOBBY_PLAYER_LIST_REQ( BufferSerializer& serializer )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = CS_LOBBY_PLAYER_LIST_REQ;

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_CS_ROOM_PLAYER_LIST_REQ( BufferSerializer& serializer )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = CS_ROOM_PLAYER_LIST_REQ;

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_CS_ROOM_LIST_REQ( BufferSerializer& serializer )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = CS_ROOM_LIST_REQ;

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_SC_ROOM_LIST_NOT( BufferSerializer& serializer, IEncodeIterator* roomList_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = SC_ROOM_LIST_NOT;

	unsigned short* count = serializer.getTypePointer<unsigned short>();
	if( count == nullptr )
		return false;

	for( roomList_->begin(); roomList_->hasNext(); roomList_->next() )
	{
		roomList_->fill( serializer );
		++(*count);
	}

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_CS_CREATE_ROOM_REQ( BufferSerializer& serializer, const char * roomName_, unsigned short roomNameSize_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = CS_CREATE_ROOM_REQ;

	serializer.put_data( roomName_, roomNameSize_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_SC_CREATE_ROOM_ACK( BufferSerializer& serializer, const bool& success_, const int& errorCode_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = SC_CREATE_ROOM_ACK;

	serializer.put_data( success_ );
	serializer.put_data( errorCode_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_DS_CONNECT_REQ( BufferSerializer& serializer, const char * dedicateIp_, unsigned short dedicateIpSize_, const int& dedicatePort_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = DS_CONNECT_REQ;

	serializer.put_data( dedicateIp_, dedicateIpSize_ );
	serializer.put_data( dedicatePort_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_SD_CONNECT_ACK( BufferSerializer& serializer, const bool& success_, const int& errorCode_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = SD_CONNECT_ACK;

	serializer.put_data( success_ );
	serializer.put_data( errorCode_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_SD_LOAD_GAME_NOT( BufferSerializer& serializer, const char * levelName_, unsigned short levelNameSize_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = SD_LOAD_GAME_NOT;

	serializer.put_data( levelName_, levelNameSize_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_DS_GAME_READY_REQ( BufferSerializer& serializer )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = DS_GAME_READY_REQ;

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_SD_START_GAME_NOT( BufferSerializer& serializer )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = SD_START_GAME_NOT;

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_DS_END_GAME_REQ( BufferSerializer& serializer )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = DS_END_GAME_REQ;

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_SD_END_GAME_ACK( BufferSerializer& serializer, const bool& success_, const int& errorCode_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = SD_END_GAME_ACK;

	serializer.put_data( success_ );
	serializer.put_data( errorCode_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_SC_PING( BufferSerializer& serializer, const int& sequence_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = SC_PING;

	serializer.put_data( sequence_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
inline static bool encode_CS_PONG( BufferSerializer& serializer, const int& sequence_ )
{
	PACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();
	header->protocol_ = CS_PONG;

	serializer.put_data( sequence_ );

	header->size_ = static_cast<unsigned short>( serializer.getSize() );

	return true;
}
