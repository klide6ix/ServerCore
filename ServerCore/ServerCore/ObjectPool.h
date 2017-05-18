#pragma once
#include <list>
#include <memory>
#include <algorithm>

template< typename t >
class ObjectPool
{
	int defaultPoolSize_ = 32;
	int maxPoolSize_ = 1024;

	typedef std::shared_ptr<t> PoolObject;
	std::list<PoolObject>	memoryPool_;

	void _ReAllocation()
	{
		defaultPoolSize_ = std::min( defaultPoolSize_ * 2, maxPoolSize_ );

		for( int i = 0; i < defaultPoolSize_; ++i )
		{
			PoolObject obj;
			try
			{
				obj = std::make_shared<t>();
				new ( obj.get() ) t();
			}
			catch( std::bad_alloc& )
			{
				throw;
			}

			memoryPool_.push_back(obj);
		}
	}

public:

	ObjectPool() {}
	virtual ~ObjectPool() {}

	void SetDefaultPoolSize( int size ) { defaultPoolSize_ = size; }
	void SetMaxPoolSize( int size ) { maxPoolSize_ = size; }

	bool Init()
	{
		try
		{
			_ReAllocation();
		}
		catch( ... )
		{
			return false;
		}

		return true;
	}

	std::shared_ptr<t> Alloc()
	{
		if( memoryPool_.empty() == true )
		{
			_ReAllocation();
		}

		if( memoryPool_.empty() == true )
		{
			return nullptr;
		}

		std::shared_ptr<t> obj = memoryPool_.front();
		memoryPool_.pop_front();

		new ( obj.get() ) t();

		return obj;
	}

	void Free( std::shared_ptr<t> obj )
	{
		obj.get()->~t();
		memoryPool_.push_back( obj );
	}
};
