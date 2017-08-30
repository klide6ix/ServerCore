#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <vector>

#include <boost/asio.hpp>

#include "Command.h"

class IParser;
class Session;
class Packet;
class Socket;

class ServerApp;
class NetworkImplement;
class AsioModel;

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

	void StartServer();
	void StopServer();

	Session* CreateSession();
	void AddSession( Session* newSession, int acceptPort );
	void CloseSession( Session* session );
	void ShutdownSession( Session* session );

	bool EncodePacket( const char* src, int srcSize, char* dest, int& destSize );
	bool DecodePacket( const char* src, int srcSize, char* dest, int& destSize );

	Packet* AllocatePacket();
	void FreePacket( Packet* obj );

	void PushCommand( Command& cmd );
	bool PopCommand( Command& cmd );

	void AddServerCommand( COMMAND_ID protocol, CommandFunction_t command );
	CommandFunction_t GetServerCommand( COMMAND_ID protocol );

	boost::asio::io_service& GetIoService();
};

