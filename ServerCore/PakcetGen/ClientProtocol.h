#pragma once
#pragma pack ( push, 1 )

#include "ProtocolDefine.h"

#define PROTOCOL_VERSION 1075

enum EnumClientProtocol
{
	CS_CONNECT_REQ = 0,
	SC_CONNECT_ACK,
	SC_LOBBY_PLAYER_LIST,
	CS_ROOM_PLAYER_JOIN_REQ,
	SC_ROOM_PLAYER_JOIN_ACK,
	CS_ROOM_PLAYER_CHANGE_REQ,
	SC_ROOM_PLAYER_CHANGE_ACK,
	CS_ROOM_PLAYER_LEAVE_REQ,
	SC_ROOM_PLAYER_LEAVE_ACK,
	SC_ROOM_PLAYER_LIST,
	CS_ROOM_CHAT_REQ,
	SC_ROOM_CHAT_MSG,
	CS_GAME_START_REQ,
	SC_GAME_START_ACK,
	SC_LOAD_GAME_NOT,
	CS_GAME_READY_REQ,
	SC_LEAVE_GAME_NOT,
	CS_LOBBY_PLAYER_LIST_REQ,
	CS_ROOM_PLAYER_LIST_REQ,
	CS_ROOM_LIST_REQ,
	SC_ROOM_LIST_NOT,
	CS_CREATE_ROOM_REQ,
	SC_CREATE_ROOM_ACK,
	DS_CONNECT_REQ = 1000,
	SD_CONNECT_ACK,
	SD_LOAD_GAME_NOT,
	DS_GAME_READY_REQ,
	SD_START_GAME_NOT,
	DS_END_GAME_REQ,
	SD_END_GAME_ACK,
	SC_PING = 9999,
	CS_PONG,
};

struct PCK_CS_CONNECT_REQ : public PACKET_HEADER
{
	char  nickName_[PLAYER_NICKNAME_LENGTH] = {};

	PCK_CS_CONNECT_REQ() : PACKET_HEADER()
	{
		setProtocol( CS_CONNECT_REQ);
		setSize( sizeof( PCK_CS_CONNECT_REQ ) );
	}
};

struct PCK_SC_CONNECT_ACK : public PACKET_HEADER
{
	MEMBER_INFO memberInfo_;

	PCK_SC_CONNECT_ACK() : PACKET_HEADER()
	{
		setProtocol( SC_CONNECT_ACK);
		setSize( sizeof( PCK_SC_CONNECT_ACK ) );
	}
};

struct PCK_SC_LOBBY_PLAYER_LIST : public PACKET_HEADER
{
	std::vector<MEMBER_INFO> memberList_;

	PCK_SC_LOBBY_PLAYER_LIST() : PACKET_HEADER()
	{
		setProtocol( SC_LOBBY_PLAYER_LIST);
		setSize( sizeof( PCK_SC_LOBBY_PLAYER_LIST ) );
	}
};

struct PCK_CS_ROOM_PLAYER_JOIN_REQ : public PACKET_HEADER
{
	unsigned int roomId_ = 0;

	PCK_CS_ROOM_PLAYER_JOIN_REQ() : PACKET_HEADER()
	{
		setProtocol( CS_ROOM_PLAYER_JOIN_REQ);
		setSize( sizeof( PCK_CS_ROOM_PLAYER_JOIN_REQ ) );
	}
};

struct PCK_SC_ROOM_PLAYER_JOIN_ACK : public PACKET_HEADER
{
	bool success_ = false;
	int errorCode_ = 0;
	unsigned int roomId_ = 0;
	ENUM_TEAM_TYPE team_ = TEAM_RED;

	PCK_SC_ROOM_PLAYER_JOIN_ACK() : PACKET_HEADER()
	{
		setProtocol( SC_ROOM_PLAYER_JOIN_ACK);
		setSize( sizeof( PCK_SC_ROOM_PLAYER_JOIN_ACK ) );
	}
};

struct PCK_CS_ROOM_PLAYER_CHANGE_REQ : public PACKET_HEADER
{
	unsigned char team_ = 0;

	PCK_CS_ROOM_PLAYER_CHANGE_REQ() : PACKET_HEADER()
	{
		setProtocol( CS_ROOM_PLAYER_CHANGE_REQ);
		setSize( sizeof( PCK_CS_ROOM_PLAYER_CHANGE_REQ ) );
	}
};

struct PCK_SC_ROOM_PLAYER_CHANGE_ACK : public PACKET_HEADER
{
	bool success_ = false;
	int errorCode_ = 0;
	ENUM_TEAM_TYPE team_ = TEAM_RED;

	PCK_SC_ROOM_PLAYER_CHANGE_ACK() : PACKET_HEADER()
	{
		setProtocol( SC_ROOM_PLAYER_CHANGE_ACK);
		setSize( sizeof( PCK_SC_ROOM_PLAYER_CHANGE_ACK ) );
	}
};

struct PCK_CS_ROOM_PLAYER_LEAVE_REQ : public PACKET_HEADER
{

	PCK_CS_ROOM_PLAYER_LEAVE_REQ() : PACKET_HEADER()
	{
		setProtocol( CS_ROOM_PLAYER_LEAVE_REQ);
		setSize( sizeof( PCK_CS_ROOM_PLAYER_LEAVE_REQ ) );
	}
};

struct PCK_SC_ROOM_PLAYER_LEAVE_ACK : public PACKET_HEADER
{
	bool success_ = false;
	int errorCode_ = 0;

	PCK_SC_ROOM_PLAYER_LEAVE_ACK() : PACKET_HEADER()
	{
		setProtocol( SC_ROOM_PLAYER_LEAVE_ACK);
		setSize( sizeof( PCK_SC_ROOM_PLAYER_LEAVE_ACK ) );
	}
};

struct PCK_SC_ROOM_PLAYER_LIST : public PACKET_HEADER
{
	MEMBER_INFO  blueList_[MAX_TEAM_PLAYER];
	MEMBER_INFO  redList_[MAX_TEAM_PLAYER];

	PCK_SC_ROOM_PLAYER_LIST() : PACKET_HEADER()
	{
		setProtocol( SC_ROOM_PLAYER_LIST);
		setSize( sizeof( PCK_SC_ROOM_PLAYER_LIST ) );
	}
};

struct PCK_CS_ROOM_CHAT_REQ : public PACKET_HEADER
{
	char  chatMsg_[CHAT_MSG_LENGTH] = {};
	int chatSize_ = 0;

	PCK_CS_ROOM_CHAT_REQ() : PACKET_HEADER()
	{
		setProtocol( CS_ROOM_CHAT_REQ);
		setSize( sizeof( PCK_CS_ROOM_CHAT_REQ ) );
	}
};

struct PCK_SC_ROOM_CHAT_MSG : public PACKET_HEADER
{
	MEMBER_INFO sender_;
	char  chatMsg_[CHAT_MSG_LENGTH] = {};
	int chatSize_ = 0;

	PCK_SC_ROOM_CHAT_MSG() : PACKET_HEADER()
	{
		setProtocol( SC_ROOM_CHAT_MSG);
		setSize( sizeof( PCK_SC_ROOM_CHAT_MSG ) );
	}
};

struct PCK_CS_GAME_START_REQ : public PACKET_HEADER
{
	char  levelName_[LEVEL_NAME_LENGTH] = {};

	PCK_CS_GAME_START_REQ() : PACKET_HEADER()
	{
		setProtocol( CS_GAME_START_REQ);
		setSize( sizeof( PCK_CS_GAME_START_REQ ) );
	}
};

struct PCK_SC_GAME_START_ACK : public PACKET_HEADER
{
	bool success_ = false;
	int errorCode_ = 0;

	PCK_SC_GAME_START_ACK() : PACKET_HEADER()
	{
		setProtocol( SC_GAME_START_ACK);
		setSize( sizeof( PCK_SC_GAME_START_ACK ) );
	}
};

struct PCK_SC_LOAD_GAME_NOT : public PACKET_HEADER
{
	char  dedicateIp_[20] = {};
	int dedicatePort_ = 0;

	PCK_SC_LOAD_GAME_NOT() : PACKET_HEADER()
	{
		setProtocol( SC_LOAD_GAME_NOT);
		setSize( sizeof( PCK_SC_LOAD_GAME_NOT ) );
	}
};

struct PCK_CS_GAME_READY_REQ : public PACKET_HEADER
{

	PCK_CS_GAME_READY_REQ() : PACKET_HEADER()
	{
		setProtocol( CS_GAME_READY_REQ);
		setSize( sizeof( PCK_CS_GAME_READY_REQ ) );
	}
};

struct PCK_SC_LEAVE_GAME_NOT : public PACKET_HEADER
{

	PCK_SC_LEAVE_GAME_NOT() : PACKET_HEADER()
	{
		setProtocol( SC_LEAVE_GAME_NOT);
		setSize( sizeof( PCK_SC_LEAVE_GAME_NOT ) );
	}
};

struct PCK_CS_LOBBY_PLAYER_LIST_REQ : public PACKET_HEADER
{

	PCK_CS_LOBBY_PLAYER_LIST_REQ() : PACKET_HEADER()
	{
		setProtocol( CS_LOBBY_PLAYER_LIST_REQ);
		setSize( sizeof( PCK_CS_LOBBY_PLAYER_LIST_REQ ) );
	}
};

struct PCK_CS_ROOM_PLAYER_LIST_REQ : public PACKET_HEADER
{

	PCK_CS_ROOM_PLAYER_LIST_REQ() : PACKET_HEADER()
	{
		setProtocol( CS_ROOM_PLAYER_LIST_REQ);
		setSize( sizeof( PCK_CS_ROOM_PLAYER_LIST_REQ ) );
	}
};

struct PCK_CS_ROOM_LIST_REQ : public PACKET_HEADER
{

	PCK_CS_ROOM_LIST_REQ() : PACKET_HEADER()
	{
		setProtocol( CS_ROOM_LIST_REQ);
		setSize( sizeof( PCK_CS_ROOM_LIST_REQ ) );
	}
};

struct PCK_SC_ROOM_LIST_NOT : public PACKET_HEADER
{
	std::vector<ROOM_INFO> roomList_;

	PCK_SC_ROOM_LIST_NOT() : PACKET_HEADER()
	{
		setProtocol( SC_ROOM_LIST_NOT);
		setSize( sizeof( PCK_SC_ROOM_LIST_NOT ) );
	}
};

struct PCK_CS_CREATE_ROOM_REQ : public PACKET_HEADER
{
	char  roomName_[ROOM_NAME_LENGTH] = {};

	PCK_CS_CREATE_ROOM_REQ() : PACKET_HEADER()
	{
		setProtocol( CS_CREATE_ROOM_REQ);
		setSize( sizeof( PCK_CS_CREATE_ROOM_REQ ) );
	}
};

struct PCK_SC_CREATE_ROOM_ACK : public PACKET_HEADER
{
	bool success_ = false;
	int errorCode_ = 0;

	PCK_SC_CREATE_ROOM_ACK() : PACKET_HEADER()
	{
		setProtocol( SC_CREATE_ROOM_ACK);
		setSize( sizeof( PCK_SC_CREATE_ROOM_ACK ) );
	}
};

struct PCK_DS_CONNECT_REQ : public PACKET_HEADER
{
	char  dedicateIp_[20] = {};
	int dedicatePort_ = 0;

	PCK_DS_CONNECT_REQ() : PACKET_HEADER()
	{
		setProtocol( DS_CONNECT_REQ);
		setSize( sizeof( PCK_DS_CONNECT_REQ ) );
	}
};

struct PCK_SD_CONNECT_ACK : public PACKET_HEADER
{
	bool success_ = false;
	int errorCode_ = 0;

	PCK_SD_CONNECT_ACK() : PACKET_HEADER()
	{
		setProtocol( SD_CONNECT_ACK);
		setSize( sizeof( PCK_SD_CONNECT_ACK ) );
	}
};

struct PCK_SD_LOAD_GAME_NOT : public PACKET_HEADER
{
	char  levelName_[LEVEL_NAME_LENGTH] = {};

	PCK_SD_LOAD_GAME_NOT() : PACKET_HEADER()
	{
		setProtocol( SD_LOAD_GAME_NOT);
		setSize( sizeof( PCK_SD_LOAD_GAME_NOT ) );
	}
};

struct PCK_DS_GAME_READY_REQ : public PACKET_HEADER
{

	PCK_DS_GAME_READY_REQ() : PACKET_HEADER()
	{
		setProtocol( DS_GAME_READY_REQ);
		setSize( sizeof( PCK_DS_GAME_READY_REQ ) );
	}
};

struct PCK_SD_START_GAME_NOT : public PACKET_HEADER
{

	PCK_SD_START_GAME_NOT() : PACKET_HEADER()
	{
		setProtocol( SD_START_GAME_NOT);
		setSize( sizeof( PCK_SD_START_GAME_NOT ) );
	}
};

struct PCK_DS_END_GAME_REQ : public PACKET_HEADER
{

	PCK_DS_END_GAME_REQ() : PACKET_HEADER()
	{
		setProtocol( DS_END_GAME_REQ);
		setSize( sizeof( PCK_DS_END_GAME_REQ ) );
	}
};

struct PCK_SD_END_GAME_ACK : public PACKET_HEADER
{
	bool success_ = false;
	int errorCode_ = 0;

	PCK_SD_END_GAME_ACK() : PACKET_HEADER()
	{
		setProtocol( SD_END_GAME_ACK);
		setSize( sizeof( PCK_SD_END_GAME_ACK ) );
	}
};

struct PCK_SC_PING : public PACKET_HEADER
{
	int sequence_ = 0;

	PCK_SC_PING() : PACKET_HEADER()
	{
		setProtocol( SC_PING);
		setSize( sizeof( PCK_SC_PING ) );
	}
};

struct PCK_CS_PONG : public PACKET_HEADER
{
	int sequence_ = 0;

	PCK_CS_PONG() : PACKET_HEADER()
	{
		setProtocol( CS_PONG);
		setSize( sizeof( PCK_CS_PONG ) );
	}
};

#pragma pack ( pop )
