#include "NetworkThread.h"
#include "Application.h"


NetworkThread::NetworkThread()
{
}


NetworkThread::~NetworkThread()
{
}

void NetworkThread::Process()
{
	while( IsRunning() == true )
	{
		Application::GetInstance().SelectSession();
	}
}