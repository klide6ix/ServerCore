#pragma once

enum SERVER_MODEL
{
	MODEL_EPOLL,
	MODEL_IOCP,
	MODEL_SELECT,
};

class NetworkModel
{
public:
	NetworkModel();
	virtual ~NetworkModel();
};

