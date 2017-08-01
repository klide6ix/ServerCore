#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "Session.h"
#include "NetworkCore.h"

Session::Session() : socket_(NetworkCore::GetInstance().GetIoService())
{
}

Session::~Session()
{
	CleanUp();
}

bool Session::ConnectTo( const char* ip, int port )
{
	boost::asio::ip::tcp::resolver resolver(NetworkCore::GetInstance().GetIoService());
	boost::asio::ip::tcp::resolver::query query( ip, std::to_string( port ) );
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve( query );

	boost::asio::connect( socket_, endpoint_iterator );

	return true;
}

void Session::_handle_write( const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/ )
{
	//printf("_handle_write(%d)\n", bytes_transferred );
}

void Session::_handle_read( const boost::system::error_code& error, size_t bytes_transferred )
{
	if( bytes_transferred == 0 )
	{
		NetworkCore::GetInstance().CloseSession( this );
		return;
	}

	RecvBufferConsume( static_cast<int>(bytes_transferred) );

	do
	{
		Packet* packet = NetworkCore::GetInstance().AllocatePacket();

		if( packet == nullptr )
			break;

		int packetSize = 0;
		if( NetworkCore::GetInstance().DecodePacket( RecvBufferPos(), static_cast<int>(bytes_transferred), packet->GetPacketBuffer(), packetSize ) == false )
		{
			NetworkCore::GetInstance().FreePacket( packet );
			break;
		}
		else
		{
			NetworkCore::GetInstance().PushCommand( Command( static_cast<COMMAND_ID>(packet->GetProtocol()), static_cast<void*>(packet) ) );
			RecvBufferRestore( packet->GetPacketSize() );
			bytes_transferred -= packet->GetPacketSize();
		}

	} while( bytes_transferred > 0 );

	RecvPost();
}

bool Session::RecvPost()
{
	if( socket_.is_open() == false )
		return false;

	socket_.async_receive( boost::asio::buffer( recvBuffer_.GetBufferPos(), recvBuffer_.GetBufferSize() ),
							  boost::bind( &Session::_handle_read, this,
										   boost::asio::placeholders::error,
										   boost::asio::placeholders::bytes_transferred ) );
	return true;
}

void Session::RecvBufferConsume( int size )
{
	recvBuffer_.ConsumeBuffer( size );
}

void Session::RecvBufferRestore( int size )
{
	recvBuffer_.RestoreBuffer( size );
}

void Session::CleanUp()
{
	if( socket_.is_open() == true )
	{
		socket_.close();
	}
}

int Session::SendPacket( Packet& packet )
{
	boost::asio::async_write( socket_, boost::asio::buffer( packet.GetPacketBuffer(), packet.GetPacketSize() ),
							  boost::bind( &Session::_handle_write, this,
										   boost::asio::placeholders::error,
										   boost::asio::placeholders::bytes_transferred ) );

	return 0;
}