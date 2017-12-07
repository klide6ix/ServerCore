#include "UdpNetworkThread.h"
#include "NetworkUdp.h"

void UdpNetworkThread::Process()
{
	NetworkUdp::GetInstance().GetUdpIoService().run();
}