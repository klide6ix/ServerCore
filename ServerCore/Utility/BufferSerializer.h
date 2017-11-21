#pragma once

#include <array>

#define MAX_NET_BUFFER 24000

class BufferSerializer
{
	std::array<char, MAX_NET_BUFFER>	buffer_ = {};
	char*								bufferPos_ = &buffer_[0];
	unsigned int						bufferPosRead_ = 0;
	unsigned int						bufferPosRecv_ = 0;
	unsigned int						bufferRemainSize_ = MAX_NET_BUFFER;

	bool _IsOverData( unsigned short size )
	{
		if( MAX_NET_BUFFER < static_cast<size_t>( bufferPosRead_ + size ) )
			return true;

		return false;
	}

	char* _getBufferFromPos() const { return bufferPos_ + bufferPosRead_; }

public:
	BufferSerializer() {}
	~BufferSerializer() {}

	void ResetBuffer()
	{
		std::fill( buffer_.begin(), buffer_.end(), 0 );
		bufferPosRead_ = 0;
		bufferPosRecv_ = 0;
		bufferRemainSize_ = MAX_NET_BUFFER;
	}

	void InitializeBuffer( const char* msg, unsigned short msgLen )
	{
		memcpy( bufferPos_, msg, msgLen );
		bufferPosRead_ = 0;
	}
	
	char*			GetBuffer() { return &buffer_[0]; }
	unsigned short	GetSize() { return bufferPosRead_; }

	template< typename TType >
	inline TType* GetTypePointer( size_t count = 1 )
	{
		if( _IsOverData( static_cast<unsigned short>( sizeof( TType ) * count ) ) == true )
			return nullptr;

		TType* buf = reinterpret_cast<TType*>( _getBufferFromPos() );
		bufferPosRead_ += static_cast<unsigned short>( sizeof( TType ) * count );
		return buf;
	}

	template< typename TType >
	inline bool put_data( TType& data )
	{
		if( _IsOverData( sizeof( TType ) ) == true )
			return false;

		memcpy( _getBufferFromPos(), &data, sizeof( TType ) );
		bufferPosRead_ += static_cast<unsigned short>( sizeof( TType ) );

		return true;
	}

	template< typename TType >
	inline bool get_data( TType& data )
	{
		memcpy( &data, _getBufferFromPos(), sizeof( TType ) );
		bufferPosRead_ += static_cast<unsigned short>( sizeof( TType ) );

		return true;
	}

	inline bool put_data( const void* data, unsigned short size )
	{
		if( _IsOverData( size + sizeof( unsigned short ) ) == true )
			return false;

		memcpy( _getBufferFromPos(), &size, sizeof( unsigned short ) );
		bufferPosRead_ += static_cast<unsigned short>( sizeof( unsigned short ) );
		
		memcpy( _getBufferFromPos(), data, size );
		bufferPosRead_ += size;

		return true;
	}

	inline bool get_data( void* data, unsigned short& size )
	{
		memcpy( &size, _getBufferFromPos(), sizeof( unsigned short ) );
		bufferPosRead_ += static_cast<unsigned short>( sizeof( unsigned short ) );

		memcpy( data, _getBufferFromPos(), size );
		bufferPosRead_ += size;

		return true;
	}

	// network receive function
	inline char* GetBufferPosRead()
	{
		return bufferPos_ + bufferPosRead_;
	}

	inline char* GetBufferPosRecv()
	{
		return bufferPos_ + bufferPosRecv_;
	}

	inline int GetBufferRemainSize()
	{
		return bufferRemainSize_;
	}

	inline int ConsumeRecvBuffer( int size )
	{
		bufferRemainSize_ -= size;
		bufferPosRecv_ += size;

		//printf("ConsumeRecvBuffer size(%d), buffer(%d), pos(%d)\n", size, bufferRemainSize_, bufferPos_ );
		return bufferRemainSize_;
	}

	inline int ConsumeReadBuffer( int size )
	{
		bufferPosRead_ += size;

		return bufferPosRead_;
	}

	inline void ArrageBuffer()
	{
		// Recv한 만큼 모두 Buffer으로 Read 되었다. 처음부터 다시 Recv 한다.
		if( bufferPosRead_ > bufferPosRecv_ )
		{
			// 나오면 안되는 상황.
			printf("ArrageBuffer Error : BufferRead(%d), BufferRecv(%d)\n", bufferPosRead_, bufferPosRecv_ );
		}
		else if( bufferPosRead_ == bufferPosRecv_ )
		{
			bufferPosRead_ = 0;
			bufferPosRecv_ = 0;
			bufferRemainSize_ = MAX_NET_BUFFER;

			//printf("ArrageBuffer Complete\n" );
		}
		else
		{
			int remainSize = bufferPosRecv_ - bufferPosRead_;
			std::memmove( buffer_.data(), buffer_.data() + bufferPosRead_, remainSize );
			bufferRemainSize_ = MAX_NET_BUFFER - remainSize;
			bufferPosRecv_ = remainSize;
			bufferPosRead_ = 0;

			//printf("ArrageBuffer Remain : BufferRecv(%d)\n", bufferPosRecv_ );
		}
	}

	inline bool IsFull()
	{
		return (bufferRemainSize_ == 0);
	}

	inline int GetCurrentBufferSize()
	{
		if( bufferPosRecv_ > bufferPosRead_ )
			return bufferPosRecv_ - bufferPosRead_;

		return 0;
	}
};