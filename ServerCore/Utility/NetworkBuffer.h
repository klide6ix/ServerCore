#pragma once

#define MAX_NET_BUFFER 24000

class NetworkBuffer
{
	char buffer_[MAX_NET_BUFFER];
	int bufferPos_ = 0;
	int bufferSize_ = MAX_NET_BUFFER;

public:
	NetworkBuffer()
	{
	}

	~NetworkBuffer()
	{
	}

	inline char* GetBufferOrg()
	{
		return buffer_;
	}

	inline char* GetBufferPos()
	{
		return buffer_ + bufferPos_;
	}

	inline int GetBufferSize()
	{
		return bufferSize_;
	}

	inline int RestoreBuffer( int size )
	{
		std::memmove( buffer_, buffer_ + size, bufferSize_ - size );
		bufferSize_ += size;
		bufferPos_ -= size;

		return bufferSize_;
	}

	inline int ConsumeBuffer( int size )
	{
		if( bufferSize_ < size )
			size = bufferSize_;

		bufferSize_ -= size;
		bufferPos_ += size;

		return bufferSize_;
	}
};