#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <vector>

#include "Command.h"
#include "EventTimer.h"

class IParser;
class Session;
class Socket;

class ServerApp;
class NetworkImplement;

enum ENUM_SESSION_EVENT
{
	SESSION_CLOSE,
	SESSION_RECV,
	SESSION_SEND,
};

struct SessionEvent
{
	ENUM_SESSION_EVENT event_ = SESSION_CLOSE;
	Session* session_ = nullptr;
	int recvSize_ = 0;
};


class NetworkCore
{
	static std::unique_ptr<NetworkCore>	 instance_;
	static std::once_flag				 onceFlag_;

	NetworkImplement* networkImpl_ = nullptr;

	NetworkCore();
	NetworkCore(const NetworkCore& src) = delete;
	NetworkCore& operator=(const NetworkCore& rhs) = delete;

	unsigned int networkThreadCount_ = 0;
	unsigned int workThreadCount_ = 0;

public:
	virtual ~NetworkCore();
	static NetworkCore& GetInstance();

	ServerApp* GetServerApp();

	void SetNetworkThreadCount( unsigned int count ) { networkThreadCount_ = count; }
	void SetWorkThreadCount( unsigned int count ) { workThreadCount_ = count; }

	bool InitializeEngine( ServerApp* application );
	bool InitializeParser( IParser* parser );
	bool InitializeAccepter();

	bool AddAcceptPort( int port );

	void SelectSession( std::vector<SessionEvent>& sessionList );

	void StartServer();
	void StopServer();

	Session* CreateSession();
	void AddSession( Session* newSession, int acceptPort );
	void CloseSession( Session* session );

	int ParsePacket( const char* src, int srcSize, Command* command );

	Command* AllocateCommand();
	void DeallocateCommand( Command* obj );

	// Command
	void PushCommand( Command* cmd );
	Command* PopCommand();

	void AddServerCommand( COMMAND_ID protocol, CommandFunction_t command );
	CommandFunction_t GetServerCommand( COMMAND_ID protocol );

	// Timer Message
	void PushTimerMessage( TIMER_ID id, int workCount, int workTime, void* object );
	bool PopTimerMessage( TimerObject*& timerObj );

	void AddTimerCommand( TIMER_ID protocol, TimerFunction_t command );
	TimerFunction_t GetTimerCommand( TIMER_ID protocol );

	void RecvRetryProcess( Session* session );
};

