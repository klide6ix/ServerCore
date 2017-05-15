// TestServer.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Application.h"
#include "Accepter.h"

#define SERVER_PORT 1500

int main()
{
	Application application;

	application.InitApplication( MODEL_IOCP );
	application.AddAcceptPort( SERVER_PORT );
	
	application.StartServer();

    return 0;
}

