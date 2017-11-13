#include "Command.h"

CommandQueue::CommandQueue()
{
}

CommandQueue::~CommandQueue()
{
	std::unique_lock<std::mutex> lock { commandMutex_ };

	for( auto itr : commandQueue_ )
	{
		if( itr != nullptr )
			delete itr;
	}

	commandQueue_.clear();
}

bool CommandQueue::_Empty()
{
	std::unique_lock<std::mutex> lock { commandMutex_ };
	return commandQueue_.empty();
}

Command* CommandQueue::PopCommand()
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

	Command* cmd = commandQueue_.front();
	commandQueue_.pop_front();

	return cmd;
}

void CommandQueue::PushCommand( Command* cmd )
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
