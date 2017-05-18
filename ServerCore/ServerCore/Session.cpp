#include <stdio.h>

#include "Session.h"



Session::Session()
{
	printf("Init Session\n");
}

Session::Session( SOCKET socket )
{
	socket_.SetSocket( socket );
}


Session::~Session()
{
	printf("Release Session\n");
}

void Session::SetSocket( SOCKET socket )
{
	socket_.SetSocket( socket );
}