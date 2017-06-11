#pragma once

#include <memory>

#include "Packet.h"

class IParser
{
public:
	IParser() {}
	virtual ~IParser() {}

	virtual bool encodeMessage( Packet* packet, char* dest, int& destSize ) = 0;
	virtual bool decodeMessage( const char* src, int srcSize, Packet* packet ) = 0;
};

class ParserDefault : public IParser
{
public:
	ParserDefault() {}
	virtual ~ParserDefault() {}

	virtual bool encodeMessage( Packet* packet, char* dest, int& destSize )
	{
		destSize = (static_cast<int>(packet->GetPacketSize()));
		memcpy( dest, packet->GetPacketBuffer(), destSize );

		return true;
	}
	virtual bool decodeMessage( const char* src, int srcSize, Packet* packet )
	{
		if( srcSize < sizeof( PROTOCOL_TYPE ) + sizeof( unsigned short ) )
			return false;

		memcpy( packet->GetPacketBuffer(), src, srcSize );

		if( packet->GetPacketSize() < srcSize )
			return false;

		return true;
	}
};
