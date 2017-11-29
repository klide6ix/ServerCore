#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <vector>

#include <boost/asio.hpp>

#include "Command.h"
#include "EventTimer.h"

class IParser;
class Session;

class ServerApp;
class NetworkImplement;
class AsioModel;

class NetworkCore
{
	static std::unique_ptr<NetworkCore>	 instance_;
	static std::once_flag				 onceFlag_;

	int networkThreadCount_ = std::thread::hardware_concurrency();
	int workThreadCount_ = std::thread::hardware_concurrency();

	NetworkImplement* networkImpl_ = nullptr;

	NetworkCore();
	NetworkCore(const NetworkCore& src) = delete;
	NetworkCore& operator=(const NetworkCore& rhs) = delete;

public:
	virtual ~NetworkCore();
	static NetworkCore& GetInstance();

	ServerApp* GetServerApp();

	void MakeDaemon( bool debug, char* ServiceName );

	void SetNetworkThreadCount( int count ) { networkThreadCount_ = count; }
	void SetWorkThreadCount( int count ) { workThreadCount_ = count; }

	bool InitializeEngine( ServerApp* application );
	bool InitializeParser( IParser* parser );
	bool InitializeAccepter();

	bool AddAcceptPort( int port );

	void StartServer();
	void StopServer();

	Session* CreateSession();
	void AddSession( Session* newSession, int acceptPort );
	void CloseSession( Session* session );
	void ShutdownSession( Session* session );

	bool IsCompletePacket( const char* src, int srcSize );
	int ParseBuffer( const char* src, int srcSize, Command* command );

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

	boost::asio::io_service& GetIoService();
};
