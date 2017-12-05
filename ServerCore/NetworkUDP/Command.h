#pragma once

#include <list>
#include <mutex>
#include <condition_variable>

#include "../Utility/BufferSerializer.h"

using COMMAND_ID = unsigned int;

class UdpSession;
class UdpCommand
{
public:
	COMMAND_ID		 cmdID_ = 0;
	UdpSession*		 cmdSession_ = nullptr;
	BufferSerializer cmdBuffer_;

	UdpCommand()
	{
		cmdID_ = 0;
		cmdSession_ = nullptr;
		cmdBuffer_.ResetBuffer();
	}

	~UdpCommand()
	{
		cmdID_ = 0;
		cmdSession_ = nullptr;
		cmdBuffer_.ResetBuffer();
	}
};

using UdpCommandFunction_t = unsigned int (*)( UdpCommand* command );

class UdpCommandQueue
{
	std::mutex					queueLock_;
	std::condition_variable		queueCond_;

	std::list<UdpCommand*>		commandQueue_;
	std::mutex					commandMutex_;

	bool _Empty();

public:
	UdpCommandQueue();
	virtual ~UdpCommandQueue();

	UdpCommand* PopCommand();
	void PushCommand( UdpCommand* obj );
};
