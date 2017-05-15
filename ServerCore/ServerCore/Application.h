#pragma once
#include "NetworkModel.h"
#include "Accepter.h"

class Application
{
	Accepter accepter_;

public:
	Application();
	~Application();

	bool InitApplication( SERVER_MODEL serverModel );
	
	bool AddAcceptPort( int port );

	void StartServer();
};

