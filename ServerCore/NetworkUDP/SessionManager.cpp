#include <memory>
#include "SessionManager.h"


SessionManager::SessionManager()
{
	sessionPool_.Init();
}


SessionManager::~SessionManager()
{
}

UdpSession* SessionManager::CreateUdpSession()
{
	std::unique_lock<std::mutex> lock { mutex_ };

	UdpSession* newSession = sessionPool_.Alloc();

	if( newSession == nullptr )
		return nullptr;

	return newSession;
}

void SessionManager::RestoreUdpSession( UdpSession* session )
{
	if( session == nullptr )
		return;

	std::unique_lock<std::mutex> lock { mutex_ };

	session->~UdpSession();
	sessionPool_.Free( session );
}