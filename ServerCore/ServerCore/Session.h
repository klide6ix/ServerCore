#pragma once

#include <memory>
#include "Socket.h"
#include "NetworkBuffer.h"

#define  ASYNCFLAG_SEND				0x01
#define  ASYNCFLAG_RECEIVE			0x02

// 확장 OVERLAPPED 구조체
typedef struct OVERLAPPEDEX : OVERLAPPED
{
	DWORD dwFlags;
} OVERLAPPEDEX;

class Session
{
	Socket socket_;
	
	OVERLAPPEDEX overlappedSend_;
	OVERLAPPEDEX overlappedRecv_;

	NetworkBuffer recvBuffer_;

public:
	Session();
	Session( SOCKET socket );
	virtual ~Session();

	void SetSocket( SOCKET socket );
	SOCKET GetSocket();

	bool RecvPost();
	int  RecvPacket();
	
	char* RecvBufferPos() { return recvBuffer_.GetBufferOrg(); }
	void  RecvBufferConsume( int size );

	int  SendPacket( const char* message, int length );

	void CleanUp();
};
