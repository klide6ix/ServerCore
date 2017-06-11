#pragma once

#include <mutex>
#include <condition_variable>

#include "Packet.h"
#include "ObjectPool.h"
#include "ObjectQueue.h"

class WorkQueue
{
	std::mutex					queueLock_;
	std::condition_variable		queueCond_;

	ObjectPool<Packet>			messagePool_;
	ObjectQueue<Packet>			messageQueue_;

public:
	WorkQueue();
	virtual ~WorkQueue();

	bool				Init();
	Packet*		AllocObject();
	void				RestoreObject( Packet* obj );
	Packet*		Pop();
	void				Push( Packet* obj );
};


