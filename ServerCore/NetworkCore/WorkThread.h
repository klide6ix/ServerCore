#pragma once

#include "../Utility/IThread.h"

class WorkThread : public IThread
{
public:
	WorkThread() {}
	virtual ~WorkThread() {}

	virtual void Process();
};

