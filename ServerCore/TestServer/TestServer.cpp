// TestServer.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

#include "NetworkModel.h"
#include "Accepter.h"
#include "ServerEngine.h"

#define SERVER_PORT 1500

int main()
{
	ServerEngine::GetInstance().InitializeEngine( MODEL_IOCP );
	
	ServerEngine::GetInstance().InitializeAccepter();
	ServerEngine::GetInstance().AddAcceptPort( SERVER_PORT );
	
	ServerEngine::GetInstance().StartAccepter();
	ServerEngine::GetInstance().StartServer();

    return 0;
}

