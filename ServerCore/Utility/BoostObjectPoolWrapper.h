#pragma once

#include <boost/pool/object_pool.hpp>
#include "Allocator.h"

template< typename T, typename TAllocator = allocator_new_delete_plain>
class BoostObjectPoolWrapper
{
protected:

	typedef boost::object_pool<T, TAllocator>	pool_t;
	pool_t pool_;

public:
	BoostObjectPoolWrapper() {}
	virtual ~BoostObjectPoolWrapper() {}

	T* malloc()
	{
		return pool_.malloc();
	}

	T* construct()
	{
		return pool_.construct();
	}

	void free( T* p )
	{
		pool_.free( reinterpret_cast<char*>(p) );
	}

	void destroy( T* p )
	{
		pool_.destroy( p );
	}
};
