#include "json/json.h"

#include "DatabaseThread.h"
#include "ODBCHandler.h"

DatabaseThread::DatabaseThread()
{
	odbcHandler_ = std::make_shared<ODBCHandler>();
}

DatabaseThread::~DatabaseThread()
{
}

bool DatabaseThread::Initialize( const char* connectString )
{
	if( odbcHandler_->Initialize() == false )
		return false;

	if( odbcHandler_->SetConnect( connectString ) == false )
		return false;

	return true;
}

void DatabaseThread::StartThread()
{
	isRunning_ = true;

	dbThread_ = std::make_shared<std::thread>( [&] () { Process(); } );
}

void DatabaseThread::StopThread()
{
	isRunning_ = false;
}


Json::Value* ServerEngine::AllocJson()
{
	return serverImpl_->jsonPool_.Alloc();
}

void ServerEngine::FreeJson( Json::Value* value )
{
	if( value == nullptr )
		return;

	serverImpl_->jsonPool_.Free( value );
}


void DatabaseThread::JoinThread()
{
	if( dbThread_ != nullptr && dbThread_->joinable() == true )
	{
		dbThread_->join();
	}
}

void DatabaseThread::Process()
{
	while( IsRunning() == true )
	{
		Command command;
		if( ServerEngine::GetInstance().PopQuery( command ) == false )
			continue;

		Json::Value* outValue = ServerEngine::GetInstance().AllocJson();
		odbcHandler_->ExecuteQuery( (const char*)command.cmdMessage_, outValue );
		ServerEngine::GetInstance().FreeQuery( command );

		Json::StyledWriter writer;
		std::string outputConfig = writer.write( *outValue );
		printf("%s\n", outputConfig.c_str() );

	}
}