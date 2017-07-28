#pragma once

#include <boost/asio.hpp>

#include "../ServerCore/Packet.h"

class Session : public std::enable_shared_from_this<Session>
{
public:
	typedef std::shared_ptr<Session> SessionPtr;

	static SessionPtr CreateSession( boost::asio::io_service& io_service )
	{
		return SessionPtr( new Session( io_service ) );
	}

	boost::asio::ip::tcp::socket& socket()
	{
		return socket_;
	}

	void start()
	{
		boost::asio::async_write( socket_, boost::asio::buffer( message_ ),
								  boost::bind( &tcp_connection::handle_write, shared_from_this(),
											   boost::asio::placeholders::error,
											   boost::asio::placeholders::bytes_transferred ) );
	}

private:
	Session( boost::asio::io_service& io_service ) : socket_( io_service )
	{
	}

	void handle_write( const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/ )
	{
	}

	boost::asio::ip::tcp::socket socket_;
};
