#include <stdio.h>

#include "Session.h"
#include "SessionManager.h"
#include "Application.h"

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

SOCKET Session::GetSocket()
{
	return socket_.GetSocket();
}


bool Session::RecvPost()
{
	if( socket_.GetSocket() == INVALID_SOCKET )
		return false;


	ZeroMemory( &overlappedRecv_, sizeof( OVERLAPPEDEX ) );
	overlappedRecv_.dwFlags = ASYNCFLAG_RECEIVE;

	// WSABUF 구조체 셋팅
	WSABUF wsaBuffer;
	wsaBuffer.buf = recvBuffer_.GetBuffer();
	wsaBuffer.len = recvBuffer_.GetBufferSize();

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
			if( nErrorCode == WSAECONNRESET || nErrorCode == WSAENETRESET || nErrorCode == WSAENOTCONN || nErrorCode == WSAECONNABORTED )
			{
				CleanUp();
			}

			return false;
		}
	}

	return true;
}

void Session::RecvProcess( DWORD bytesTransfer )
{
	recvBuffer_.ConsumeBuffer( static_cast<int>(bytesTransfer) );
}

void Session::RecvPacket()
{
	if( socket_.GetSocket() == INVALID_SOCKET )
	{
		return;
	}

	if( MAX_NET_BUFFER - recvBuffer_.GetBufferSize() <= 0 )
	{
		return;
	}

	//! receive buffer size calculate
	int size = recv( socket_.GetSocket(), recvBuffer_.GetBuffer(), MAX_NET_BUFFER - recvBuffer_.GetBufferSize(), 0  );

	if ( size < 0 )
	{
		int err = GetLastError();
		if( EWOULDBLOCK == err )
		{
			return;
		}
		else
		{
			CleanUp();
			return;
		}
	}
	else if ( size == 0 ) 
	{
		CleanUp();
		return;
	}

	recvBuffer_.ConsumeBuffer( size );
}

void Session::CleanUp()
{
	Application::GetInstance().getSessionManager()->RestoreSession( this );
	Application::GetInstance().RemoveSession( this );

	socket_.CloseSocket();
}