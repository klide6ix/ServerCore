#pragma once

#include <list>
#include <mutex>

template< typename t >
class ObjectQueue
{
	std::list<t*>	messageQueue_;
	std::mutex		messageMutex_;

public:

	ObjectQueue() {}
	virtual ~ObjectQueue()
	{
		std::unique_lock<std::mutex> lock { messageMutex_ };

		for( auto itr : messageQueue_ )
		{
			delete itr;
		}
	}

	void Push( t* obj )
	{
		if( obj == nullptr )
			return;

		std::unique_lock<std::mutex> lock { messageMutex_ };

		messageQueue_.push_back( obj );
	}

	t* Pop()
	{
		std::unique_lock<std::mutex> lock { messageMutex_ };

		if( messageQueue_.empty() == true )
		{
			return nullptr;
		}

		t* obj = messageQueue_.front();
		messageQueue_.pop_front();

		return obj;
	}

	bool Empty()
	{
		std::unique_lock<std::mutex> lock { messageMutex_ };
		return messageQueue_.empty();
	}
};
