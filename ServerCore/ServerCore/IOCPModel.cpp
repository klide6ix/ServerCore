#include "Session.h"
#include "IOCPModel.h"


IOCPModel::IOCPModel()
{
}


IOCPModel::~IOCPModel()
{
}

bool IOCPModel::InitNetworkModel()
{
	iocpHandle_ = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );

	if( iocpHandle_ == NULL )
		return false;

	return true;
}

bool IOCPModel::AddSession( Session* newSession )
{
	HANDLE retHandle = CreateIoCompletionPort( (HANDLE)newSession->GetSocket() , iocpHandle_, (ULONG_PTR)newSession, 0 );
	if( retHandle != iocpHandle_ )
		return false;

	newSession->RecvPost();

	return true;
}

bool IOCPModel::RemoveSession( Session* session )
{
	return true;
}

void IOCPModel::SelectSession()
{
	DWORD dwBytesTransfer = 0;
	LPOVERLAPPED lpOverlapped = NULL;
	ULONG_PTR CompletionKey = NULL;

	BOOL ret = GetQueuedCompletionStatus( iocpHandle_, &dwBytesTransfer, &CompletionKey, &lpOverlapped, INFINITE );

	if( lpOverlapped == nullptr )
	{
		return;
	}
	else
	{
		Session* session = ( Session* )CompletionKey;
		if( session )
		{
			if( ( ret == FALSE && GetLastError() == ERROR_NETNAME_DELETED )	||	// 강제 종료
				( ret == TRUE && dwBytesTransfer == 0 ) )						// Graceful 종료
			{
				session->CleanUp();
			}
			else
			{
				OVERLAPPEDEX* overlapped = ( OVERLAPPEDEX* )lpOverlapped;

				switch( overlapped->dwFlags )
				{
				case ASYNCFLAG_RECEIVE:
					{
						session->RecvProcess( dwBytesTransfer );
					}
				break;
				}
			}
		}
	}
}