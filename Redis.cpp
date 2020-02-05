#include "Redis.h"


//≤‚ ‘
/*
#include <unistd.h>
#include <hiredis/hiredis.h> 
#include <iostream>
using namespace std;

int main()
{
#if 0
	redisContext* c = redisConnect("127.0.0.1", 6379);
	if (c->err)
	{
		redisFree(c);
		printf("Connect to redisServer faile\n");
		return -1;
	}
	printf("Connect to redisServer Success\n");

	const char* command1 = "set stest1 value1";
	redisReply* r = (redisReply*)redisCommand(c, command1);
	redisFree(c);
#endif

	Redis obj;
	string str;
	while (1)
	{
		getline(cin, str);
		obj.Redis_Insert(str);
		fflush(stdin);
		getline(cin, str);
		obj.Redis_Inquire(str);
	}
	return 0;
}
*/


Redis::Redis()
{
	redis_Init("127.0.0.1", 6379);
}

void Redis::redis_Init(const char *ip, const int port)
{
	RedisInterface = redisConnect(ip, port);
	if (!RedisInterface)
	{
		printf("Redis init failed\n");
	}
}


void Redis::Redis_Insert(string sql)
{
	redisReply *r = (redisReply*)redisCommand(this->RedisInterface, sql.c_str());
	if (!r)
	{
		printf("RedisCommand write failed\n");
		return;
	}
	else if (REDIS_REPLY_STATUS != r->type)
	{
		printf("RedisCommand Execute failed\n");
		freeReplyObject(r);
		return;
	}
	freeReplyObject(r);
}

string Redis::Redis_Inquire(string sql)
{
	redisReply *r = (redisReply*)redisCommand(this->RedisInterface, sql.c_str());
	if (!r)
	{
		printf("RedisCommand get failed\n");
		return "";
	}
	if (REDIS_REPLY_NIL == r->type)
		return "";
	string result = r->str;
	
	cout << result << endl;
	printf("The result: %s\n", result.c_str());
	freeReplyObject(r);
	return result;
}
