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

#define HEADER_SIZE (sizeof(PROTOCOL_TYPE) + sizeof(SIZE_TYPE))
#define MESSAGE_BUFFER_SIZE_MAX	(20000)

typedef unsigned int (*CommandFunction_t)( PROTOCOL_TYPE protocol, Packet* messageObj );


