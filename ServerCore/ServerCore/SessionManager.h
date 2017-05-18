#pragma once

#include "ObjectPool.h"
#include "Session.h"


class SessionManager
{
	ObjectPool<Session>	sessionPool_;

public:
	SessionManager();
	~SessionManager();

	std::shared_ptr<Session> CreateNewSession( SOCKET socket );
};

