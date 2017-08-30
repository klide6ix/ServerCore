#pragma once

#include <memory>
#include <functional>
#include <vector>

#include "../Utility/Packet.h"
#include "../Utility/NetworkBuffer.h"

#include "Socket.h"

// Ȯ�� OVERLAPPED ����ü
#define  ASYNCFLAG_SEND				0x01
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

	NetworkBuffer recvBuffer_;

public:
	Session();
	Session( SOCKET socket );
	virtual ~Session();

	SOCKET GetSocket();
	void SetSocket( Socket& socket );

	bool ConnectTo( const char* ip, int port );
	int  RecvPost();
	int  RecvPacket();
	
	char* RecvBufferPos() { return recvBuffer_.GetBufferPosRead(); }
	void  ReadBufferConsume( int size );
	void  RecvBufferConsume( int size );
	void  ArrangeBuffer();
	int   GetCurrentRecvBufferSize();

	int  SendPacket( Packet& packet );

	void Close();

	void Connect( const std::string& addr, std::uint32_t port );
	void Disconnect( bool wait_for_removal = false );
	bool IsConnected() const;
};
