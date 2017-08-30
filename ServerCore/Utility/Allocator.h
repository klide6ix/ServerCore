#pragma once
#include <memory>
#include <string>
#include <map>

struct allocator_new_delete_plain
{
	static char* malloc( const std::size_t bytes )
	{
		char* pointer = new (std::nothrow) char[bytes];

		return pointer;
	}

	static void free( char * const block )
	{
		delete [] block;
	}
};

#define MEMORY_POOL_LOGGING
struct allocator_new_delete
{
#ifdef MEMORY_POOL_LOGGING
	struct stAllocatorCallLog_t
	{
		std::string stackFrame_;
		size_t		totalCallCount_;
		size_t		totalAllocatedSize_;
		size_t		lastAllocatedSize_;
	};

	typedef std::map< size_t, stAllocatorCallLog_t >	mapStackFrequency_t;
	static mapStackFrequency_t mapAllocateStackFrequency_;
	static mapStackFrequency_t mapFreeStackFrequency_;
#endif

	static char* malloc( const std::size_t bytes );
	static void free( char* const block );
};