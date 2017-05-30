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

class ServerEngine
{
private:
	std::shared_ptr<SessionManager>		sessionManager_;
	std::shared_ptr<NetworkModel>		networkModel_;

	std::shared_ptr<Accepter>			accepter_;
	std::shared_ptr<WorkThread>			workThread_;
	std::shared_ptr<NetworkThread>		networkThread_;

	static std::unique_ptr<ServerEngine> instance_;
	static std::once_flag				onceFlag_;

	ServerEngine();
	ServerEngine(const ServerEngine& src) = delete;
	ServerEngine& operator=(const ServerEngine& rhs) = delete;

public:
	virtual ~ServerEngine();
	static ServerEngine& GetInstance();

	std::shared_ptr<SessionManager> getSessionManager() { return sessionManager_; }

public:
	bool InitApplication( SERVER_MODEL serverModel );	
	bool AddAcceptPort( int port );
	void StartServer();
	void StartClient();
	
	void AddSession( Session* newSession );
	void RemoveSession( Session* session );
	void SelectSession();
};

