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

	try
	{
		boost::asio::connect( socket_, endpoint_iterator );
	}
	catch( const boost::system::system_error& /*ex*/ )
	{
		return false;
	}

	return true;
}

void Session::Disconnect( bool /*wait_for_removal*/ )
{
	Close();
}

void Session::_processReceive( size_t bytes_transferred )
{
	do
	{
		if( NetworkCore::GetInstance().IsCompletePacket( recvBuffer_.GetBufferPosRead(), static_cast<int>(bytes_transferred) ) == false )
			break;

		Command* command = NetworkCore::GetInstance().AllocateCommand();

		if( command == nullptr )
			break;

		command->cmdSession_ = this;

		int packetSize = NetworkCore::GetInstance().ParseBuffer( recvBuffer_.GetBufferPosRead(), static_cast<int>(bytes_transferred), command );

		if( packetSize == 0 )
		{
			NetworkCore::GetInstance().DeallocateCommand( command );
			break;
		}
		else
		{
			NetworkCore::GetInstance().PushCommand( command );
			recvBuffer_.ConsumeReadBuffer( packetSize );
			bytes_transferred -= packetSize;
		}

	} while( bytes_transferred > 0 );

	recvBuffer_.ArrageBuffer();
	
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

			_processReceive( recvBuffer_.GetCurrentBufferSize() );
		} );

		return true;
	}

	socket_.async_receive( boost::asio::buffer( recvBuffer_.GetBufferPosRecv(), recvBuffer_.GetBufferRemainSize() ), [this] ( boost::system::error_code const& error, std::size_t bytes_transferred )
	{
		if( bytes_transferred == 0 )
		{
			NetworkCore::GetInstance().CloseSession( this );
			return;
		}

		recvBuffer_.ConsumeRecvBuffer( static_cast<int>(bytes_transferred) );
		_processReceive( bytes_transferred );
	});

	return true;
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

size_t Session::SendBuffer()
{
	boost::asio::async_write( socket_, boost::asio::buffer( sendBuffer_.GetBuffer(), sendBuffer_.GetSize() ),	[this] ( boost::system::error_code const& error, std::size_t bytes_transferred )
	{
		//printf("_handle_write(%d)\n", bytes_transferred );
	});

	return 0;
}

size_t Session::SendBuffer( BufferSerializer& buffer )
{
	boost::asio::async_write( socket_, boost::asio::buffer( buffer.GetBuffer(), buffer.GetSize() ),	[this] ( boost::system::error_code const& error, std::size_t bytes_transferred )
	{
		//printf("_handle_write(%d)\n", bytes_transferred );
	});

	return 0;
}
