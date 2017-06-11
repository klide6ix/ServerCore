#include "WorkThread.h"
#include "ServerEngine.h"
#include "Packet.h"

WorkThread::WorkThread()
{
}


WorkThread::~WorkThread()
{
}

void WorkThread::Process()
{
	while( IsRunning() == true )
	{
		Packet* packet = ServerEngine::GetInstance().PopPacket();

		if( packet == nullptr )
			continue;

		CommandFunction_t workFunc = ServerEngine::GetInstance().GetServerCommand( packet->GetProtocol() );

		if( workFunc != nullptr )
		{
			workFunc( packet->GetProtocol(), packet );
		}

	}
}