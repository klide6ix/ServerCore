#pragma once

#include <vector>

#pragma pack( push, 1 )

struct PACKET_HEADER
{
	unsigned short protocol_ = 0;
	unsigned short size_ = 0;
	
	void setProtocol( unsigned short protocol ) { protocol_ = protocol; }
	void setSize( unsigned short size ) { size_ = size; }
};

#pragma pack( pop )