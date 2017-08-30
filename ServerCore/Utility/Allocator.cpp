#include <boost/functional/hash.hpp>

#include "System.h"
#include "Allocator.h"

#ifdef MEMORY_POOL_LOGGING
allocator_new_delete::mapStackFrequency_t allocator_new_delete::mapAllocateStackFrequency_;
allocator_new_delete::mapStackFrequency_t allocator_new_delete::mapFreeStackFrequency_;
#endif

char* allocator_new_delete::malloc( const std::size_t bytes )
{
	char* p = allocator_new_delete_plain::malloc( bytes + sizeof( std::size_t ) );

	if( p == nullptr )
		return nullptr;

#ifdef MEMORY_POOL_LOGGING
	boost::hash<std::string> hasher;
	stAllocatorCallLog_t log;
	log.stackFrame_ = CSystem::getStackFrame(3);
	size_t hashValue = hasher(log.stackFrame_);
	log.lastAllocatedSize_ = bytes;
	log.totalAllocatedSize_ = bytes;
	log.totalCallCount_ = 1;

	std::pair<mapStackFrequency_t::iterator, bool> insertResult = mapAllocateStackFrequency_.insert( mapStackFrequency_t::value_type( hashValue, log ) );
	if( insertResult.second == false )
	{
		insertResult.first->second.totalAllocatedSize_ += bytes;
		insertResult.first->second.lastAllocatedSize_ = bytes;
		insertResult.first->second.totalCallCount_ += 1;
	}
#endif

	return p + sizeof( std::size_t );
}

void allocator_new_delete::free( char* const block )
{
	char* freeChunk = (block - sizeof( std::size_t ));
	std::size_t freeSize = ((std::size_t*)freeChunk)[0];

#ifdef MEMORY_POOL_LOGGING
	boost::hash<std::string> hasher;
	stAllocatorCallLog_t log;
	log.stackFrame_ = CSystem::getStackFrame(3);
	size_t hashValue = hasher(log.stackFrame_);
	log.lastAllocatedSize_ = freeSize;
	log.totalAllocatedSize_ = freeSize;
	log.totalCallCount_ = 1;

	std::pair<mapStackFrequency_t::iterator, bool> insertResult = mapFreeStackFrequency_.insert( mapStackFrequency_t::value_type( hashValue, log ) );
	if( insertResult.second == false )
	{
		insertResult.first->second.totalAllocatedSize_ += freeSize;
		insertResult.first->second.lastAllocatedSize_ = freeSize;
		insertResult.first->second.totalCallCount_ += 1;
	}
#endif

	allocator_new_delete_plain::free( freeChunk );
}