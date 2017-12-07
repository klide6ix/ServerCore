#pragma once

#include "../Utility/IThread.h"

class UdpNetworkThread : public IThread
{
public:
	UdpNetworkThread() {}
	virtual ~UdpNetworkThread() {}

	virtual void Process();
};

