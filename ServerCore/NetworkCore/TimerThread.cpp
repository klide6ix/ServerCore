#include <chrono>

#include "TimerThread.h"
#include "NetworkCore.h"

using namespace std::chrono_literals;

void TimerThread::Process()
{
	while( IsRunning() == true )
	{
		{
			std::unique_lock<std::mutex> lock( timerLock_ );
			timerCond_.wait_for( lock, 1ms );	// wait for milliseconds
		}

		TimerObject* timerObj = nullptr;

		if( NetworkCore::GetInstance().PopTimerMessage( timerObj ) == false )
			continue;

		if( timerObj == nullptr )
			continue;

		TimerFunction_t func = NetworkCore::GetInstance().GetTimerCommand( timerObj->timerID_ );

		if( func != nullptr )
		{
			func( *timerObj );
		}
	}
}
