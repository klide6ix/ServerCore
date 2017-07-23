#pragma once
#include <thread>
#include <vector>

class IThread
{
protected:

	std::vector<std::shared_ptr<std::thread>>	thread_;
	unsigned int				threadCount_ = 1;
	
	volatile bool				isRunning_ = false;

public:
	IThread();
	virtual ~IThread();

	virtual bool IsRunning() { return isRunning_; }

	virtual void StartThread();
	virtual void StopThread();
	virtual void JoinThread();

	virtual void SetThreadCount( unsigned int count ) { threadCount_ = count; }
	virtual void Process() = 0;
};

