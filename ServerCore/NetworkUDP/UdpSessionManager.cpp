#include <memory>
#include "UdpSessionManager.h"


UdpSessionManager::UdpSessionManager()
{
	sessionPool_.Init();
}


UdpSessionManager::~UdpSessionManager()
{
}

UdpSession* UdpSessionManager::CreateUdpSession()
{
	std::unique_lock<std::mutex> lock { mutex_ };

	UdpSession* newSession = sessionPool_.Alloc();

	if( newSession == nullptr )
		return nullptr;

	return newSession;
}

void UdpSessionManager::RestoreUdpSession( UdpSession* session )
{
	if( session == nullptr )
		return;

	std::unique_lock<std::mutex> lock { mutex_ };

	session->~UdpSession();
	sessionPool_.Free( session );
}