// TestClient.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

#include "Application.h"
#include "Socket.h"

#define SERVER_PORT 1500

int main()
{
	Application application;

	Socket socket;
	socket.CreateSocket();

	socket.Connect( "127.0.0.1", SERVER_PORT );
    
	return 0;
}

