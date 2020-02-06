#ifndef _DATABASESYN_H_
#define _DATABASESYN_H_
#include <string>
#include <string.h>
#include "Mysql.h"
#include "Redis.h"


class DatabaseSyn
{
public:
	string location;
	DatabaseSyn();
	bool Find_data(string key);
	bool Find_data_Redis(string key);
	bool Find_data_Mysql(string key);
	void Mysql_to_redis(string key);
private:
	Mysql MYSQL;
	Redis REDIS;
};

#endif