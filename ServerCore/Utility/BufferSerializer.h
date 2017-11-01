#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "NetworkBuffer.h"

class BufferSerializer
{
	boost::array<char, MAX_NET_BUFFER>	buffer_ = {};
	char*								bufferPos_ = &buffer_[0];
	unsigned short						bufferSize_ = 0;

	bool _IsOverData( unsigned short size )
	{
		if( MAX_NET_BUFFER < static_cast<size_t>( bufferSize_ + size ) )
			return true;

		return false;
	}

	char* _getBufferFromPos() const { return bufferPos_ + bufferSize_; }

public:
	BufferSerializer() {}
	~BufferSerializer() {}
	
	char*			getBuffer() { return &buffer_[0]; }
	unsigned short	getSize() { return bufferSize_; }

	template< typename TType >
	inline TType* getTypePointer( size_t count = 1 )
	{
		if( _IsOverData( static_cast<unsigned short>( sizeof( TType ) * count ) ) == true )
			return nullptr;

		TType* buf = reinterpret_cast<TType*>( _getBufferFromPos() );
		bufferSize_ += static_cast<unsigned short>( sizeof( TType ) * count );
		return buf;
	}

	template< typename TType >
	inline bool put_data( TType& data )
	{
		if( _IsOverData( sizeof( TType ) ) == true )
			return false;

		memcpy( _getBufferFromPos(), &data, sizeof( TType ) );
		bufferSize_ += static_cast<unsigned short>( sizeof( TType ) );

		return true;
	}

	inline bool put_data( const void* data, unsigned short size )
	{
		if( _IsOverData( size + sizeof( unsigned short ) ) == true )
			return false;

		memcpy( _getBufferFromPos(), &size, sizeof( unsigned short ) );
		bufferSize_ += static_cast<unsigned short>( sizeof( unsigned short ) );
		
		memcpy( _getBufferFromPos(), &data, size );
		bufferSize_ += size;

		return true;
	}
};