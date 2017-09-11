#pragma once
#include <memory>

#define MESSAGE_BUFFER_SIZE_MAX	(20000)

class Packet
{
private:
	char				packetBuffer_[MESSAGE_BUFFER_SIZE_MAX] = {0};
	unsigned int		packetProtocol_ = 0;
	unsigned int		packetSize_ = 0;

public:
	Packet()
	{
		memset( packetBuffer_, 0, MESSAGE_BUFFER_SIZE_MAX );
	}

	Packet( const char* data, unsigned int size )
	{
		memset( packetBuffer_, 0, MESSAGE_BUFFER_SIZE_MAX );
		memcpy( packetBuffer_, data, size );
		packetSize_ = size;
	}

	virtual ~Packet() {}

	void			SetProtocol( unsigned int protocol ) { packetProtocol_ = protocol; }
	unsigned int	GetProtocol() { return packetProtocol_; }

	char*			GetPacketBuffer() { return packetBuffer_; }

	void			SetPacketSize( unsigned int size ) { packetSize_ = size; }
	unsigned int	GetPacketSize() { return packetSize_; }

	bool AddPacketData( const char* data, unsigned int size )
	{
		int currentPos = GetPacketSize();

		if( currentPos + size > MESSAGE_BUFFER_SIZE_MAX )
			return false;

		memcpy( &packetBuffer_[currentPos], data, static_cast<size_t>(size) );

		packetSize_ += size;

		return true;
	}
};