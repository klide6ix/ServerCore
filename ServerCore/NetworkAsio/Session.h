#pragma once

#include <boost/asio.hpp>

#include "../Utility/Packet.h"
#include "../Utility/NetworkBuffer.h"

class Session : public std::enable_shared_from_this<Session>
{
	void*						sessionObj_ = nullptr;
	NetworkBuffer				recvBuffer_;
	boost::asio::ip::tcp::socket socket_;

	volatile bool				isShutdown_;
	std::atomic<int>			isAbortiveDisconnect_;

	boost::asio::deadline_timer	connectTimeOut_;
	boost::asio::deadline_timer	recvRetry_;
	boost::asio::deadline_timer	sendRetry_;
	
	void _handle_read( const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/ );
	void _handle_read_retry( const boost::system::error_code& /*error*/ );
	void _handle_write( const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/ );

	void _process_recv( size_t bytes_transferred );

public:
	boost::asio::ip::tcp::socket& GetSocket() { return socket_; }
	
	Session();
	~Session();

	void  SetSessionObject( void* obj ) { sessionObj_ = obj; }
	void* GetSessionObject() { return sessionObj_; }

	bool RecvPost();
	bool IsConnected() const;
	bool ConnectTo( const char* ip, int port );
	void Disconnect( bool wait_for_removal );

	bool IsClosed() const;

	char* RecvBufferPos() { return recvBuffer_.GetBufferPosRead(); }
	void  ReadBufferConsume( int size );
	void  RecvBufferConsume( int size );
	void  ArrangeBuffer();
	void  Close();
	void  Shutdown();

	int  SendPacket( Packet& packet );
	
	size_t SendBuffer( const void* buffer, size_t size );
	size_t SendBuffer( std::vector<char>& buffer, size_t size );
	size_t RecvBuffer( std::vector<char>& buffer, size_t size );
};
