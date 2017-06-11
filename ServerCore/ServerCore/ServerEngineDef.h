#pragma once

enum SERVER_MODEL
{
	MODEL_EPOLL,
	MODEL_IOCP,
	MODEL_SELECT,
};

class Packet;

typedef unsigned short PROTOCOL_TYPE;
typedef unsigned int (*CommandFunction_t)( PROTOCOL_TYPE protocol, Packet* messageObj );


