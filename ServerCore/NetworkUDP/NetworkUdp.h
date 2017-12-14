#pragma once

#include <mutex>

#include <boost/asio.hpp>

#include "Command.h"

class IParser;
class UdpSession;

class ServerApp;
class UdpImplement;
class AsioModel;

class NetworkUdp
{
	static std::unique_ptr<NetworkUdp>	 instance_;
	static std::once_flag				 onceFlag_;

	UdpImplement* udpImpl_ = nullptr;

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

	UdpCommand* AllocateUdpCommand();
	void DeallocateUdpCommand( UdpCommand* obj );

	// UdpCommand
	void PushUdpCommand( UdpCommand* cmd );
	UdpCommand* PopUdpCommand();

	void SetUdpCommand( UdpCommandFunction_t command );
	UdpCommandFunction_t GetUdpCommand();

	boost::asio::io_service& GetUdpIoService();
};

