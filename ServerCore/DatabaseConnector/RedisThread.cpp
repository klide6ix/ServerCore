#include "RedisThread.h"
#include "RedisClient.h"
#include "DatabaseCore.h"


RedisThread::RedisThread()
{
}


RedisThread::~RedisThread()
{
}

void RedisThread::Process()
{
	DatabaseCore::GetInstance()->RunRedisClient();
}