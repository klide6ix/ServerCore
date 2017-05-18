// TestClient.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "Socket.h"
#include "Application.h"

#define SERVER_PORT 1500

int main()
{
	Application::GetInstance();

	Socket socket;
	socket.CreateSocket();

	socket.Connect( "127.0.0.1", SERVER_PORT );
    
	return 0;
}

