#include "UdpSession.h"
#include "NetworkUdp.h"

UdpSession::UdpSession() :
	udpSocket_(NetworkUdp::GetInstance().GetUdpIoService())
{
}


UdpSession::~UdpSession()
{
	if( udpSocket_.is_open() == true )
	{
		udpSocket_.close();
	}
}

void UdpSession::StartReceive()
{
	udpSocket_.async_receive_from( boost::asio::buffer( recvBuffer_.GetBuffer(), recvBuffer_.GetBufferRemainSize() ), remoteEndpoint_,
		[this] ( boost::system::error_code const& error, std::size_t bytes_transferred )
	{
		if( !error || error == boost::asio::error::message_size )
		{
			if( NetworkUdp::GetInstance().IsCompleteDatagram( recvBuffer_.GetBufferPosRead(), static_cast<int>(bytes_transferred) ) == false )
			{
				StartReceive();
				return;
			}

			UdpCommand* command = NetworkUdp::GetInstance().AllocateUdpCommand();

			if( command == nullptr )
			{
				StartReceive();
				return;
			}

			command->cmdSession_ = this;

			int packetSize = NetworkUdp::GetInstance().ParseDatagram( recvBuffer_.GetBufferPosRead(), static_cast<int>(bytes_transferred), command );

			if( packetSize == 0 )
			{
				NetworkUdp::GetInstance().DeallocateUdpCommand( command );
			}
			else
			{
				NetworkUdp::GetInstance().PushUdpCommand( command );
			}

			StartReceive();
		}
	});
}

bool UdpSession::InitializeUdpSession( int port )
{
	thisEndpoint_ =  boost::asio::ip::udp::endpoint( boost::asio::ip::udp::v4(), port );
	udpSocket_.bind( thisEndpoint_ );

	StartReceive();

	return true;
}

int UdpSession::SendDatagram( boost::asio::ip::udp::endpoint& sendPoint, BufferSerializer& data )
{
	udpSocket_.async_send_to(boost::asio::buffer(data.GetBuffer(), data.GetSize()), sendPoint,
		[this] ( boost::system::error_code const& error, std::size_t bytes_transferred )
	{
		//printf("SendDatagram(%d)\n", bytes_transferred );
	});

	return 0;
}

void UdpSession::Close()
{
}