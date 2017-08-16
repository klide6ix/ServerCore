// MIT License
//
// Copyright (c) 2016-2017 Simon Ninon <simon.ninon@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
#include <algorithm>

#include <cpp_redis/network/tcp_client_iface.hpp>
#include <cpp_redis/redis_error.hpp>

#include "../DatabaseConnector/DatabaseCore.h"

namespace cpp_redis
{

namespace network
{

class tcp_client : public tcp_client_iface
{
public:
	//! ctor & dtor
	tcp_client( void ) : socket_(DatabaseCore::GetInstance()->GetRedisIoService()) {}
	~tcp_client( void ) = default;

public:
	//! start & stop the tcp client
	void connect( const std::string& addr, std::uint32_t port )
	{
		boost::asio::ip::tcp::resolver resolver(DatabaseCore::GetInstance()->GetRedisIoService());
		boost::asio::ip::tcp::resolver::query query( addr.c_str(), std::to_string( port ) );
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve( query );

		boost::asio::connect( socket_, endpoint_iterator );
	}

	void disconnect( bool wait_for_removal = false )
	{
		if( socket_.is_open() == true )
		{
			socket_.close();
		}
	}

	//! returns whether the client is currently connected or not
	bool is_connected( void ) const
	{
		return socket_.is_open();
	}

	void _handle_write( const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/ )
	{
		//printf("_handle_write(%d)\n", bytes_transferred );
	}

	void _handle_read( const boost::system::error_code& error, size_t bytes_transferred )
	{
		if( on_recv_call_back_ == false )
		{
			return;
		}

		if (error == boost::asio::error::eof)
		{
			return; // Connection closed cleanly by peer.
		}
		else if (error)
		{
			//printf("_handle_read(%d)(%s)\n", bytes_transferred, error.message().c_str() );
			return;
		}

		read_result converted_result;
		converted_result.success = true;
		converted_result.read_buffer.assign( recv_buffer_.begin(), recv_buffer_.begin() + bytes_transferred );

		on_recv_call_back_(converted_result);
	}

public:
	//! async read & write operations
	void async_read( read_request& request )
	{
		if( socket_.is_open() == false )
			return;

		if( recv_buffer_.size() != request.size )
		{
			recv_buffer_.resize( request.size );
		}

		socket_.async_receive( boost::asio::buffer( recv_buffer_, recv_buffer_.size() ),
							   boost::bind( &tcp_client::_handle_read, this,
											boost::asio::placeholders::error,
											boost::asio::placeholders::bytes_transferred ) );
	}

	void async_write( write_request& request )
		{
			auto callback = std::move( request.async_write_callback );
			
			boost::asio::async_write( socket_, boost::asio::buffer( request.buffer, request.buffer.size() ),
									  boost::bind( &tcp_client::_handle_write, this,
												   boost::asio::placeholders::error,
												   boost::asio::placeholders::bytes_transferred ) );
		}

public:
	//! set on disconnection handler
	void set_on_disconnection_handler( const disconnection_handler_t& disconnection_handler )
	{
		//socket_.set_on_disconnection_handler( disconnection_handler );
	}

	void set_on_recv_call_back( const async_read_callback_t& callback )
	{
		on_recv_call_back_ = callback;
	}

private:
	std::vector<char>			 recv_buffer_;
	boost::asio::ip::tcp::socket socket_;	
	async_read_callback_t		 on_recv_call_back_;
};

} //! network

} //! cpp_redis
