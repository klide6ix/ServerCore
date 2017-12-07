#include "NetworkUdp.h"
#include "UdpSessionManager.h"
#include "UdpNetworkThread.h"
#include "UdpWorkThread.h"

#include "../Utility/LogWriter.h"
#include "../Utility/ObjectPool.h"
#include "../Utility/Parser.h"
#include "../Utility/BoostObjectPoolWrapper.h"

class UdpImplement
{
public:

	std::map< COMMAND_ID, UdpCommandFunction_t >	serverCommand_;

	boost::asio::io_service							udpIoService_;
	std::shared_ptr<boost::asio::io_service::work>	ioWork_ = nullptr;

	std::shared_ptr<ServerApp>						serverApp_ = nullptr;
	std::shared_ptr<UdpSessionManager>				sessionManager_ = nullptr;

	std::shared_ptr<UdpCommandQueue>				workQueue_ = nullptr;

	ObjectPool<UdpCommand>							commandPool_;

	std::vector<std::shared_ptr<UdpNetworkThread>>		networkThreads_;
	std::vector<std::shared_ptr<UdpWorkThread>>		workThreads_;
};

NetworkUdp::NetworkUdp()
{
}

NetworkUdp::~NetworkUdp()
{
	if( udpImpl_ != nullptr )
		delete udpImpl_;
}

std::unique_ptr<NetworkUdp> NetworkUdp::instance_;
std::once_flag NetworkUdp::onceFlag_;

NetworkUdp& NetworkUdp::GetInstance()
{
	std::call_once( onceFlag_, [] 
	{
		instance_.reset(new NetworkUdp);
	});

	return *instance_;
}


bool NetworkUdp::InitializeEngine()
{
	try
	{
		udpImpl_ = new UdpImplement();

		udpImpl_->workQueue_ = std::make_shared<UdpCommandQueue>();

		udpImpl_->sessionManager_ = std::make_shared<UdpSessionManager>();
		udpImpl_->ioWork_ = std::make_shared<boost::asio::io_service::work>(udpImpl_->udpIoService_);

		for( unsigned int i = 0; i < std::thread::hardware_concurrency(); ++i )
		{
			udpImpl_->workThreads_.push_back( std::make_shared<UdpWorkThread>() );
			udpImpl_->networkThreads_.push_back( std::make_shared<UdpNetworkThread>() );
		}
	}
	catch( std::bad_alloc& )
	{
		return false;
	}

	udpImpl_->commandPool_.SetMaxPoolSize( 32 * 10 );
	if( udpImpl_->commandPool_.Init() == false )
		return false;

	for( auto thread : udpImpl_->networkThreads_ )
	{
		thread->StartThread();
	}

	for( auto thread : udpImpl_->workThreads_ )
	{
		thread->StartThread();
	}

	return true;
}

UdpSession* NetworkUdp::CreateUdpSession()
{
	return udpImpl_->sessionManager_->CreateUdpSession();
}

void NetworkUdp::CloseUdpSession( UdpSession* session )
{
	if( session == nullptr )
		return;

	//udpImpl_->serverApp_->OnClose( session );
	udpImpl_->sessionManager_->RestoreUdpSession( session );

	session->Close();
}

bool NetworkUdp::IsCompleteDatagram( const char* src, int srcSize )
{
	return true;
}

int NetworkUdp::ParseDatagram( const char* src, int srcSize, UdpCommand* command )
{
	if( command == nullptr )
		return 0;

	command->cmdBuffer_.InitializeBuffer( src, srcSize );

	return srcSize;
}

UdpCommand* NetworkUdp::AllocateUdpCommand()
{
	return udpImpl_->commandPool_.Alloc();
}

void NetworkUdp::DeallocateUdpCommand( UdpCommand* obj )
{
	udpImpl_->commandPool_.Free( obj );
}

void NetworkUdp::PushUdpCommand( UdpCommand* cmd )
{
	udpImpl_->workQueue_->PushCommand( cmd );
}

UdpCommand* NetworkUdp::PopUdpCommand()
{
	return udpImpl_->workQueue_->PopCommand();
}

void NetworkUdp::AddServerCommand( COMMAND_ID protocol, UdpCommandFunction_t command )
{
	if( udpImpl_->serverCommand_.find( protocol ) == udpImpl_->serverCommand_.end() )
	{
		udpImpl_->serverCommand_.insert( std::pair< COMMAND_ID, UdpCommandFunction_t >( protocol, command ) );
	}
	else
	{
		udpImpl_->serverCommand_[protocol] = command;
	}
}

UdpCommandFunction_t NetworkUdp::GetServerCommand( COMMAND_ID protocol )
{
	if( udpImpl_->serverCommand_.find( protocol ) == udpImpl_->serverCommand_.end() )
	{
		return nullptr;
	}

	return udpImpl_->serverCommand_[protocol];
}

void NetworkUdp::StartUdp()
{
	for( auto thread : udpImpl_->networkThreads_ )
	{
		if( thread != nullptr )
			thread->JoinThread();
	}
}

void NetworkUdp::StopUdp()
{
	udpImpl_->ioWork_->get_io_service().stop();

	for( auto thread : udpImpl_->networkThreads_ )
	{
		if( thread != nullptr )
			thread->StopThread();
	}

	for( auto thread : udpImpl_->workThreads_ )
	{
		if( thread != nullptr )
			thread->StopThread();
	}

}

boost::asio::io_service& NetworkUdp::GetUdpIoService()
{
	return udpImpl_->udpIoService_;
}