#include "UdpWorkThread.h"
#include "NetworkUdp.h"

void UdpWorkThread::Process()
{
	while( IsRunning() == true )
	{
		UdpCommand* command = NetworkUdp::GetInstance().PopUdpCommand();
		
		if( command == nullptr )
			continue;

		UdpCommandFunction_t workFunc = NetworkUdp::GetInstance().GetServerCommand( command->cmdID_ );

		if( workFunc != nullptr )
		{
			workFunc( command );
		}

		NetworkUdp::GetInstance().DeallocateUdpCommand( command );
	}
}