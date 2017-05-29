// TestServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "NetworkModel.h"
#include "Accepter.h"
#include "ServerEngine.h"

#define SERVER_PORT 1500

int main()
{
	ServerEngine::GetInstance().InitApplication( MODEL_IOCP );
	ServerEngine::GetInstance().AddAcceptPort( SERVER_PORT );
	
	ServerEngine::GetInstance().StartServer();

    return 0;
}

