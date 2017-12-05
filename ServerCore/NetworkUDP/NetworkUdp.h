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

	bool InitializeEngine();

	void StartUdp();
	void StopUdp();

	UdpSession* CreateUdpSession();
	void CloseUdpSession( UdpSession* session );

	bool IsCompleteDatagram( const char* src, int srcSize );
	int ParseDatagram( const char* src, int srcSize, UdpCommand* command );

	UdpCommand* AllocateUdpCommand();
	void DeallocateUdpCommand( UdpCommand* obj );

	// UdpCommand
	void PushUdpCommand( UdpCommand* cmd );
	UdpCommand* PopUdpCommand();

	void AddServerCommand( COMMAND_ID protocol, UdpCommandFunction_t command );
	UdpCommandFunction_t GetServerCommand( COMMAND_ID protocol );

	boost::asio::io_service& GetIoService();
};

