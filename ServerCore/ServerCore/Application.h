#pragma once

enum SERVER_MODEL
{
	MODEL_EPOLL,
	MODEL_IOCP,
	MODEL_SELECT,
};

class Application
{
public:
	Application();
	~Application();

	void InitApplication( SERVER_MODEL serverModel );
};

