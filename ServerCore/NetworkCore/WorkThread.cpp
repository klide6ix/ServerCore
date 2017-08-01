#include "WorkThread.h"
#include "NetworkCore.h"
#include "../Utility/Packet.h"

void WorkThread::Process()
{
	while( IsRunning() == true )
	{
		Command command;
		if( NetworkCore::GetInstance().PopCommand( command ) == false )
			continue;

		if( command.cmdMessage_ == nullptr )
			continue;

		CommandFunction_t workFunc = NetworkCore::GetInstance().GetServerCommand( command.cmdID_ );

		if( workFunc != nullptr )
		{
			workFunc( command );
		}

		NetworkCore::GetInstance().FreePacket( static_cast<Packet*>(command.cmdMessage_) );
	}
}