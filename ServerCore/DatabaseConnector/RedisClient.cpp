#include "RedisClient.h"


RedisClient::RedisClient()
{
}


RedisClient::~RedisClient()
{
}

bool RedisClient::InitializeRedisClient()
{
	try
	{
		redisIoWork_ = std::make_shared<boost::asio::io_service::work>(redisIoService_);
	}
	catch( std::bad_alloc& )
	{
		return false;
	}

	return true;
}

void RedisClient::Run()
{
	redisIoService_.run();
}

void RedisClient::_TestRedis()
{
	//! Enable logging
	cpp_redis::active_logger = std::unique_ptr<cpp_redis::logger>(new cpp_redis::logger);

	cpp_redis::future_client client;

	client.connect("127.0.0.1", 6379, [](cpp_redis::redis_client&) {
		std::cout << "client disconnected (disconnection handler)" << std::endl;
	});

	//! Set a value
	auto set    = client.set("hello", "42");
	auto decrby = client.decrby("hello", 12);
	auto get    = client.get("hello");

	// commands are pipelined and only sent when client.commit() is called
	client.commit();

	// synchronous commit, no timeout
	// client.sync_commit();

	// synchronous commit, timeout
	// client.sync_commit(std::chrono::milliseconds(100));

	std::cout << "set 'hello' 42: " << set.get() << std::endl;

	cpp_redis::reply r = decrby.get();
	if (r.is_integer())
		std::cout << "After 'hello' decrement by 12: " << r.as_integer() << std::endl;

	std::cout << "get 'hello': " << get.get() << std::endl;
}