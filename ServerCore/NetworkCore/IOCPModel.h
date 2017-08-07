#pragma once
#include "NetworkModel.h"

#include <WinSock2.h>
#include <list>

#include "Session.h"

class IOCPModel : public NetworkModel
{
	OVERLAPPEDEX			overlappedRetry_;
	HANDLE					iocpHandle_;
	std::list<Session*>		sessionList_;

public:
	IOCPModel();
	virtual ~IOCPModel();

	virtual bool InitNetworkModel();

	virtual bool AddSession( Session* newSession );
	virtual bool RemoveSession( Session* newSession );

	virtual void SelectSession( std::vector<SessionEvent>& sessionList );
	virtual void StopNetworkModel();

	virtual int RecvRetry( Session* session );
};

