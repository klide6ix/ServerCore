#include "NetworkThread.h"
#include "NetworkCore.h"

void NetworkThread::Process()
{
	NetworkCore::GetInstance().GetIoService().run();
}