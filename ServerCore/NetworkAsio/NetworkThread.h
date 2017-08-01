#pragma once

#include "../Utility/IThread.h"

class NetworkThread : public IThread
{
public:
	NetworkThread() {}
	virtual ~NetworkThread() {}

	virtual void Process();
};

