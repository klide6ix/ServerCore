#pragma once



class NetworkBuffer
{
	char buffer_[MAX_NET_BUFFER] = {0,};
	int bufferPosRead_ = 0; // ��Ŷ���� �Ľ̵� ����
	int bufferPosRecv_ = 0; // ��Ʈ��ũ���� ���� ����
	int bufferSize_ = MAX_NET_BUFFER; // ���� ������

public:
	NetworkBuffer()
	{
	}

	~NetworkBuffer()
	{
	}

	inline char* GetBufferPosRead()
	{
		return buffer_ + bufferPosRead_;
	}

	inline char* GetBufferPosRecv()
	{
		return buffer_ + bufferPosRecv_;
	}

	inline int GetBufferSize()
	{
		return bufferSize_;
	}

	inline int ConsumeRecvBuffer( int size )
	{
		bufferSize_ -= size;
		bufferPosRecv_ += size;

		//printf("ConsumeRecvBuffer size(%d), buffer(%d), pos(%d)\n", size, bufferSize_, bufferPos_ );
		return bufferSize_;
	}

	inline int ConsumeReadBuffer( int size )
	{
		bufferPosRead_ += size;

		return bufferPosRead_;
	}

	inline void ArrageBuffer()
	{
		// Recv�� ��ŭ ��� Packet���� Read �Ǿ���. ó������ �ٽ� Recv �Ѵ�.
		if( bufferPosRead_ > bufferPosRecv_ )
		{
			// ������ �ȵǴ� ��Ȳ.
			printf("ArrageBuffer Error : BufferRead(%d), BufferRecv(%d)\n", bufferPosRead_, bufferPosRecv_ );
		}
		else if( bufferPosRead_ == bufferPosRecv_ )
		{
			bufferPosRead_ = 0;
			bufferPosRecv_ = 0;
			bufferSize_ = MAX_NET_BUFFER;

			//printf("ArrageBuffer Complete\n" );
		}
		else
		{
			int remainSize = bufferPosRecv_ - bufferPosRead_;
			std::memmove( buffer_, buffer_ + bufferPosRead_, remainSize );
			bufferSize_ = MAX_NET_BUFFER - remainSize;
			bufferPosRecv_ = remainSize;
			bufferPosRead_ = 0;

			//printf("ArrageBuffer Remain : BufferRecv(%d)\n", bufferPosRecv_ );
		}
	}

	inline bool IsFull()
	{
		return (bufferSize_ == 0);
	}

	inline int GetCurrentBufferSize()
	{
		if( bufferPosRecv_ > bufferPosRead_ )
			return bufferPosRecv_ - bufferPosRead_;

		return 0;
	}
};