#pragma once
#include <thread>
#include <vector>

class IThread
{
private:

	std::vector<std::shared_ptr<std::thread>>	thread_;
	unsigned int				threadCount_ = 1;
	
	volatile bool				isRunning_ = false;

public:
	IThread();
	virtual ~IThread();

	bool IsRunning() { return isRunning_; }

	void StartThread();
	void StopThread();
	void JoinThread();

	virtual void SetThreadCount( unsigned int count ) { threadCount_ = count; }
	virtual void Process() = 0;
};

