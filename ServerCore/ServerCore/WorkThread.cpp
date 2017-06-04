#include "WorkThread.h"
#include "ServerEngine.h"
#include "MessageObject.h"

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
		MessageObject* message = ServerEngine::GetInstance().PopMessageObject();

		if( message == nullptr )
			continue;

		printf("Message : %s\n", message->messageBuffer_ );
	}
}