#pragma once
#include <list>
#include <memory>
#include <algorithm>
#include <mutex>

template< typename t >
class ObjectPool
{
	std::mutex	mutex_;
	int defaultPoolSize_ = 32;
	int maxPoolSize_ = 256;

	std::list<t*>	memoryPool_;

	void _Reallocation()
	{
		defaultPoolSize_ = (std::min)( defaultPoolSize_ * 2, maxPoolSize_ );

		for( int i = 0; i < defaultPoolSize_; ++i )
		{
			t* obj = nullptr;
			try
			{
				obj = new t {};
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
	virtual ~ObjectPool()
	{
		for( auto itr : memoryPool_ )
		{
			delete itr;
		}
	}

	void SetDefaultPoolSize( int size ) { defaultPoolSize_ = size; }
	void SetMaxPoolSize( int size ) { maxPoolSize_ = size; }

	bool Init()
	{
		try
		{
			_Reallocation();
		}
		catch( ... )
		{
			return false;
		}

		return true;
	}

	t* Alloc()
	{
		std::unique_lock<std::mutex> lock { mutex_ };

		if( memoryPool_.empty() == true )
		{
			_Reallocation();
		}

		if( memoryPool_.empty() == true )
		{
			return nullptr;
		}

		t* obj = memoryPool_.front();
		memoryPool_.pop_front();

		new ( obj ) t();

		return obj;
	}

	void Free( t* obj )
	{
		if( obj == nullptr )
			return;

		std::unique_lock<std::mutex> lock { mutex_ };

		obj->~t();
		memoryPool_.push_back( obj );
	}
};
