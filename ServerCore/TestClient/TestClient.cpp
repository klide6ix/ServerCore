// TestClient.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

#include "Socket.h"
#include "ServerEngine.h"

#define SERVER_PORT 1500

int main()
{
	ServerEngine::GetInstance();

	Socket socket;
	socket.CreateSocket();

	socket.Connect( "127.0.0.1", SERVER_PORT );
    
	return 0;
}

