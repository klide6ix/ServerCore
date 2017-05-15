#pragma once
#include "Socket.h"

class ServerSocket : public Socket
{
	int listenPort_;

public:
	ServerSocket();
	virtual ~ServerSocket();

	SOCKET InitServerSocket( int listenPort );
	int Bind();
	int Listen();
	SOCKET Accept();
};

