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

MessageObject* WorkQueue::AllocObject()
{
	return messagePool_.Alloc();
}

void WorkQueue::RestoreObject( MessageObject* obj )
{
	messagePool_.Free( obj );
}

MessageObject* WorkQueue::Pop()
{
	if( messageQueue_.Empty() == true )
	{
		std::unique_lock<std::mutex> lock( queueLock_ );
		queueCond_.wait( lock );
	}
	return messageQueue_.Pop();
}

void WorkQueue::Push( MessageObject* obj )
{
	messageQueue_.Push( obj );

	std::unique_lock<std::mutex> lock( queueLock_ );
	queueCond_.notify_all();
}
