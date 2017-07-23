#pragma once
#include <memory>
#include <mutex>
#include "Command.h"

#include "json/json.h"

enum SERVER_MODEL
{
	MODEL_EPOLL,
	MODEL_IOCP,
	MODEL_SELECT,
};


class Socket;
class IParser;
class Session;
class Packet;

class ServerApp;
class ServerImplement;

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
	
	ServerApp* GetServerApp();

	void StartServer();
	
	bool InitializeEngine( SERVER_MODEL serverModel );
	bool InitializeParser( IParser* parser );
	bool InitializeApplication( ServerApp* application );

	bool InitializeAccepter();
	bool AddAcceptPort( int port );
	void StartAccepter();

	bool InitializeDatabase( const char* connectStr );
	bool AddDatabaseConnection();
	void StartDatabase();

	Session* CreateSession( Socket& socket );

	void AddSession( Session* newSession, int acceptPort );
	void CloseSession( Session* session );
	void SelectSession( std::vector<SessionEvent>& sessionList );
	void StopNetworkModel();
	
	bool EncodePacket( const char* src, int srcSize, char* dest, int& destSize );
	bool DecodePacket( const char* src, int srcSize, char* dest, int& destSize );

	Packet* AllocatePacket();
	void FreePacket( Packet* obj );

	char* AllocateBuffer();
	void FreeBuffer( char* buffer );

	void PushCommand( Command& cmd );
	bool PopCommand( Command& cmd );

	void PushQuery( char* query );
	bool PopQuery( Command& cmd );
	void FreeQuery( Command& cmd );

	Json::Value* AllocJson();
	void		 FreeJson( Json::Value* value );

	void AddServerCommand( COMMAND_ID protocol, CommandFunction_t command );
	CommandFunction_t GetServerCommand( COMMAND_ID protocol );
};

