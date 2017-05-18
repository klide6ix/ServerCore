#pragma once

#include <memory>
#include "Socket.h"

class Session
{
	Socket socket_;

public:
	Session();
	Session( SOCKET socket );
	virtual ~Session();

	void SetSocket( SOCKET socket );
};

typedef std::shared_ptr<Session> SessionPtr;