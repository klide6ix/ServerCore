// TestServer.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Application.h"

#define SERVER_PORT 1500

int main()
{
	Application application;
	application.InitApplication( MODEL_IOCP );
	application.Listen( SERVER_PORT );

    return 0;
}

