#pragma once

#include "TestProtocol.h"

#include "..\Utility\BufferSerializer.h"

class IEncodeIterator
{
public:
	virtual void begin() = 0;
	virtual void next() = 0;
	virtual bool hasNext() = 0;
	virtual bool fill( BufferSerializer& serializer ) = 0;
};

inline static bool encode_CS_ECHO_TEST_REQ( BufferSerializer& serializer, const char * data_, unsigned short dataSize_ );
inline static bool encode_SC_ECHO_TEST_ACK( BufferSerializer& serializer, const char * data_, unsigned short dataSize_ );
inline static bool encode_CS_DB_TEST_REQ( BufferSerializer& serializer );
inline static bool encode_SC_PING( BufferSerializer& serializer, const unsigned int& sequence_ );
inline static bool encode_CS_PONG( BufferSerializer& serializer, const unsigned int& sequence_ );

#include "TestProtocolEncode.inl"
