/*
Mysql:
	DATABASE: example
	TABLE: short_connect

	Table Field:
		id, short_key,  url
Redis:

*/

#include "DatabaseSyn.h"

DatabaseSyn::DatabaseSyn()
	:location("")
{}

bool DatabaseSyn::Find_data(string key)
{
	printf("find key\n");
	if (!Find_data_Redis(key))
	{
		printf("The key not in Redis\n");
		if (!Find_data_Mysql(key))
		{
			printf("The key not in DB\n");
			return false;
		}
		//Synchronize Mysql-->Redis
		printf("The key is in Mysql\n");
		Mysql_to_redis(key);
	}
	return true;
}

bool DatabaseSyn::Find_data_Redis(string key)
{
	string sql = "get " + key;
	location = REDIS.Redis_Inquire(sql);
	if (location.empty())
	{
		return false;
	}
	else
		return true;
}

bool DatabaseSyn::Find_data_Mysql(string key)
{
	string sql = "select * from short_connect where short_key = \"" + key + "\"";
	if (!MYSQL.Mysql_Inquire(sql))//not find
	{
		printf("The key doesn't in table, please check it!!!\n");
		return false;
	}
	return true;
}

/*
Key doesn't exist in Redis, Check if it exists in Mysql and do synchronization
*/
void DatabaseSyn::Mysql_to_redis(string key)
{
	string sql = "select url from short_connect where short_key = \"" + key + "\"";
	string url = MYSQL.Mysql_Get_URL(sql);
	location = url;
	//add this url to redis
	sql = "set " + key + " " + url;
	REDIS.Redis_Insert(sql);
}
