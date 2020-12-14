#pragma once

#include <memory>
#include <functional>
#include <vector>
#include <string>

#include "../Utility/BufferSerializer.h"

#include "Socket.h"

// 확장 OVERLAPPED 구조체
#define  ASYNCFLAG_SEND			0x01
#define  ASYNCFLAG_RECEIVE			0x02
#define  ASYNCFLAG_RECE_RETRY		0x04

typedef struct OVERLAPPEDEX : OVERLAPPED
{
	DWORD dwFlags;
} OVERLAPPEDEX;

class Session
{
	Socket socket_;
	
	OVERLAPPEDEX overlappedSend_;
	OVERLAPPEDEX overlappedRecv_;

	BufferSerializer recvBuffer_;
	BufferSerializer sendBuffer_;

public:
	Session();
	Session( SOCKET socket );
	virtual ~Session();

	SOCKET GetSocket();
	void SetSocket( Socket& socket );

	BufferSerializer& GetReceiveBuffer() { return recvBuffer_; }
	BufferSerializer& GetSendBuffer() { return sendBuffer_; }

	bool ConnectTo( const char* ip, int port );
	int  RecvPost();
	int  RecvPacket();
	
	char* RecvBufferPos() { return recvBuffer_.GetBufferPosRead(); }
	void  ReadBufferConsume( int size );
	void  RecvBufferConsume( int size );
	void  ArrangeBuffer();
	int   GetCurrentRecvBufferSize();

	void Close();

	void Connect( const std::string& addr, std::uint32_t port );
	void Disconnect( bool wait_for_removal = false );
	bool IsConnected() const;
};
