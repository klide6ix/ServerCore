#pragma once

#include <list>
#include <mutex>
#include <condition_variable>

using COMMAND_ID = unsigned int;

class Session;
class Command
{
public:
	COMMAND_ID		cmdID_ = 0;
	void*			cmdMessage_ = nullptr;
	Session*		cmdSession_ = nullptr;

	Command() {}
	Command( COMMAND_ID id, void* msg, Session* session ) : cmdID_(id), cmdMessage_(msg), cmdSession_(session) {}
};

using CommandFunction_t = unsigned int (*)( Command& command );

class CommandQueue
{
	std::mutex					queueLock_;
	std::condition_variable		queueCond_;

	std::list<Command>			commandQueue_;
	std::mutex					commandMutex_;

	bool _Empty();

public:
	CommandQueue();
	virtual ~CommandQueue();

	bool PopCommand( Command& obj );
	void PushCommand( Command& obj );
};

