#include "UdpSession.h"
#include "Command.h"
#include "NetworkUdp.h"

#include <boost/crc.hpp>

UdpSession::UdpSession() :
	udpSocket_(NetworkUdp::GetInstance().GetUdpIoService())
{
}

UdpSession::~UdpSession()
{
	Close();
}

unsigned int UdpSession::_GetCRC( const char* src, int srcSize )
{
	try
	{
		boost::crc_32_type  result;
		result.process_bytes( src, srcSize );
		
		return result.checksum();
	}
	catch ( std::exception &e )
	{
		printf( "%s\n", e.what() );
		return 0;
	}

	return 0;
}

bool UdpSession::_IsCompleteDatagram( unsigned int crc, const char* src, int srcSize )
{
	if( crc != _GetCRC( src, srcSize ) )
	{
		return false;
	}

	return true;
}

int UdpSession::_ParseDatagram( const char* src, unsigned short srcSize, UdpCommand* command )
{
	if( command == nullptr )
		return 0;

	command->cmdBuffer_.InitializeBuffer( src, srcSize );

	return srcSize;
}

void UdpSession::_StartReceive()
{
	recvBuffer_.ResetBuffer();

	udpSocket_.async_receive_from( boost::asio::buffer( recvBuffer_.GetBuffer(), recvBuffer_.GetBufferRemainSize() ), remoteEndpoint_,
		[this] ( boost::system::error_code const& error, std::size_t bytes_transferred )
	{
		if( !error || error == boost::asio::error::message_size )
		{
			unsigned int crc = 0;
			recvBuffer_.get_data( crc );
			unsigned short size = 0;
			recvBuffer_.get_data( size );

			// 사이즈 체크
			if( size != bytes_transferred - sizeof( unsigned int ) - sizeof( unsigned short ) )
			{
				_StartReceive();
				return;
			}

			// CRC 체크
			if( _IsCompleteDatagram( crc, recvBuffer_.GetBufferPosRead(), size ) == false )
			{
				_StartReceive();
				return;
			}

			UdpCommand* command = NetworkUdp::GetInstance().AllocateUdpCommand();

			if( command == nullptr )
			{
				_StartReceive();
				return;
			}

			command->cmdSession_ = this;
			command->cmdEndPoint_ = remoteEndpoint_;

			int packetSize = _ParseDatagram( recvBuffer_.GetBufferPosRead(), size, command );

			if( packetSize == 0 )
			{
				NetworkUdp::GetInstance().DeallocateUdpCommand( command );
			}
			else
			{
				NetworkUdp::GetInstance().PushUdpCommand( command );
			}

			_StartReceive();
		}
	});
}

bool UdpSession::InitializeUdpSession( int port )
{
	boost::system::error_code errorCode;
	thisEndpoint_ = boost::asio::ip::udp::endpoint( boost::asio::ip::address_v4::any(), port );
	udpSocket_.open(boost::asio::ip::udp::v4(), errorCode);
	
	if( errorCode )
		return false;
	
	udpSocket_.bind(thisEndpoint_, errorCode);
	
	if( errorCode )
		return false;
	
	_StartReceive();

	return true;
}

int UdpSession::SendDatagram( boost::asio::ip::udp::endpoint& sendPoint, BufferSerializer& data )
{
	BufferSerializer datagram;
	unsigned int crc = _GetCRC( data.GetBuffer(), data.GetSize() );
	datagram.put_data( crc );
	datagram.put_data( data.GetBuffer(), data.GetSize() );

	udpSocket_.async_send_to(boost::asio::buffer(datagram.GetBuffer(), datagram.GetSize()), sendPoint,
		[this] ( boost::system::error_code const& error, std::size_t bytes_transferred )
	{
		printf("SendDatagram completed(%d)\n", bytes_transferred );
	});

	return 0;
}

void UdpSession::Close()
{
	if( udpSocket_.is_open() == true )
	{
		udpSocket_.close();
	}
}