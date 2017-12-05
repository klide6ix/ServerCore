#include "NetworkUdp.h"
#include "UdpSessionManager.h"
#include "NetworkThread.h"
#include "WorkThread.h"

#include "../Utility/LogWriter.h"
#include "../Utility/ObjectPool.h"
#include "../Utility/Parser.h"
#include "../Utility/BoostObjectPoolWrapper.h"

class NetworkImplement
{
public:

	std::map< COMMAND_ID, UdpCommandFunction_t >	serverCommand_;

	boost::asio::io_service							ioService_;
	std::shared_ptr<boost::asio::io_service::work>	ioWork_ = nullptr;

	std::shared_ptr<IParser>						parser_ = nullptr;
	std::shared_ptr<ServerApp>						serverApp_ = nullptr;
	std::shared_ptr<UdpSessionManager>				sessionManager_ = nullptr;

	std::shared_ptr<UdpCommandQueue>				workQueue_ = nullptr;

	ObjectPool<UdpCommand>							commandPool_;

	std::vector<std::shared_ptr<NetworkThread>>		networkThreads_;
	std::vector<std::shared_ptr<WorkThread>>		workThreads_;
};

NetworkUdp::NetworkUdp()
{
}

NetworkUdp::~NetworkUdp()
{
	if( networkImpl_ != nullptr )
		delete networkImpl_;
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
		networkImpl_ = new NetworkImplement();

		//networkImpl_->serverApp_.reset( application );

		networkImpl_->workQueue_ = std::make_shared<UdpCommandQueue>();

		networkImpl_->sessionManager_ = std::make_shared<UdpSessionManager>();
		networkImpl_->ioWork_ = std::make_shared<boost::asio::io_service::work>(networkImpl_->ioService_);

		for( unsigned int i = 0; i < std::thread::hardware_concurrency(); ++i )
		{
			networkImpl_->workThreads_.push_back( std::make_shared<WorkThread>() );
			networkImpl_->networkThreads_.push_back( std::make_shared<NetworkThread>() );
		}
	}
	catch( std::bad_alloc& )
	{
		return false;
	}

	networkImpl_->commandPool_.SetMaxPoolSize( 32 * 10 );
	if( networkImpl_->commandPool_.Init() == false )
		return false;

	for( auto thread : networkImpl_->networkThreads_ )
	{
		thread->StartThread();
	}

	for( auto thread : networkImpl_->workThreads_ )
	{
		thread->StartThread();
	}

	return true;
}

UdpSession* NetworkUdp::CreateUdpSession()
{
	return networkImpl_->sessionManager_->CreateUdpSession();
}

void NetworkUdp::CloseUdpSession( UdpSession* session )
{
	if( session == nullptr )
		return;

	//networkImpl_->serverApp_->OnClose( session );
	networkImpl_->sessionManager_->RestoreUdpSession( session );

	session->Close();
}

bool NetworkUdp::IsCompleteDatagram( const char* src, int srcSize )
{
	return networkImpl_->parser_->IsCompletePacket( src, srcSize );
}

int NetworkUdp::ParseDatagram( const char* src, int srcSize, UdpCommand* command )
{
	return networkImpl_->parser_->ParseDatagram( src, srcSize, command );
}

UdpCommand* NetworkUdp::AllocateUdpCommand()
{
	return networkImpl_->commandPool_.Alloc();
}

void NetworkUdp::DeallocateUdpCommand( UdpCommand* obj )
{
	networkImpl_->commandPool_.Free( obj );
}

void NetworkUdp::PushUdpCommand( UdpCommand* cmd )
{
	networkImpl_->workQueue_->PushCommand( cmd );
}

UdpCommand* NetworkUdp::PopUdpCommand()
{
	return networkImpl_->workQueue_->PopCommand();
}

void NetworkUdp::AddServerCommand( COMMAND_ID protocol, UdpCommandFunction_t command )
{
	if( networkImpl_->serverCommand_.find( protocol ) == networkImpl_->serverCommand_.end() )
	{
		networkImpl_->serverCommand_.insert( std::pair< COMMAND_ID, UdpCommandFunction_t >( protocol, command ) );
	}
	else
	{
		networkImpl_->serverCommand_[protocol] = command;
	}
}

UdpCommandFunction_t NetworkUdp::GetServerCommand( COMMAND_ID protocol )
{
	if( networkImpl_->serverCommand_.find( protocol ) == networkImpl_->serverCommand_.end() )
	{
		return nullptr;
	}

	return networkImpl_->serverCommand_[protocol];
}

void NetworkUdp::StartUdp()
{
	for( auto thread : networkImpl_->networkThreads_ )
	{
		if( thread != nullptr )
			thread->JoinThread();
	}
}

void NetworkUdp::StopUdp()
{
	networkImpl_->ioWork_->get_io_service().stop();

	for( auto thread : networkImpl_->networkThreads_ )
	{
		if( thread != nullptr )
			thread->StopThread();
	}

	for( auto thread : networkImpl_->workThreads_ )
	{
		if( thread != nullptr )
			thread->StopThread();
	}

}

boost::asio::io_service& NetworkUdp::GetIoService()
{
	return networkImpl_->ioService_;
}