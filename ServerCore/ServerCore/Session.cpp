#include <stdio.h>

#include "Session.h"
#include "SessionManager.h"
#include "ServerEngine.h"

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
	wsaBuffer.buf = recvBuffer_.GetBufferPos();
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
	printf("%s\n", recvBuffer_.GetBufferOrg() );

	recvBuffer_.ConsumeBuffer( static_cast<int>(bytesTransfer) );
}

int Session::RecvPacket()
{
	if( socket_.GetSocket() == INVALID_SOCKET )
	{
		return -1;
	}

	if( MAX_NET_BUFFER - recvBuffer_.GetBufferSize() <= 0 )
	{
		return -1;
	}

	int size = recv( socket_.GetSocket(), recvBuffer_.GetBufferPos(), MAX_NET_BUFFER - recvBuffer_.GetBufferSize(), 0  );

	if ( size < 0 )
	{
		int err = GetLastError();
		if( EWOULDBLOCK == err )
		{
			return 0;
		}
		else
		{
			CleanUp();
			return -1;
		}
	}
	else if ( size == 0 ) 
	{
		CleanUp();
		return 0;
	}

	recvBuffer_.ConsumeBuffer( size );

	return size;
}

void Session::CleanUp()
{
	ServerEngine::GetInstance().getSessionManager()->RestoreSession( this );
	ServerEngine::GetInstance().RemoveSession( this );

	socket_.CloseSocket();
}

int Session::SendPacket( const char* message, int length )
{
	if( socket_.GetSocket() == INVALID_SOCKET )
		return 0;

	return send( socket_.GetSocket(), message, length, 0 );
}