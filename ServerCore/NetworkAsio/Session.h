#pragma once

#include <boost/asio.hpp>

#include "../Utility/Packet.h"
#include "../Utility/CircularBuffer.h"

class Session : public std::enable_shared_from_this<Session>
{
	CircularBuffer recvBuffer_;
	boost::asio::ip::tcp::socket socket_;

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

	bool RecvPost();
	bool IsConnected() const;
	bool ConnectTo( const char* ip, int port );
	void Disconnect( bool wait_for_removal );

	char* RecvBufferPos() { return recvBuffer_.GetBufferOrg(); }
	void  RecvBufferRestore( int size );
	void  RecvBufferConsume( int size );
	void  CleanUp();

	int  SendPacket( Packet& packet );

	size_t RecvBuffer( std::vector<char>& buffer, size_t size );
	size_t SendBuffer( std::vector<char>& buffer, size_t size );
};
