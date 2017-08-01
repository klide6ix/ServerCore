#pragma once
#include <memory>
#include <array>

#define MAX_NET_BUFFER 24000
using NET_BUFFER_TYPE = std::array<char, MAX_NET_BUFFER>;

class NetworkBuffer
{
	NET_BUFFER_TYPE	buffer_;
	int bufferPos_ = 0;
	int bufferSize_ = MAX_NET_BUFFER;

public:
	NetworkBuffer()
	{
	}

	~NetworkBuffer()
	{
	}

	inline NET_BUFFER_TYPE& GetBufferOrg()
	{
		return buffer_;
	}

	inline char* GetBufferPos()
	{
		return buffer_.data() + bufferPos_;
	}

	inline int GetBufferSize()
	{
		return bufferSize_;
	}

	inline int ConsumeBuffer( int size )
	{
		std::memmove( buffer_.data(), buffer_.data() + size, bufferSize_ - size );

		return bufferSize_;
	}
};