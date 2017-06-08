#include "WorkThread.h"
#include "ServerEngine.h"
#include "MessageObject.h"

WorkThread::WorkThread()
{
}


WorkThread::~WorkThread()
{
}

static int gIndex = 0;

void WorkThread::Process()
{
	int thisThread = gIndex++;
	printf("Start Thread [%d]\n", thisThread);

	while( IsRunning() == true )
	{
		MessageObject* message = ServerEngine::GetInstance().PopMessageObject();

		if( message == nullptr )
			continue;

		printf("[%d]Message : %s\n", thisThread, message->messageBuffer_ );
	}
}