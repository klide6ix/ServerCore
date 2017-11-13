#include <stdio.h>
#include <string>

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

SOCKET Session::GetSocket()
{
	return socket_.GetSocket();
}

void Session::SetSocket( Socket& socket )
{
	socket_.SetSocket( socket.GetSocket() );
}

bool Session::ConnectTo( const char* ip, int port )
{
	socket_.CreateSocket();

	if( socket_.Connect( "127.0.0.1", port ) == false )
		return 0;

	return true;
}

int Session::RecvPost()
{
	if( socket_.GetSocket() == INVALID_SOCKET )
		return -1;

	if( recvBuffer_.IsFull() == true )
	{
		return 0;
	}

	ZeroMemory( &overlappedRecv_, sizeof( OVERLAPPEDEX ) );
	overlappedRecv_.dwFlags = ASYNCFLAG_RECEIVE;

	// WSABUF 구조체 셋팅
	WSABUF wsaBuffer;
	wsaBuffer.buf = recvBuffer_.GetBufferPosRecv();
	wsaBuffer.len = recvBuffer_.GetBufferRemainSize();

	DWORD dwRecvBytes = 0;
	DWORD dwFlags = 0;	
	int nRet = WSARecv(	socket_.GetSocket(),
		&wsaBuffer,	1,
		&dwRecvBytes,
		&dwFlags,
		(OVERLAPPEDEX *)&overlappedRecv_,
		NULL);

	if( SOCKET_ERROR == nRet )
	{
		int nErrorCode = WSAGetLastError();
		if( nErrorCode != ERROR_IO_PENDING )
		{
			return -1;
		}
	}

	return 1;
}

void Session::RecvBufferConsume( int size )
{
	recvBuffer_.ConsumeRecvBuffer( size );
}

void Session::ReadBufferConsume( int size )
{
	recvBuffer_.ConsumeReadBuffer( size );
}

void Session::ArrangeBuffer()
{
	recvBuffer_.ArrageBuffer();
}

int Session::GetCurrentRecvBufferSize()
{
	return recvBuffer_.GetCurrentBufferSize();
}

int Session::RecvPacket()
{
	if( socket_.GetSocket() == INVALID_SOCKET )
	{
		return -1;
	}

	if( recvBuffer_.GetBufferRemainSize() <= 0 )
	{
		return -1;
	}

	int size = recv( socket_.GetSocket(), recvBuffer_.GetBufferPosRecv(), recvBuffer_.GetBufferRemainSize(), 0  );

	if ( size < 0 )
	{
		int err = GetLastError();
		if( EWOULDBLOCK == err )
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else if ( size == 0 ) 
	{
		return -2;
	}

	return size;
}

void Session::Close()
{
	socket_.CloseSocket();
}

void Session::Connect( const std::string& addr, std::uint32_t port )
{
	if( socket_.GetSocket() == INVALID_SOCKET )
	{
		socket_.CreateSocket();
	}

	socket_.Connect( addr.c_str(), port );
}

void Session::Disconnect( bool wait_for_removal )
{
	socket_.CloseSocket();
}

bool Session::IsConnected() const
{
	return socket_.IsConnected();
}
