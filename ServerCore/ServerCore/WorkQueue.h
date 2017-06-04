#pragma once

#include <mutex>
#include <condition_variable>

#include "MessageObject.h"
#include "ObjectPool.h"
#include "ObjectQueue.h"

class WorkQueue
{
	std::mutex					queueLock_;
	std::condition_variable		queueCond_;

	ObjectPool<MessageObject>	messagePool_;
	ObjectQueue<MessageObject>	messageQueue_;

public:
	WorkQueue();
	virtual ~WorkQueue();

	bool				Init();
	MessageObject*		AllocObject();
	void				RestoreObject( MessageObject* obj );
	MessageObject*		Pop();
	void				Push( MessageObject* obj );
};


