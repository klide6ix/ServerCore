#pragma once

#include <memory>

#include "Packet.h"

class IParser
{
public:
	IParser() {}
	virtual ~IParser() {}

	virtual bool encodeMessage( const char* src, int srcSize, Packet* packet ) = 0;
	virtual bool decodeMessage( const char* src, int srcSize, Packet* packet ) = 0;
};

class ParserDefault : public IParser
{
public:
	ParserDefault() {}
	virtual ~ParserDefault() {}

	virtual bool encodeMessage( const char* src, int srcSize, Packet* packet )
	{
		packet->SetPacketSize( srcSize );
		memcpy( packet->GetPacketBuffer(), src, srcSize );

		return true;
	}
	virtual bool decodeMessage( const char* src, int srcSize, Packet* packet )
	{
		packet->SetPacketSize( static_cast<unsigned int>( srcSize ) );
		memcpy( packet->GetPacketBuffer(), src, srcSize );

		return true;
	}
};
