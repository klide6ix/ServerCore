#include "Command.h"

UdpCommandQueue::UdpCommandQueue()
{
}

UdpCommandQueue::~UdpCommandQueue()
{
	{
		std::unique_lock<std::mutex> lock { commandMutex_ };

		for( auto itr : commandQueue_ )
		{
			if( itr != nullptr )
				delete itr;
		}

		commandQueue_.clear();
	}

	std::unique_lock<std::mutex> lock( queueLock_ );
	queueCond_.notify_one();
}

bool UdpCommandQueue::_Empty()
{
	std::unique_lock<std::mutex> lock { commandMutex_ };
	return commandQueue_.empty();
}

UdpCommand* UdpCommandQueue::PopCommand()
{
	if( _Empty() == true )
	{
		std::unique_lock<std::mutex> lock( queueLock_ );
		queueCond_.wait( lock );
	}
	
	std::unique_lock<std::mutex> lock { commandMutex_ };

	if( commandQueue_.empty() == true )
	{
		return nullptr;
	}

	UdpCommand* cmd = commandQueue_.front();
	commandQueue_.pop_front();

	return cmd;
}

void UdpCommandQueue::PushCommand( UdpCommand* cmd )
{
	if( cmd == nullptr )
		return;
	
	{
		std::unique_lock<std::mutex> lock { commandMutex_ };
		commandQueue_.push_back( cmd );
	}

	std::unique_lock<std::mutex> lock( queueLock_ );
	queueCond_.notify_one();
}
