#pragma once
#include <memory>
#include <mutex>

enum SERVER_MODEL
{
	MODEL_EPOLL,
	MODEL_IOCP,
	MODEL_SELECT,
};

class Socket;
class IParser;
class Session;
class MessageObject;

class ServerApp;
class ServerImplement;

class ServerEngine
{
private:

	static std::unique_ptr<ServerEngine> instance_;
	static std::once_flag				 onceFlag_;

	ServerImplement* serverImpl_ = nullptr;

	ServerEngine();
	ServerEngine(const ServerEngine& src) = delete;
	ServerEngine& operator=(const ServerEngine& rhs) = delete;

public:
	virtual ~ServerEngine();
	static ServerEngine& GetInstance();

	bool InitializeEngine( SERVER_MODEL serverModel );
	bool InitializeParser( IParser* parser );
	bool InitializeApplication( ServerApp* application );
	bool InitializeAccepter();
	
	void StartServer();

	bool AddAcceptPort( int port );
	void StartAccepter();

	Session* CreateSession( Socket& socket );

	void AddSession( Session* newSession, int acceptPort );
	void CloseSession( Session* session );
	void SelectSession();
	
	bool EncodePacket( const char* src, int srcSize, char* dest, int& destSize );
	bool DecodePacket( const char* src, int srcSize, char* dest, int& destSize );

	MessageObject*	GetMessageObject();
	void			ReturnMessageObject( MessageObject* obj );

	void			PushMessageObject( MessageObject* obj );
	MessageObject*	PopMessageObject();
};

