#include <vector>

#include "NetworkThread.h"
#include "NetworkCore.h"

#include "NetworkCore.h"
#include "Session.h"

void NetworkThread::Process()
{
	std::vector<SessionEvent> _sessionEvents;

	while( IsRunning() == true )
	{
		NetworkCore::GetInstance().SelectSession( _sessionEvents );

		for( auto sessionEvent : _sessionEvents )
		{
			if( sessionEvent.session_ == nullptr )
				continue;

			switch( sessionEvent.event_ )
			{
			case SESSION_CLOSE:
				{
					NetworkCore::GetInstance().CloseSession( sessionEvent.session_ );
				}
				break;

			case SESSION_RECV:
				{
					do
					{
						Packet* packet = NetworkCore::GetInstance().AllocatePacket();

						if( packet == nullptr )
							return;

						int packetSize = 0;
						if( NetworkCore::GetInstance().DecodePacket( sessionEvent.session_->RecvBufferPos(), static_cast<int>(sessionEvent.recvSize_), packet->GetPacketBuffer(), packetSize ) == false )
						{
							NetworkCore::GetInstance().FreePacket( packet );
							break;
						}
						else
						{
							NetworkCore::GetInstance().PushCommand( Command( static_cast<COMMAND_ID>(packet->GetProtocol()), static_cast<void*>(packet) ) );
							sessionEvent.session_->RecvBufferRestore( packet->GetPacketSize() );
							sessionEvent.recvSize_ -= packet->GetPacketSize();
						}

					} while( true );

					int postResult = sessionEvent.session_->RecvPost();
					
					if( postResult == -1 )
					{
						NetworkCore::GetInstance().CloseSession( sessionEvent.session_ );
					}
					else if( postResult == 0 )
					{
						NetworkCore::GetInstance().RecvRetryProcess( sessionEvent.session_ );
					}
				}
				break;
			}
		}

		_sessionEvents.clear();
	}
}