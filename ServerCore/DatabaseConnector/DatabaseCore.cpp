#include <vector>

#include "DatabaseCore.h"

#include "ODBCHandler.h"
#include "DatabaseThread.h"

class DatabaseImplement
{
public:

	std::shared_ptr<SessionManager>			sessionManager_ = nullptr;
	std::shared_ptr<Accepter>				accepter_ = nullptr;
	std::shared_ptr<NetworkModel>			networkModel_ = nullptr;
};

DatabaseCore::DatabaseCore()
{
}


DatabaseCore::~DatabaseCore()
{
	if( databaseImpl_ != nullptr )
		delete databaseImpl_;
}

std::unique_ptr<DatabaseCore> DatabaseCore::instance_;
std::once_flag DatabaseCore::onceFlag_;

DatabaseCore* DatabaseCore::GetInstance()
{
	std::call_once( onceFlag_, [] 
	{
		instance_.reset(new DatabaseCore);
	});

	return instance_.get();
}

bool DatabaseCore::InitDatabaseCore()
{
	try
	{
		databaseImpl_ = new DatabaseImplement();

		databaseImpl_->sessionManager_ = std::make_shared<SessionManager>();

#ifdef _WIN32
			databaseImpl_->networkModel_ = std::make_shared<IOCPModel>();
#else
			databaseImpl_->networkModel_ = std::make_shared<EpollModel>();
#endif

	}
	catch( std::bad_alloc& )
	{
		return false;
	}

	if( databaseImpl_->networkModel_->InitNetworkModel() == false )
		return false;


	return true;
}
