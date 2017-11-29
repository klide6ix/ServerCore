#pragma once

#include <mutex>

#include <boost/asio.hpp>

#include "Command.h"

class IParser;
class UdpSession;

class ServerApp;
class NetworkImplement;
class AsioModel;

class NetworkUdp
{
	static std::unique_ptr<NetworkUdp>	 instance_;
	static std::once_flag				 onceFlag_;

	NetworkImplement* networkImpl_ = nullptr;

	NetworkUdp();
	NetworkUdp(const NetworkUdp& src) = delete;
	NetworkUdp& operator=(const NetworkUdp& rhs) = delete;

public:
	virtual ~NetworkUdp();
	static NetworkUdp& GetInstance();

	bool InitializeEngine( ServerApp* application );

	void StartUdp();
	void StopUdp();

	UdpSession* CreateUdpSession();
	void CloseUdpSession( UdpSession* session );

	bool IsCompletePacket( const char* src, int srcSize );
	int ParseBuffer( const char* src, int srcSize, Command* command );

	Command* AllocateCommand();
	void DeallocateCommand( Command* obj );

	// Command
	void PushCommand( Command* cmd );
	Command* PopCommand();

	void AddServerCommand( COMMAND_ID protocol, CommandFunction_t command );
	CommandFunction_t GetServerCommand( COMMAND_ID protocol );

	boost::asio::io_service& GetIoService();
};

