#pragma once

using COMMAND_ID = unsigned int;

class Command
{
public:
	COMMAND_ID		cmdID_ = 0;
	void*			cmdMessage_ = nullptr;
	void*			cmdObject_ = nullptr;

	Command() {}
	Command( COMMAND_ID id, void* msg, void* obj ) : cmdID_(id), cmdMessage_(msg), cmdObject_(obj) {}
};

using CommandFunction_t = unsigned int (*)( Command& command );


