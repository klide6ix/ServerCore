#pragma once

#include <mutex>

#include "../ServerCore/ObjectPool.h"
#include "Session.h"


class SessionManager
{
	ObjectPool<Session>	sessionPool_;
	std::mutex	mutex_;

public:
	SessionManager();
	~SessionManager();

	Session* CreateSession( boost::asio::io_service& io_service );
	void RestoreSession( Session* session );
};

