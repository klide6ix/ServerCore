#pragma once

#include "Socket.h"

class Accepter
{
	Socket listenSocket_;

public:
	Accepter();
	~Accepter();

	void InitAccepter( int port );

};

