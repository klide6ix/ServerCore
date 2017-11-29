#pragma once

#include <mutex>

#include "../Utility/ObjectPool.h"

#include "UdpSession.h"


class SessionManager
{
	ObjectPool<UdpSession>	sessionPool_;
	std::mutex	mutex_;

public:
	SessionManager();
	~SessionManager();

	UdpSession* CreateUdpSession();
	void RestoreUdpSession( UdpSession* session );
};

