#pragma once

#include "../Utility/IThread.h"

class UdpWorkThread : public IThread
{
public:
	UdpWorkThread() {}
	virtual ~UdpWorkThread() {}

	virtual void Process();
};

