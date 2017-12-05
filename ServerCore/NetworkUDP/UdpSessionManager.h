#pragma once

#include <mutex>

#include "../Utility/ObjectPool.h"

#include "UdpSession.h"


class UdpSessionManager
{
	ObjectPool<UdpSession>	sessionPool_;
	std::mutex	mutex_;

public:
	UdpSessionManager();
	~UdpSessionManager();

	UdpSession* CreateUdpSession();
	void RestoreUdpSession( UdpSession* session );
};

