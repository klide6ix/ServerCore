#include "ServerCommand.h"

void ServerCommandHandler::AddServerCommand( PROTOCOL_TYPE protocol, ProtocolCommandFunction_t command )
{
	if( serverCommand_.find( protocol ) == serverCommand_.end() )
	{
		serverCommand_.insert( std::pair< PROTOCOL_TYPE, ProtocolCommandFunction_t >( protocol, command ) );
	}
	else
	{
		serverCommand_[protocol] = command;
	}
}

ProtocolCommandFunction_t ServerCommandHandler::GetServerCommand( PROTOCOL_TYPE protocol )
{
	if( serverCommand_.find( protocol ) == serverCommand_.end() )
	{
		return nullptr;
	}

	return serverCommand_[protocol];
}
