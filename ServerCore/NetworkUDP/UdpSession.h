#pragma once

#include <boost/asio.hpp>

#include "../Utility/BufferSerializer.h"

class UdpSession
{
	boost::asio::ip::udp::socket udpSocket_;
	boost::asio::ip::udp::endpoint remote_endpoint_;

	void*						sessionObj_ = nullptr;
	BufferSerializer			recvBuffer_;

	void StartReceive();

public:
	UdpSession();
	~UdpSession();

	bool InitializeUdpSession( int port );
	int  SendTo();

	void Close();
};

