#pragma once

#include "Socket.h"

class Session
{
	Socket socket_;

public:
	Session();
	Session( SOCKET socket );
	virtual ~Session();
};

