#pragma once

#include "../Utility/IThread.h"

class RedisThread : public IThread
{
public:
	RedisThread();
	virtual ~RedisThread();

	virtual void Process();
};

