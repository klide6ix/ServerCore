#pragma once

#include <mutex>

#include "../Utility/ObjectPool.h"
#include "Session.h"


class SessionManager
{
	ObjectPool<Session>	sessionPool_;
	std::mutex	mutex_;

public:
	SessionManager();
	~SessionManager();

	Session* CreateSession();
	void RestoreSession( Session* session );
};

