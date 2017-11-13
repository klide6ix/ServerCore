#pragma once

#include <memory>

class Command;
class IParser
{
public:
	IParser() {}
	virtual ~IParser() {}

	virtual int ParseStream( const char* src, int srcSize, Command* command ) = 0;
};

class ParserDefault : public IParser
{
public:
	ParserDefault() {}
	virtual ~ParserDefault() {}

	virtual int ParseStream( const char* src, int srcSize, Command* command )
	{
		return srcSize;
	}
};
