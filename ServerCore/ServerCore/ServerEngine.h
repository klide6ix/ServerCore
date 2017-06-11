#pragma once
#include <memory>
#include <mutex>
#include "ServerEngineDef.h"

class Socket;
class IParser;
class Session;
class Packet;

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

	ServerApp* GetServerApp();
	
	void StartServer();

	bool AddAcceptPort( int port );
	void StartAccepter();

	Session* CreateSession( Socket& socket );

	void AddSession( Session* newSession, int acceptPort );
	void CloseSession( Session* session );
	void SelectSession();
	
	bool EncodePacket( Packet* packet, char* dest, int& destSize );
	bool DecodePacket( const char* src, int srcSize, Packet* packet );

	Packet*	AllocPacket();
	void	FreePacket( Packet* obj );

	void	PushPacket( Packet* obj );
	Packet*	PopPacket();

	void				AddServerCommand( PROTOCOL_TYPE protocol, CommandFunction_t command );
	CommandFunction_t	GetServerCommand( PROTOCOL_TYPE protocol );
};

