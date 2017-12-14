#pragma once

#include <boost/asio.hpp>

#include "../Utility/BufferSerializer.h"

class UdpCommand;
class UdpSession
{
	boost::asio::ip::udp::socket udpSocket_;
	
	boost::asio::ip::udp::endpoint thisEndpoint_;
	boost::asio::ip::udp::endpoint remoteEndpoint_;

	void*						sessionObj_ = nullptr;
	BufferSerializer			recvBuffer_;

	unsigned int _GetCRC( const char* src, int srcSize );
	bool _IsCompleteDatagram( unsigned int crc, const char* src, int srcSize );
	int	 _ParseDatagram( const char* src, unsigned short srcSize, UdpCommand* command );

	void _StartReceive();

public:
	UdpSession();
	~UdpSession();

	boost::asio::ip::udp::endpoint& GetSessionPoint() { return thisEndpoint_; }

	bool InitializeUdpSession( int port );
	int  SendDatagram( boost::asio::ip::udp::endpoint& sendPoint, BufferSerializer& data );

	void Close();
};

