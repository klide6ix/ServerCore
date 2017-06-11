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

		printf("[%d]Message(%d) : %s\n", packet->GetProtocol(), packet->GetPacketSize(), packet->GetPacketData() + 4 );
	}
}