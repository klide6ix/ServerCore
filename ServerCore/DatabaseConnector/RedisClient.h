#pragma once

#include <cpp_redis/cpp_redis>

#include <boost/asio.hpp>

class RedisClient
{
	boost::asio::io_service	redisIoService_;
	std::shared_ptr<boost::asio::io_service::work>	redisIoWork_ = nullptr;

public:
	RedisClient();
	virtual ~RedisClient();

	bool InitializeRedisClient();
	void Run();

	boost::asio::io_service& GetIoService() { return redisIoService_; }

public:
	void _TestRedis();
};

