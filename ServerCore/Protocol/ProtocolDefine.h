#pragma once

#include <vector>

#pragma pack( push, 1 )

#define MAX_LOBBY_PLAYER 30
#define MAX_TEAM_PLAYER 3
#define PLAYER_NICKNAME_LENGTH 16
#define LEVEL_NAME_LENGTH 36
#define ROOM_NAME_LENGTH	36
#define MAX_ROOM_COUNT		10

enum ENUM_TEAM_TYPE
{
	TEAM_RED = 0,
	TEAM_BLUE,
};

struct PACKET_HEADER
{
	unsigned short protocol_ = 0;
	unsigned short size_ = 0;
	
	void setProtocol( unsigned short protocol ) { protocol_ = protocol; }
	void setSize( unsigned short size ) { size_ = size; }
};

struct MEMBER_INFO
{
	unsigned long	memberId_ = 0;
	char		 	memberNickName_[PLAYER_NICKNAME_LENGTH] = {};
	bool			isRoomMaster_ = false;
};

enum ENUM_ROOMS_STATE
{
	ROOM_STATE_WAIT = 0,
	ROOM_STATE_GAMING,
};

struct ROOM_INFO
{
	unsigned int	 roomId_ = 0;
	char			 roomName_[ROOM_NAME_LENGTH] = {};
	int				 roomCurrentMember_ = 0;
	int				 roomMaxMember_ = 0;
	ENUM_ROOMS_STATE roomState_ = ROOM_STATE_WAIT;
};

#pragma pack( pop )