#pragma once
#include <string>
#include <vector>

class StringTokenizer
{
public:
	StringTokenizer( const std::string& inputString, const std::string& seperator ) : input_( inputString ), delimiter_( seperator )
	{
		_split();
	}
	~StringTokenizer() {}

	size_t tokenCount() const { return token_.size(); }
	std::string tokenAt( size_t idx ) const { return token_.at(idx); }

protected:
	std::string input_;
	std::string delimiter_;
	std::vector<std::string> token_;
	std::vector<std::string>::iterator index_;

	void _split()
	{
		std::string::size_type lastPos = input_.find_first_not_of( delimiter_, 0 );
		std::string::size_type pos = input_.find_first_of( delimiter_, lastPos );

		while( std::string::npos != pos || std::string::npos != lastPos )
		{
			token_.push_back( input_.substr( lastPos, pos - lastPos ) );

			lastPos = input_.find_first_not_of( delimiter_, pos );
			pos = input_.find_first_of( delimiter_, lastPos );
		}

		index_ = token_.begin();
	}
};