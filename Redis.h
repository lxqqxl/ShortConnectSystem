#ifndef _REDIS_H_
#define _REDIS_H_
#include <hiredis/hiredis.h> 
#include <string>
#include <string.h>
#include <iostream>
using namespace std;
/*
redis 
*/

class Redis
{
private:
	redisContext* RedisInterface;
protected:
public:
	void redis_Init(const char *ip = "127.0.0.1", const int port = 6379);
	void Redis_Insert(string sql);
	string Redis_Inquire(string sql);
	Redis();
	~Redis()
	{
		redisFree(RedisInterface);
	}
};

#endif