#pragma once
#include <memory>
#include <mutex>

enum SERVER_MODEL
{
	MODEL_EPOLL,
	MODEL_IOCP,
	MODEL_SELECT,
};

class Accepter;
class SessionManager;

class Application
{
private:
	std::shared_ptr<Accepter> accepter_;
	std::shared_ptr<SessionManager> sessionManager_;

	static std::unique_ptr<Application> instance_;
	static std::once_flag onceFlag_;

	Application();
	Application(const Application& src) = delete;
	Application& operator=(const Application& rhs) = delete;

public:
	virtual ~Application();
	static Application& GetInstance();

	std::shared_ptr<SessionManager> getSessionManager() { return sessionManager_; }

public:
	bool InitApplication( SERVER_MODEL serverModel );	
	bool AddAcceptPort( int port );
	void StartServer();
};

