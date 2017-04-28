// TestServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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

