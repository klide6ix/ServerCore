#pragma once

#include <list>
#include <mutex>
#include <condition_variable>

#include "../Utility/BufferSerializer.h"

using COMMAND_ID = unsigned int;

class Session;
class Command
{
public:
	COMMAND_ID		 cmdID_ = 0;
	Session*		 cmdSession_ = nullptr;
	BufferSerializer cmdBuffer_;

	Command() {}
	Command( COMMAND_ID id, Session* session, const char* message, size_t messageLen ) : cmdID_(id), cmdSession_(session), cmdBuffer_( message, messageLen ) {}
};

using CommandFunction_t = unsigned int (*)( Command* command );

class CommandQueue
{
	std::mutex					queueLock_;
	std::condition_variable		queueCond_;

	std::list<Command*>			commandQueue_;
	std::mutex					commandMutex_;

	bool _Empty();

public:
	CommandQueue();
	virtual ~CommandQueue();

	Command* PopCommand();
	void PushCommand( Command* obj );
};

