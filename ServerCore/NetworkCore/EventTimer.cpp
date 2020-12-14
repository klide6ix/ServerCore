#include <ctime>
#include <chrono>

#include "EventTimer.h"

EventTimer::EventTimer()
{
	timerPool_.SetMaxPoolSize( 32 * 100 );
	timerPool_.Init();
}


EventTimer::~EventTimer()
{
	std::unique_lock<std::mutex> lock { timerLock_ };

	for( auto itr : timerMap_ )
	{
		if( itr.second  != nullptr )
		{
			delete itr.second;
		}
	}
}

unsigned int EventTimer::PushTimer( TIMER_ID id, int workCount, int workTime, void* object )
{
	std::unique_lock<std::mutex> lock { timerLock_ };

	TimerObject* obj = timerPool_.Alloc();

	if( obj == nullptr )
		return 0;

	obj->timerTime_ = std::chrono::system_clock::now() + std::chrono::seconds(workTime);
	obj->timerDuration_ = workTime;
	obj->timerID_ = id;
	obj->timerKey_ = timerIdx_++;
	obj->timerCount_ = workCount;
	obj->timerMsg_ = object;

	timerQueue_.push( obj );
	timerMap_.insert( std::pair< unsigned int, TimerObject* >( obj->timerKey_, obj ) );

	return obj->timerKey_;
}

bool EventTimer::PopTimer( TimerObject*& obj )
{
	std::unique_lock<std::mutex> lock { timerLock_ };

	if( timerQueue_.empty() == true )
		return false;

	if( timerQueue_.top()->timerTime_ > std::chrono::system_clock::now() )
		return false;

	obj = timerQueue_.top();
	timerQueue_.pop();

	if( obj->timerCount_ == TIMER_INFINITE )
	{
		obj->timerTime_ = std::chrono::system_clock::now() + std::chrono::seconds( obj->timerDuration_ );
		timerQueue_.push( obj );
	}
	else if( obj->timerCount_ > 0 )
	{
		--obj->timerCount_;

		if( obj->timerCount_ > 0 )
		{
			obj->timerTime_ = std::chrono::system_clock::now() + std::chrono::seconds( obj->timerDuration_ );
			timerQueue_.push( obj );
		}
		else
		{
			timerMap_.erase( obj->timerKey_ );
		}
	}

	return true;
}