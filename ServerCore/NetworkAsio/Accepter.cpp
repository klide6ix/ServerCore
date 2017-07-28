#include "Accepter.h"
#include "Session.h"
#include "NetworkCore.h"

Accepter::Accepter( boost::asio::io_service& io_service,  int port ) : accepter_( io_service, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), port ) )
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

	newSession->start();
	StartAccept();
}

void Accepter::StartAccept()
{
	Session* newSession = NetworkCore::GetInstance()->CreateSession( accepter_.get_io_service() );

	if( newSession == nullptr )
		return;

	accepter_.async_accept( newSession->GetSocket() );
}