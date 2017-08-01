#include <boost/bind.hpp>

#include "Accepter.h"
#include "Session.h"
#include "NetworkCore.h"

Accepter::Accepter( boost::asio::io_service& io_service,  int port ) : accepter_( io_service, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), port ) ), port_( port )
{
}


Accepter::~Accepter()
{
	boost::system::error_code err;
	accepter_.close( err );
}

void Accepter::_handleAccept( Session* newConnection, const boost::system::error_code& error )
{
	if( error != 0 )
	{
		printf( "%s\n", error.message().c_str() );
	}  

	StartAccept();
	NetworkCore::GetInstance().AddSession( newConnection, port_ );
}

void Accepter::StartAccept()
{
	Session* newSession = NetworkCore::GetInstance().CreateSession();

	if( newSession == nullptr )
		return;

	accepter_.async_accept( newSession->GetSocket(),
							boost::bind( &Accepter::_handleAccept, this, newSession, boost::asio::placeholders::error ) );
}