#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

class Socket
{
protected:

	SOCKET	socket_;
	int		lastError_;

public:

	Socket();
	Socket( SOCKET socket );
	virtual ~Socket();

	void	SetSocket( SOCKET socket );
	SOCKET	CreateSocket();
	void	CloseSocket();
	
	SOCKET GetSocket() { return socket_; }

	bool SetBlocking( bool block );

	virtual bool Connect( const char* serveraddress, int port );
};

