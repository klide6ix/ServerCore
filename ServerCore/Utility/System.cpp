#include <string>
#include "System.h"

#ifdef WIN32
#else
#include <execinfo.h>
#endif

std::string CSystem::getStackFrame( unsigned int depth )
{
	std::string backTrace;

#ifdef WIN32
#else
	void* trace[1024];
	int trace_size = backtrace( trace, depth );
	char** messages = backtrace_symbols( trace, trace_size );
	for( int i = 1; i < trace_size; ++i )
	{
		CStringTokenizer token( message[i], "(+)" );

		int status = 0;
		if( token.countToken() >= 2 )
		{
			char* symbol = abi::__cxa_demangle( token.tokenAt(1).c_str(), 0, 0, &status );

			if( symbol == nullptr )
				symbol = message(i);

			backTrace += mesasge[i];
		}

		backTrace += "|";
	}

	free( messages );
#endif

	return backTrace;
}