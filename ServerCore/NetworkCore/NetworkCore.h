#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <vector>

#include "Command.h"

class IParser;
class Session;
class Packet;
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

public:
	virtual ~NetworkCore();
	static NetworkCore& GetInstance();

	ServerApp* GetServerApp();

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

	bool EncodePacket( const char* src, int srcSize, char* dest, int& destSize );
	bool DecodePacket( const char* src, int srcSize, char* dest, int& destSize );

	Packet* AllocatePacket();
	void FreePacket( Packet* obj );

	void PushCommand( Command& cmd );
	bool PopCommand( Command& cmd );

	bool InitializeDatabase( const char* connectString );
	void PushQuery( const char* query, size_t len );
	void StartDatabase();

	void AddServerCommand( COMMAND_ID protocol, CommandFunction_t command );
	CommandFunction_t GetServerCommand( COMMAND_ID protocol );

	void RecvRetryProcess( Session* session );
};

