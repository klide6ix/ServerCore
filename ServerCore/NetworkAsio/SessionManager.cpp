#include <memory>
#include "SessionManager.h"


SessionManager::SessionManager()
{
	sessionPool_.Init();
}


SessionManager::~SessionManager()
{
}

Session* SessionManager::CreateSession()
{
	std::unique_lock<std::mutex> lock { mutex_ };

	Session* newSession = sessionPool_.Alloc();

	if( newSession == nullptr )
		return nullptr;

	return newSession;
}

void SessionManager::RestoreSession( Session* session )
{
	if( session == nullptr )
		return;

	std::unique_lock<std::mutex> lock { mutex_ };

	session->~Session();
	sessionPool_.Free( session );
}