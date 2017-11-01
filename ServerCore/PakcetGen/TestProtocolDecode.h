#pragma once
#include <map>
#include "TestProtocol.h"

using DecodeFunction_t = bool (*)( PACKET_HEADER*& pck, char* buffer );
class TestProtocolDecoder
{
	std::map<EnumTestProtocol, DecodeFunction_t> decodeFunction_;

public:
	TestProtocolDecoder();
	~TestProtocolDecoder();

	bool DecodeTestProtocol( EnumTestProtocol protocol, PACKET_HEADER*& pck, char* buffer );
};
