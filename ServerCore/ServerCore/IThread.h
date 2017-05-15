#pragma once
#include <thread>

class IThread
{
private:

	std::thread* thread_;
	bool isRunning_;

public:
	IThread();
	virtual ~IThread();

	bool IsRunning() { return isRunning_; }

	void StartThread();
	void StopThread();
	void JoinThread();

	virtual void Process() = 0;
};

