#include "NetworkThread.h"
#include "NetworkUdp.h"

void NetworkThread::Process()
{
	NetworkUdp::GetInstance().GetUdpIoService().run();
}