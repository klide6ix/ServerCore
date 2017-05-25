#pragma once
#include <memory>
#include <mutex>
#include <vector>

enum SERVER_MODEL
{
	MODEL_EPOLL,
	MODEL_IOCP,
	MODEL_SELECT,
};

class Session;
class SessionManager;
class NetworkModel;

class Accepter;
class WorkThread;
class NetworkThread;

class Application
{
private:
	std::shared_ptr<SessionManager>		sessionManager_;
	std::shared_ptr<NetworkModel>		networkModel_;

	std::shared_ptr<Accepter>			accepter_;
	std::shared_ptr<WorkThread>			workThread_;
	std::shared_ptr<NetworkThread>		networkThread_;

	static std::unique_ptr<Application> instance_;
	static std::once_flag				onceFlag_;

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
	
	void AddSession( Session* newSession );
	void RemoveSession( Session* session );
	void SelectSession();
};

