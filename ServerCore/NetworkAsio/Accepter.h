#pragma once

#include <boost/asio.hpp>

class Session;
class Accepter
{
	boost::asio::ip::tcp::acceptor accepter_;
	int port_ = 0;
	
public:
	Accepter() = default;
	Accepter( boost::asio::io_service& io_service, int port );
	virtual ~Accepter();

	void StartAccept();
};

