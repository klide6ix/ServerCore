#pragma once
#include <memory>
#include <mutex>

enum SERVER_MODEL
{
	MODEL_EPOLL,
	MODEL_IOCP,
	MODEL_SELECT,
};

class Session;
class SessionManager;
class NetworkModel;
class IParser;

class Accepter;
class WorkThread;
class NetworkThread;
class MessageObject;
class WorkQueue;

class ServerEngine
{
private:

	static std::unique_ptr<ServerEngine> instance_;
	static std::once_flag				 onceFlag_;

	std::shared_ptr<SessionManager>		sessionManager_;
	std::shared_ptr<NetworkModel>		networkModel_;
	std::shared_ptr<IParser>			parser_;

	std::shared_ptr<Accepter>			accepter_;
	std::shared_ptr<WorkThread>			workThread_;
	std::shared_ptr<NetworkThread>		networkThread_;

	std::shared_ptr<WorkQueue>			workQueue_;

	ServerEngine();
	ServerEngine(const ServerEngine& src) = delete;
	ServerEngine& operator=(const ServerEngine& rhs) = delete;

public:
	virtual ~ServerEngine();
	static ServerEngine& GetInstance();

	std::shared_ptr<SessionManager> getSessionManager() { return sessionManager_; }

public:
	bool InitializeEngine( SERVER_MODEL serverModel );
	bool InitializeAccepter();
	
	void StartServer();

	bool AddAcceptPort( int port );
	void StartAccepter();
	
	void AddSession( Session* newSession );
	void CloseSession( Session* session );
	void SelectSession();

	bool EncodePacket( const char* src, int srcSize, char* dest, int& destSize );
	bool DecodePacket( const char* src, int srcSize, char* dest, int& destSize );

	MessageObject*	GetMessageObject();
	void			ReturnMessageObject( MessageObject* obj );

	void			PushMessageObject( MessageObject* obj );
	MessageObject*	PopMessageObject();

};

