#pragma once
#include <map>
#include "ServerEngineDef.h"

class Packet;
class ServerCommandHandler
{
	std::map< PROTOCOL_TYPE, ProtocolCommandFunction_t > serverCommand_;

public:

	void AddServerCommand( PROTOCOL_TYPE protocol, ProtocolCommandFunction_t command );
	ProtocolCommandFunction_t GetServerCommand( PROTOCOL_TYPE protocol );
};