#include "Session.h"



Session::Session()
{
}

Session::Session( SOCKET socket )
{
	socket_.SetSocket( socket );
}


Session::~Session()
{
}
