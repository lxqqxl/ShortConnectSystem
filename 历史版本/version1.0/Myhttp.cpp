#include "Myhttp.h"


char* MyHttp::Do_response_headbuf()
{
	static char buf[MAX_RESPONSE_HEADBUF_LEN] = { 0 };
	sprintf(buf, "HTTP1.1 301 Jump_forever\r\nLocation:WWW.baidu.com\r\n\r\n");
	//response_headbuf = buf;
	memcpy(response_headbuf, buf, strlen(buf));
	return buf;
}

/*
修改时间:2020/2/5
函数说明:仅仅只处理了http请求报头的第一行
*/
void MyHttp::Head_Parsing(char *buf)
{
	if (buf == nullptr)
	{
		printf("The head buf is NULL!\n");
		return;
	}
	
	//按照行进行解析
	int i = 0;
	while (buf[i] != ' ')//method
	{
		http_inf.method += buf[i];
		++i;
	}
	++i;
	++i;// '/'
	while (buf[i] != ' ')//url
	{
		http_inf.url += buf[i];
		++i;
	}
	++i;
	while (buf[i] != '\r' && buf[i + 1] != '\n')//protocol
	{
		http_inf.protocol += buf[i];
		++i;
	}
}

/*
时间:2020/2/5
函数说明:打印请求报头，和其他请求头部的细节信息
*/

void MyHttp::Print_Head_Inf(char *buf)
{
	printf("The HeadInf:\r\n%s", buf);
	printf("Method: %s\n", http_inf.method.c_str());
	printf("url: %s\n", http_inf.url.c_str());
	printf("Protocol: %s\n", http_inf.protocol.c_str());
}


/*
时间:2020/2/5
函数说明:处理GET请求事件
*/

char* MyHttp::Do_Get_Events()
{
	if (!Http_DatabaseSyn.Find_data(http_inf.url))
	{
		return nullptr;
	}
	else
	{
		static char buf[100] = { 0 };
		sprintf(buf, "HTTP1.1 302 ok\r\nConnection:Close\r\nLocation:%s\r\n\r\n", Http_DatabaseSyn.location.c_str());
		return buf;
	}
}