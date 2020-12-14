#include <boost/bind/bind.hpp>

#include "Accepter.h"
#include "Session.h"
#include "NetworkCore.h"

Accepter::Accepter(boost::asio::io_service& io_service, int port) : accepter_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), port_(port)
{
}


Accepter::~Accepter()
{
	boost::system::error_code err;
	accepter_.close(err);
}

void Accepter::StartAccept()
{
	Session* newSession = NetworkCore::GetInstance().CreateSession();

	if (newSession == nullptr)
		return;

	accepter_.async_accept(newSession->GetSocket(), [&, this, newSession](boost::system::error_code error)
		{
			if (!error)
			{
				printf("%s\n", error.message().c_str());
			}

			NetworkCore::GetInstance().AddSession(newSession, port_);

			StartAccept();
		});
}

void Accepter::StopAccept()
{
	accepter_.cancel();
	accepter_.close();
}
