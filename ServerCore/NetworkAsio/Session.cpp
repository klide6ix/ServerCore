#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "Session.h"
#include "NetworkCore.h"

Session::Session() : 
	socket_(NetworkCore::GetInstance().GetIoService()),
	connectTimeOut_(NetworkCore::GetInstance().GetIoService()),
	recvRetry_(NetworkCore::GetInstance().GetIoService()),
	sendRetry_(NetworkCore::GetInstance().GetIoService())
{
	isShutdown_ = false;
	isAbortiveDisconnect_ = 0;
}

Session::~Session()
{
	sessionObj_ = nullptr;
	Close();
}

bool Session::IsConnected() const
{
	return socket_.is_open();
}

bool Session::ConnectTo( const char* ip, int port )
{
	boost::asio::ip::tcp::resolver resolver(NetworkCore::GetInstance().GetIoService());
	boost::asio::ip::tcp::resolver::query query( ip, std::to_string( port ) );
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve( query );

	boost::asio::connect( socket_, endpoint_iterator );

	return true;
}

void Session::Disconnect( bool /*wait_for_removal*/ )
{
	Close();
}

void Session::ProcessReceive_( size_t bytes_transferred )
{
	do
	{
		Packet* packet = NetworkCore::GetInstance().AllocatePacket();

		if( packet == nullptr )
			break;

		if( NetworkCore::GetInstance().DecodePacket( RecvBufferPos(), static_cast<int>(bytes_transferred), packet ) == false )
		{
			NetworkCore::GetInstance().FreePacket( packet );
			break;
		}
		else
		{
			NetworkCore::GetInstance().PushCommand( Command( static_cast<COMMAND_ID>(packet->GetProtocol()), static_cast<void*>(packet), this ) );
			ReadBufferConsume( packet->GetPacketSize() );
			bytes_transferred -= packet->GetPacketSize();
		}

	} while( bytes_transferred > 0 );

	ArrangeBuffer();
	RecvPost();
}

bool Session::RecvPost()
{
	if( socket_.is_open() == false )
		return false;

	if( recvBuffer_.IsFull() == true )
	{
		recvRetry_.expires_from_now( boost::posix_time::milliseconds(1) );
		recvRetry_.async_wait( [this] ( boost::system::error_code const& error )
		{
			//printf("_handle_read_retry (%d)\n", recvBuffer_.GetCurrentBufferSize());

			ProcessReceive_( recvBuffer_.GetCurrentBufferSize() );
		} );

		return true;
	}

	socket_.async_receive( boost::asio::buffer( recvBuffer_.GetBufferPosRecv(), recvBuffer_.GetBufferSize() ), [this] ( boost::system::error_code const& error, std::size_t bytes_transferred )
	{
		if( bytes_transferred == 0 )
		{
			NetworkCore::GetInstance().CloseSession( this );
			return;
		}

		RecvBufferConsume( static_cast<int>(bytes_transferred) );
		ProcessReceive_( bytes_transferred );
	});

	return true;
}

void Session::RecvBufferConsume( int size )
{
	recvBuffer_.ConsumeRecvBuffer( size );
}

void Session::ReadBufferConsume( int size )
{
	recvBuffer_.ConsumeReadBuffer( size );
}

void Session::ArrangeBuffer()
{
	recvBuffer_.ArrageBuffer();
}

bool Session::IsClosed() const
{
	return isShutdown_ == true || socket_.is_open() == false || isAbortiveDisconnect_ == 1;
}

void Session::Close()
{
	if( socket_.is_open() == false )
		return;
	
	boost::system::error_code errorCode;
	socket_.close(errorCode);

	isAbortiveDisconnect_ = 1;
}

void Session::Shutdown()
{
	if( socket_.is_open() == false )
		return;

	boost::system::error_code errorCode;
	socket_.shutdown( boost::asio::ip::tcp::socket::shutdown_both, errorCode );
	isShutdown_ = true;
}

int Session::SendPacket( Packet& packet )
{
	boost::asio::async_write( socket_, boost::asio::buffer( packet.GetPacketBuffer(), packet.GetPacketSize() ), [this] ( boost::system::error_code const& error, std::size_t bytes_transferred )
	{
		//printf("_handle_write(%d)\n", bytes_transferred );
	});

	return 0;
}

size_t Session::SendBuffer( const void* buffer, size_t size )
{
	boost::asio::async_write( socket_, boost::asio::buffer( buffer, size ),	[this] ( boost::system::error_code const& error, std::size_t bytes_transferred )
	{
		//printf("_handle_write(%d)\n", bytes_transferred );
	});

	return 0;
}

size_t Session::SendBuffer( std::vector<char>& buffer, size_t size )
{
	boost::asio::async_write( socket_, boost::asio::buffer( buffer, size ), [this] ( boost::system::error_code const& error, std::size_t bytes_transferred )
	{
		//printf("_handle_write(%d)\n", bytes_transferred );
	});

	return 0;
}

size_t Session::RecvBuffer( std::vector<char>& buffer, size_t size )
{
	return 0;
}
