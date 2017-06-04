#pragma once

#define MESSAGE_BUFFER_SIZE_MAX	24000

class MessageObject
{
public:
	char messageBuffer_[MESSAGE_BUFFER_SIZE_MAX] = {0};
	int  messageBufferSize_ = 0;

	MessageObject() {}
	virtual ~MessageObject()
	{
		memset( messageBuffer_, 0, MESSAGE_BUFFER_SIZE_MAX );
		messageBufferSize_ = 0;
	}
};