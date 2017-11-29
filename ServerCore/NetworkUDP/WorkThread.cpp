#include "WorkThread.h"
#include "NetworkUdp.h"

void WorkThread::Process()
{
	while( IsRunning() == true )
	{
		Command* command = NetworkUdp::GetInstance().PopCommand();
		
		if( command == nullptr )
			continue;

		CommandFunction_t workFunc = NetworkUdp::GetInstance().GetServerCommand( command->cmdID_ );

		if( workFunc != nullptr )
		{
			workFunc( command );
		}

		NetworkUdp::GetInstance().DeallocateCommand( command );
	}
}