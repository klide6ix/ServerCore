#pragma once

#include <functional>
#include <mutex>

class DatabaseImplement;
class DatabaseCore
{
	static std::unique_ptr<DatabaseCore> instance_;
	static std::once_flag				 onceFlag_;

	DatabaseImplement* databaseImpl_ = nullptr;

	DatabaseCore();
	DatabaseCore(const DatabaseCore& src) = delete;
	DatabaseCore& operator=(const DatabaseCore& rhs) = delete;

public:
	virtual ~DatabaseCore();
	static DatabaseCore* GetInstance();

	bool InitDatabaseCore();
};

