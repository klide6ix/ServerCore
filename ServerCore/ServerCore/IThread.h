#pragma once
#include <thread>
#include <vector>

class IThread
{
private:

	std::vector<std::thread*>	thread_;
	int							threadCount_ = 1;
	
	volatile bool				isRunning_ = false;

public:
	IThread();
	virtual ~IThread();

	void SetThreadCount( int count ) { threadCount_ = count; }
	bool IsRunning() { return isRunning_; }

	void StartThread();
	void StopThread();
	void JoinThread();

	virtual void Process() = 0;
};
