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
		std::memmove( buffer_, buffer_ + size, bufferPos_ - size );
		bufferSize_ += size;
		bufferPos_ -= size;

		//printf("RestoreBuffer size(%d), buffer(%d), pos(%d)\n", size, bufferSize_, bufferPos_ );
		return bufferSize_;
	}

	inline int ConsumeBuffer( int size )
	{
		bufferSize_ -= size;
		bufferPos_ += size;

		//printf("ConsumeBuffer size(%d), buffer(%d), pos(%d)\n", size, bufferSize_, bufferPos_ );
		return bufferSize_;
	}

	inline bool IsFull()
	{
		return (bufferSize_ == 0);
	}

	inline int GetCurrentBufferSize()
	{
		return bufferPos_;
	}
};