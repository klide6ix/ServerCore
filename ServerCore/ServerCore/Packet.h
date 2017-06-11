#pragma once
#include <memory>

#include "ServerEngineDef.h"

class Packet
{
private:
	char			buffer_[MESSAGE_BUFFER_SIZE_MAX] = {0};

public:
	Packet() {}
	virtual ~Packet()
	{
		memset( buffer_, 0, MESSAGE_BUFFER_SIZE_MAX );
	}

	void SetProtocol( PROTOCOL_TYPE protocol )
	{
		PROTOCOL_TYPE* prot = reinterpret_cast<PROTOCOL_TYPE*>( buffer_ );
		*prot = protocol;
	}

	PROTOCOL_TYPE GetProtocol()
	{
		return *(reinterpret_cast<PROTOCOL_TYPE*>( buffer_ ));
	}

	char* GetPacketBuffer() { return buffer_; }
	SIZE_TYPE GetPacketSize()
	{
		return *(reinterpret_cast<SIZE_TYPE*>(buffer_ + sizeof(PROTOCOL_TYPE))) + HEADER_SIZE;
	}

	bool AddPacketData( const char* data, SIZE_TYPE size )
	{
		int currentPos = GetPacketSize();

		if( currentPos + size > MESSAGE_BUFFER_SIZE_MAX )
			return false;

		memcpy( &buffer_[currentPos], data, static_cast<size_t>(size) );

		*(reinterpret_cast<SIZE_TYPE*>(buffer_ + sizeof(PROTOCOL_TYPE))) += size;

		return true;
	}

	const char* GetPacketData() const { return buffer_ + HEADER_SIZE; }
	SIZE_TYPE GetPacketDataSize()
	{
		return *(reinterpret_cast<SIZE_TYPE*>(buffer_ + sizeof(PROTOCOL_TYPE)));
	}
};