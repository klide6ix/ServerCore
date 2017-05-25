#include "SelectModel.h"
#include "Session.h"


SelectModel::SelectModel()
{
}


SelectModel::~SelectModel()
{
}

bool SelectModel::InitNetworkModel()
{
	FD_ZERO( &fdReads_ );
	fdMax_ = 0;

	return true;
}

bool SelectModel::AddSession( Session* newSession )
{
	if( newSession == nullptr )
		return false;

	FD_SET( newSession->GetSocket(), &fdReads_ );

	if( fdMax_ < (int)newSession->GetSocket() )
		fdMax_ = (int)newSession->GetSocket();

	fdList_.push_back( newSession );

	return true;
}

bool SelectModel::RemoveSession( Session* newSession )
{
	if( newSession == nullptr )
		return false;

	FD_CLR( newSession->GetSocket(), &fdReads_ );

	for( auto itr = fdList_.begin(); itr != fdList_.end(); ++itr )
	{
		if( (*itr) == newSession )
		{
			fdList_.erase( itr );
			break;
		}
	}

	return true;
}

void SelectModel::SelectSession()
{
	fd_set temps = fdReads_;

	int nfds = 0;
	int nfdmax = 0;

#ifdef WIN32
	nfds = (int)fdReads_.fd_count;
#else
	nfdmax = fdMax_ + 1;
	nfds = FD_SETSIZE;
#endif

	if( select( nfdmax, &temps, 0, 0, 0 ) == SOCKET_ERROR )
	{
		return;
	}

	for( auto itr : fdList_ )
	{
		if( FD_ISSET( itr->GetSocket(), &temps ) )
		{
			itr->RecvPacket();
		}
	}
}