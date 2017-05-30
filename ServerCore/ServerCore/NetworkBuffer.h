#pragma once

#define MAX_NET_BUFFER 24000

class NetworkBuffer
{
	CHAR buffer_[MAX_NET_BUFFER];
	int bufferPos_;
	int bufferSize_;

public:
	NetworkBuffer()
	{
		bufferPos_ = 0;
		bufferSize_ = MAX_NET_BUFFER;
	}

	~NetworkBuffer()
	{
	}

	inline CHAR* GetBufferOrg()
	{
		return buffer_;
	}

	inline CHAR* GetBufferPos()
	{
		return buffer_ + bufferPos_;
	}

	inline int GetBufferSize()
	{
		return bufferSize_;
	}

	inline int ConsumeBuffer( int size )
	{
		memmove( buffer_, buffer_ + size, bufferSize_ - size );

		return bufferSize_;
	}
};