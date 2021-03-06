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

void SelectModel::SelectSession( std::vector<SessionEvent>& sessionList )
{
	fd_set temps = fdReads_;

	int nfdmax = 0;

#ifdef WIN32
#else
	nfdmax = fdMax_ + 1;
#endif

	if( select( nfdmax, &fdReads_, nullptr, nullptr, nullptr ) == SOCKET_ERROR )
	{
		int err = GetLastError();
		return;
	}

	for( auto itr : fdList_ )
	{
		if( FD_ISSET( itr->GetSocket(), &temps ) )
		{
			int recvSize = itr->RecvPacket();

			if( recvSize < 0 )
			{
				itr->RecvBufferConsume( recvSize );
				sessionList.push_back( { SESSION_CLOSE, itr, 0 } );
				continue;
			}

			sessionList.push_back( { SESSION_RECV, itr, recvSize } );
		}
	}
}

void SelectModel::StopNetworkModel()
{
}