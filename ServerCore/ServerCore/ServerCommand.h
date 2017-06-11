#pragma once
#include <map>
#include "ServerEngineDef.h"

class Packet;
class ServerCommandHandler
{
	std::map< PROTOCOL_TYPE, CommandFunction_t > serverCommand_;

public:

	void AddServerCommand( PROTOCOL_TYPE protocol, CommandFunction_t command );
	CommandFunction_t GetServerCommand( PROTOCOL_TYPE protocol );
};