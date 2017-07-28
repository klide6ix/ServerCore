#pragma once

#include <memory>
#include <list>
#include <boost/asio.hpp>

class Accepter
{
	boost::asio::ip::tcp::acceptor accepter_;
	
	void _handleAccept( Session* newConnection,	const boost::system::error_code& error );

public:
	Accepter( boost::asio::io_service& io_service, int port );
	virtual ~Accepter();

	void StartAccept();
};

