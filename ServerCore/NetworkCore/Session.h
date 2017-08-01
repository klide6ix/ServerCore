#pragma once

#include <memory>
#include <functional>
#include <vector>

#include "../Utility/Packet.h"
#include "../Utility/NetworkBuffer.h"

#include "Socket.h"

// 확장 OVERLAPPED 구조체
#define  ASYNCFLAG_SEND				0x01
#define  ASYNCFLAG_RECEIVE			0x02

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
	bool RecvPost();
	int  RecvPacket();
	
	char* RecvBufferPos() { return recvBuffer_.GetBufferOrg(); }
	void  RecvBufferRestore( int size );
	void  RecvBufferConsume( int size );

	int  SendPacket( Packet& packet );

	void CleanUp();

	void Connect( const std::string& addr, std::uint32_t port );
	void Disconnect( bool wait_for_removal = false );
	bool IsConnected() const;
};
