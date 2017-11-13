#pragma once

#include <boost/asio.hpp>

#include "../Utility/BufferSerializer.h"

class Session : public std::enable_shared_from_this<Session>
{
	void*						sessionObj_ = nullptr;
	BufferSerializer			recvBuffer_;
	BufferSerializer			sendBuffer_;
	
	boost::asio::ip::tcp::socket socket_;

	volatile bool				isShutdown_;
	std::atomic<int>			isAbortiveDisconnect_;

	boost::asio::deadline_timer	connectTimeOut_;
	boost::asio::deadline_timer	recvRetry_;
	boost::asio::deadline_timer	sendRetry_;
	
	void _processReceive( size_t bytes_transferred );

public:
	boost::asio::ip::tcp::socket& GetSocket() { return socket_; }
	
	Session();
	~Session();
	
	BufferSerializer& GetReceiveBuffer() { return recvBuffer_; }
	BufferSerializer& GetSendBuffer() { return sendBuffer_; }

	void  SetSessionObject( void* obj ) { sessionObj_ = obj; }
	void* GetSessionObject() { return sessionObj_; }

	bool RecvPost();
	bool IsConnected() const;
	bool ConnectTo( const char* ip, int port );
	void Disconnect( bool wait_for_removal );

	bool IsClosed() const;
	void Close();
	void Shutdown();

	size_t SendBuffer();
	size_t SendBuffer( BufferSerializer& buffer );
};
