#pragma once

#include <boost/asio.hpp>

class Session;
class Accepter
{
	boost::asio::ip::tcp::acceptor accepter_;
	int port_ = 0;
	
	void _handleAccept( Session* newConnection,	const boost::system::error_code& error );

public:
	Accepter() = default;
	Accepter( boost::asio::io_service& io_service, int port );
	virtual ~Accepter();

	void StartAccept();
};

