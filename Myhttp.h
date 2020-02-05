#ifndef _MYHTTP_H_
#define _MYHTTP_H_

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sstream>
#include "Mysql.h"
#include "Redis.h"
#include "DatabaseSyn.h"
using namespace std;

const int MAX_RESPONSE_HEADBUF_LEN = 1204; //This  parameter is related to browser, HTTP protocol doesn't limit the length 

enum ResponseStatus
{
	SUCCESS = 0,
	NOT_FOUND
};


class MyHttp
{
public:
	struct HTTP_INF
	{
		string protocol;
		string method;
		string url;
		HTTP_INF() : protocol(""), method(""), url("") {}
	}http_inf;
public:
	char* Do_response_headbuf();
	//void Send_File();
	void Head_Parsing(char *buf);//头部解析
	void Print_Head_Inf(char *buf);//打印头部和头部其他信息测试
	char* Do_Get_Events();
protected:
private:
	char response_headbuf[MAX_RESPONSE_HEADBUF_LEN] = {0};
	DatabaseSyn Http_DatabaseSyn;
	int fd;

};

#endif // !_MYHTTP_H_
