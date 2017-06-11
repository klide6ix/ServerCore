#include "WorkQueue.h"



WorkQueue::WorkQueue()
{
}


WorkQueue::~WorkQueue()
{
}
bool WorkQueue::Init()
{
	messagePool_.SetMaxPoolSize( 32 * 10 );
	return messagePool_.Init();
}

Packet* WorkQueue::AllocObject()
{
	return messagePool_.Alloc();
}

void WorkQueue::RestoreObject( Packet* obj )
{
	messagePool_.Free( obj );
}

Packet* WorkQueue::Pop()
{
	if( messageQueue_.Empty() == true )
	{
		std::unique_lock<std::mutex> lock( queueLock_ );
		queueCond_.wait( lock );
	}
	return messageQueue_.Pop();
}

void WorkQueue::Push( Packet* obj )
{
	messageQueue_.Push( obj );

	std::unique_lock<std::mutex> lock( queueLock_ );
	queueCond_.notify_one();
}
