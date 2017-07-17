#include "IThread.h"



IThread::IThread()
{
}


IThread::~IThread()
{
	StopThread();
}

void IThread::StartThread()
{
	isRunning_ = true;

	for( unsigned int i = 0; i < threadCount_; ++i )
	{
		thread_.push_back( new std::thread( [&] () { Process(); } ) );
	}
}

void IThread::JoinThread()
{
	for( auto thread : thread_ )
	{
		if( thread != nullptr &&
			thread->joinable() == true )
		{
			thread->join();
		}
	}
}

void IThread::StopThread()
{
	isRunning_ = false;

	for( std::thread* thread : thread_ )
	{
		if( thread != nullptr )
		{
			try
			{
				if( thread->joinable() == false )
					delete thread;
			}
			catch( std::exception& e )
			{
				printf("%s\n", e.what() );
			}
		}

		thread = nullptr;
	}
}
