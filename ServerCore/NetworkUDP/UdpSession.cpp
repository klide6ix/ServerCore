#include "UdpSession.h"
#include "NetworkUdp.h"

UdpSession::UdpSession() :
	udpSocket_(NetworkUdp::GetInstance().GetIoService())
{
}


UdpSession::~UdpSession()
{
}

void UdpSession::StartReceive()
{
	udpSocket_.async_receive_from( boost::asio::buffer( recvBuffer_.GetBuffer(), recvBuffer_.GetBufferRemainSize() ), remote_endpoint_,
		[this] ( boost::system::error_code const& error, std::size_t bytes_transferred )
	{
		if( !error || error == boost::asio::error::message_size )
		{
			if( NetworkUdp::GetInstance().IsCompletePacket( recvBuffer_.GetBufferPosRead(), static_cast<int>(bytes_transferred) ) == false )
				break;

			UdpCommand* command = NetworkUdp::GetInstance().AllocateUdpCommand();

			if( command == nullptr )
				break;

			command->cmdSession_ = this;

			int packetSize = NetworkUdp::GetInstance().ParseBuffer( recvBuffer_.GetBufferPosRead(), static_cast<int>(bytes_transferred), command );

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
	boost::asio::ip::udp::endpoint endpoint( boost::asio::ip::udp::v4(), port );
	udpSocket_.bind( endpoint );

	StartReceive();
}

int UdpSession::SendTo()
{
	return 0;
}

void UdpSession::Close()
{
}