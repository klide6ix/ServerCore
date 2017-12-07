#pragma once

#include <boost/asio.hpp>

#include "../Utility/BufferSerializer.h"

class UdpSession
{
	boost::asio::ip::udp::socket udpSocket_;
	
	boost::asio::ip::udp::endpoint thisEndpoint_;
	boost::asio::ip::udp::endpoint remoteEndpoint_;

	void*						sessionObj_ = nullptr;
	BufferSerializer			recvBuffer_;

	void StartReceive();

public:
	UdpSession();
	~UdpSession();

	boost::asio::ip::udp::endpoint& GetSessionPoint() { return thisEndpoint_; }

	bool InitializeUdpSession( int port );
	int  SendDatagram( boost::asio::ip::udp::endpoint& sendPoint, BufferSerializer& data );

	void Close();
};

