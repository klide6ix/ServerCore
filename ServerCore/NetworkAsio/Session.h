#pragma once

#include <boost/asio.hpp>

#include "../Utility/Packet.h"
#include "../Utility/NetworkBuffer.h"

class Session : public std::enable_shared_from_this<Session>
{
	NetworkBuffer recvBuffer_;
	boost::asio::ip::tcp::socket socket_;
	
	void _handle_read( const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/ );
	void _handle_write( const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/ );

public:
	boost::asio::ip::tcp::socket& GetSocket() { return socket_; }
	
	Session();
	~Session();

	bool RecvPost();
	bool ConnectTo( const char* ip, int port );

	char* RecvBufferPos() { return recvBuffer_.GetBufferOrg(); }
	void  RecvBufferRestore( int size );
	void  RecvBufferConsume( int size );
	void  CleanUp();

	int  SendPacket( Packet& packet );
};
