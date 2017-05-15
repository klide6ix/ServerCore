#pragma once

#include "Socket.h"
class Session
{
	Socket socket_;

public:
	Session();
	virtual ~Session();
};

