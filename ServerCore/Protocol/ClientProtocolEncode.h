#pragma once

#include "ClientProtocol.h"

#include ".."Utility"BufferSerializer.h

class IEncodeIterator
{
public:
	virtual void begin() = 0;
	virtual void next() = 0;
	virtual bool hasNext() = 0;
	virtual bool fill( BufferSerializer& serializer ) = 0;
};

inline static bool encode_CS_CONNECT_REQ( BufferSerializer& serializer, const char * nickName_, unsigned short nickNameSize_ );
inline static bool encode_SC_CONNECT_ACK( BufferSerializer& serializer, const MEMBER_INFO& memberInfo_ );
inline static bool encode_SC_LOBBY_PLAYER_LIST( BufferSerializer& serializer, IEncodeIterator* memberList_ );
inline static bool encode_CS_ROOM_PLAYER_JOIN_REQ( BufferSerializer& serializer, const unsigned int& roomId_ );
inline static bool encode_SC_ROOM_PLAYER_JOIN_ACK( BufferSerializer& serializer, const bool& success_, const int& errorCode_, const unsigned int& roomId_, const ENUM_TEAM_TYPE& team_ );
inline static bool encode_CS_ROOM_PLAYER_CHANGE_REQ( BufferSerializer& serializer, const unsigned char& team_ );
inline static bool encode_SC_ROOM_PLAYER_CHANGE_ACK( BufferSerializer& serializer, const bool& success_, const int& errorCode_, const ENUM_TEAM_TYPE& team_ );
inline static bool encode_CS_ROOM_PLAYER_LEAVE_REQ( BufferSerializer& serializer );
inline static bool encode_SC_ROOM_PLAYER_LEAVE_ACK( BufferSerializer& serializer, const bool& success_, const int& errorCode_ );
inline static bool encode_SC_ROOM_PLAYER_LIST( BufferSerializer& serializer, const MEMBER_INFO * blueList_, unsigned short blueListSize_, const MEMBER_INFO * redList_, unsigned short redListSize_ );
inline static bool encode_CS_ROOM_CHAT_REQ( BufferSerializer& serializer, const char * chatMsg_, unsigned short chatMsgSize_ );
inline static bool encode_SC_ROOM_CHAT_MSG( BufferSerializer& serializer, const MEMBER_INFO& sender_, const char * chatMsg_, unsigned short chatMsgSize_ );
inline static bool encode_CS_GAME_START_REQ( BufferSerializer& serializer, const char * levelName_, unsigned short levelNameSize_ );
inline static bool encode_SC_GAME_START_ACK( BufferSerializer& serializer, const bool& success_, const int& errorCode_ );
inline static bool encode_SC_LOAD_GAME_NOT( BufferSerializer& serializer, const char * dedicateIp_, unsigned short dedicateIpSize_, const int& dedicatePort_ );
inline static bool encode_CS_GAME_READY_REQ( BufferSerializer& serializer );
inline static bool encode_SC_LEAVE_GAME_NOT( BufferSerializer& serializer );
inline static bool encode_CS_LOBBY_PLAYER_LIST_REQ( BufferSerializer& serializer );
inline static bool encode_CS_ROOM_PLAYER_LIST_REQ( BufferSerializer& serializer );
inline static bool encode_CS_ROOM_LIST_REQ( BufferSerializer& serializer );
inline static bool encode_SC_ROOM_LIST_NOT( BufferSerializer& serializer, IEncodeIterator* roomList_ );
inline static bool encode_CS_CREATE_ROOM_REQ( BufferSerializer& serializer, const char * roomName_, unsigned short roomNameSize_ );
inline static bool encode_SC_CREATE_ROOM_ACK( BufferSerializer& serializer, const bool& success_, const int& errorCode_ );
inline static bool encode_DS_CONNECT_REQ( BufferSerializer& serializer, const char * dedicateIp_, unsigned short dedicateIpSize_, const int& dedicatePort_ );
inline static bool encode_SD_CONNECT_ACK( BufferSerializer& serializer, const bool& success_, const int& errorCode_ );
inline static bool encode_SD_LOAD_GAME_NOT( BufferSerializer& serializer, const char * levelName_, unsigned short levelNameSize_ );
inline static bool encode_DS_GAME_READY_REQ( BufferSerializer& serializer );
inline static bool encode_SD_START_GAME_NOT( BufferSerializer& serializer );
inline static bool encode_DS_END_GAME_REQ( BufferSerializer& serializer );
inline static bool encode_SD_END_GAME_ACK( BufferSerializer& serializer, const bool& success_, const int& errorCode_ );
inline static bool encode_SC_PING( BufferSerializer& serializer, const int& sequence_ );
inline static bool encode_CS_PONG( BufferSerializer& serializer, const int& sequence_ );

#include "ClientProtocolEncode.inl"
