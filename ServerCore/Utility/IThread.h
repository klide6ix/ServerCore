#pragma once
#include <thread>

class IThread
{
private:

	std::thread*				thread_ = nullptr;
	volatile bool				isRunning_ = false;

public:
	IThread()
	{
	}

	virtual ~IThread()
	{
		StopThread();
	}

	bool IsRunning() { return isRunning_; }

	void StartThread()
	{
		isRunning_ = true;

		thread_ = new std::thread( [&] () { Process(); } );
	}

	void StopThread()
	{
		isRunning_ = false;

		if( thread_ != nullptr )
		{
			if( thread_->joinable() == true )
			{
				thread_->detach();
			}

			delete thread_;
		}

		thread_ = nullptr;
	}

	void JoinThread()
	{
		if( thread_ != nullptr &&
			thread_->joinable() == true )
		{
			thread_->join();
		}
	}

	virtual void Process() = 0;
};

