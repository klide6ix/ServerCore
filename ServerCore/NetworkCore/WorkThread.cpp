#include "WorkThread.h"
#include "NetworkCore.h"

void WorkThread::Process()
{
	while( IsRunning() == true )
	{
		Command* command = NetworkCore::GetInstance().PopCommand();
		if( command == nullptr )
			continue;

		CommandFunction_t workFunc = NetworkCore::GetInstance().GetServerCommand( command->cmdID_ );

		if( workFunc != nullptr )
		{
			workFunc( command );
		}

		NetworkCore::GetInstance().DeallocateCommand( command );
	}
}