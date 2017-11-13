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
						Command* command = NetworkCore::GetInstance().AllocateCommand();

						if( command == nullptr )
							return;

						int packetSize = NetworkCore::GetInstance().ParsePacket( sessionEvent.session_->RecvBufferPos(), static_cast<int>(sessionEvent.recvSize_), command );
						if( packetSize == 0 )
						{
							NetworkCore::GetInstance().DeallocateCommand( command );
							break;
						}
						else
						{
							NetworkCore::GetInstance().PushCommand( command );
							sessionEvent.session_->ReadBufferConsume( packetSize );
							sessionEvent.recvSize_ -= packetSize;
						}

					} while( sessionEvent.recvSize_ > 0 );

					sessionEvent.session_->ArrangeBuffer();
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