#include <stdio.h>

#include "Application.h"



Application::Application()
{
}


Application::~Application()
{
}

void Application::InitApplication( SERVER_MODEL serverModel )
{
	printf("ServerModel : %d", serverModel );
}