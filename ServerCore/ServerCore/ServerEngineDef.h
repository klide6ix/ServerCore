#pragma once

enum SERVER_MODEL
{
	MODEL_EPOLL,
	MODEL_IOCP,
	MODEL_SELECT,
};

class Packet;

typedef unsigned short PROTOCOL_TYPE;
typedef unsigned short SIZE_TYPE;

struct PacketHeader
{
	PROTOCOL_TYPE	packetProtcol_;
	SIZE_TYPE		packetSize_;
};

#define HEADER_SIZE (sizeof(PacketHeader))
#define MESSAGE_BUFFER_SIZE_MAX	(20000)

typedef unsigned int (*CommandFunction_t)( PROTOCOL_TYPE protocol, Packet* messageObj );


