#pragma once
#include <map>
#include "ClientProtocol.h"

using DecodeFunction_t = bool (*)( PACKET_HEADER* pck, char* buffer );
class ClientProtocolDecoder
{
	std::map<EnumClientProtocol, DecodeFunction_t> decodeFunction_;

public:
	ClientProtocolDecoder();
	~ClientProtocolDecoder();

	bool DecodeClientProtocol( EnumClientProtocol protocol, PACKET_HEADER* pck, char* buffer );
};
