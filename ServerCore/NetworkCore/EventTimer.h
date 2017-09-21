#pragma once
#include <mutex>
#include <queue>
#include <unordered_map>

#include "../Utility/ObjectPool.h"

using TIMER_ID = unsigned int;
using TIMER_USE = unsigned int;
using TIMER_TYPE = std::chrono::time_point<std::chrono::system_clock>;

#define TIMER_INFINITE UINT_MAX

class TimerObject
{
public:
	TIMER_TYPE	 timerTime_;
	unsigned int timerDuration_ = 0;
	TIMER_ID	 timerID_ = 0;
	TIMER_USE	 timerUse_ = 1;
	unsigned int timerKey_ = 0;
	void*		 timerMsg_ = nullptr;
	bool		 timerDeleted_ = false;

	bool operator > (const TimerObject& rhs) const
	{
		return timerTime_ > rhs.timerTime_;
	}

};

class EventTimer
{
	unsigned int			timerIdx_ = 0;
	ObjectPool<TimerObject>	timerPool_;
	std::mutex				timerLock_;

	std::priority_queue<TimerObject*>			timerQueue_;
	std::unordered_map<time_t, TimerObject*>	timerMap_;

public:
	EventTimer();
	~EventTimer();

	unsigned int PushTimer( TIMER_ID id, int workCount, int workTime, void* object );
	bool		 PopTimer( TimerObject*& obj );
};

using TimerFunction_t = unsigned int (*)( TimerObject& timerobj );