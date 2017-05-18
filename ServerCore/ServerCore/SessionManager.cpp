#include <memory>
#include "SessionManager.h"


SessionManager::SessionManager()
{
	sessionPool_.Init();
}


SessionManager::~SessionManager()
{
}

std::shared_ptr<Session> SessionManager::CreateNewSession( SOCKET socket )
{
	printf("CreateNewSession\n");
	std::shared_ptr<Session> newSession = sessionPool_.Alloc();

	if( newSession == nullptr )
		return nullptr;

	newSession->SetSocket( socket );
	return newSession;
}