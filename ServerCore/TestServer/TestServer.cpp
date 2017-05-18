// TestServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "NetworkModel.h"
#include "Accepter.h"
#include "Application.h"

#define SERVER_PORT 1500

int main()
{
	Application::GetInstance().InitApplication( MODEL_IOCP );
	Application::GetInstance().AddAcceptPort( SERVER_PORT );
	
	Application::GetInstance().StartServer();

    return 0;
}

