#pragma once
#include <memory>

#include "ServerEngineDef.h"

class Packet
{
private:
	char				packetBuffer_[MESSAGE_BUFFER_SIZE_MAX] = {0};
	PacketHeader*		packetHeader_ = reinterpret_cast<PacketHeader*>(packetBuffer_);

public:
	Packet() {}
	virtual ~Packet() {	memset( packetBuffer_, 0, MESSAGE_BUFFER_SIZE_MAX ); }

	void			SetProtocol( PROTOCOL_TYPE protocol ) { (*packetHeader_).packetProtcol_ = protocol; }
	PROTOCOL_TYPE	GetProtocol() { return (*packetHeader_).packetProtcol_; }

	char*			GetPacketBuffer() { return packetBuffer_; }
	SIZE_TYPE		GetPacketSize() { return (*packetHeader_).packetSize_ + HEADER_SIZE; }

	const char*		GetPacketData() const { return packetBuffer_ + HEADER_SIZE; }
	SIZE_TYPE		GetPacketDataSize() const { return (*packetHeader_).packetSize_; }

	bool AddPacketData( const char* data, SIZE_TYPE size )
	{
		int currentPos = GetPacketSize();

		if( currentPos + size > MESSAGE_BUFFER_SIZE_MAX )
			return false;

		memcpy( &packetBuffer_[currentPos], data, static_cast<size_t>(size) );

		(*packetHeader_).packetSize_ += size;

		return true;
	}
};