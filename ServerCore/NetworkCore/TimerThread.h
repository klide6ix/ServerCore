#pragma once
#include <mutex>

#include "../Utility/IThread.h"

class TimerThread :	public IThread
{
	std::mutex					timerLock_;
	std::condition_variable		timerCond_;

public:
	TimerThread() {}
	~TimerThread() {}

	virtual void Process();
};

