#include <unistd.h>
#include <iostream>
//#include <sys/socket.h>
//#include <arpa/inet.h>
//#include <sys/epoll.h>
//#include <string.h>
//#include <string>
//
//#include "Myhttp.h"
#include "Epoll.h"

using namespace std;

const int MAX_LISTEN_QUEUE = 10;
const int MAX_SIZE_EPOLL_EVENTSET = 1024;
const int MAX_READ_BUFFER_LEN = 1024;
const int MAX_WRITE_BUFFER_LEN = 1024;

//This var only Test
//char global_buf[100];

int setup(char *ip, int port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}

	int opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		perror("setsockopt");
		close(sockfd);
		exit(-1);
	}

	struct sockaddr_in server_inf;
	memset(&server_inf, 0, sizeof(server_inf));
	server_inf.sin_family = AF_INET;
	server_inf.sin_addr.s_addr = inet_addr(ip);
	server_inf.sin_port = htons(port);
	if (bind(sockfd, (struct sockaddr*)&server_inf, sizeof(server_inf)) < 0)
	{
		perror("bind");
		close(sockfd);
		exit(-1);
	}

	if (listen(sockfd, MAX_LISTEN_QUEUE) < 0)
	{
		perror("listen");
		close(sockfd);
		exit(-1);
	}

	return sockfd;
}

/*
do epoll event
*/
void add_event(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.events = state;
	ev.data.fd = fd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) < 0)
	{
		perror("add_event epoll_ctl");
		return;
	}
}
void delete_event(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.events = state;
	ev.data.fd = fd;
	if (epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev) < 0)
	{
		perror("delete_event epoll_ctl");
		return;
	}
}
void change_event(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.events = state;
	ev.data.fd = fd;
	if (epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev))
	{
		perror("change_event epoll_event");
		return;
	}
}

//handle_accept
void handle_accept(int epollfd, int fd)
{
	struct sockaddr_in server_inf;
	memset(&server_inf, 0, sizeof(server_inf));
	socklen_t len = sizeof(server_inf);

	int connfd = accept(fd, (struct sockaddr*)&server_inf, &len);
	if (connfd < 0)
	{
		perror("accept");
		return;
	}
	else
	{
		printf("Accept a new connect from ip: %s port: %d\n", inet_ntoa(server_inf.sin_addr), ntohs(server_inf.sin_port));
		add_event(epollfd, connfd, EPOLLIN | EPOLLET);
	}
}
//handle_read
void handle_read(int epollfd, int fd)
{
	char buffer[MAX_READ_BUFFER_LEN] = { 0 };
	int len = read(fd, buffer, sizeof(buffer) - 1);
	if (len < 0)
	{
		perror("read");
		return;
	}
	else
	{
		if (len == 0)
		{
			printf("Client quit\n");
			//delete_event(epollfd, fd, EPOLLIN);//由于是短链接，在前面已经删除，这里再删除会出现删除错误。所以只能打开一处
			//close(fd);//close connfd first
			return;
		}
		else
		{
			//memcpy(global_buf, buffer, strlen(buffer));
			printf("%s\n", buffer);
			change_event(epollfd, fd, EPOLLOUT | EPOLLET);
		}
	}
}
//handle_write
void handle_write(int epollfd, int fd)
{
	char buffer[MAX_WRITE_BUFFER_LEN] = { 0 };
	/*
	一下作为测试使用
	*/
	
	/*
	处理Redis
	*/
	/*
	Redis redis;
	redis.redis_Init();
	string str = "get xianqi";
	string location = redis.Redis_Inquire(str);

	char buf[100] = { 0 };
	sprintf(buf, "HTTP1.1 302 ok\r\nConnection:Close\r\nLocation:%s\r\n\r\n", location.c_str());
	*/
	
	MyHttp myhttp;
	//myhttp.Head_Parsing(global_buf);
	char *buf = myhttp.Do_Get_Events();

#if 0
	char buf[] = "HTTP1.1 302 ok\r\nConnection:Close\r\nLocation:https://www.baidu.com/\r\n\r\n";
#endif

	write(fd, buf, strlen(buf));
	//add_event(epollfd, fd, EPOLLIN | EPOLLET);
	delete_event(epollfd, fd, EPOLLIN);
	close(fd);
}
//添加到事件处理
void handle_event(int epollfd, struct epoll_event *eventSet, int num, int listenfd)
{
	for (int i = 0; i < num; i++)
	{
		printf("num = %d\n", num);
		int fd = eventSet[i].data.fd;
		if (fd == listenfd && (eventSet[i].events & (EPOLLIN | EPOLLET)))
		{
			handle_accept(epollfd, fd);
		}
		else if (eventSet[i].events & (EPOLLIN | EPOLLET))
		{
			handle_read(epollfd, fd);
		}
		else if (eventSet[i].events & (EPOLLOUT | EPOLLET))
		{
			handle_write(epollfd, fd);
			printf("Write is ok\n");
		}
	}
}

void do_epoll(int listenfd)
{
	int epollfd = epoll_create1(EPOLL_CLOEXEC);

	//add listen to epollfd
	add_event(epollfd, listenfd, EPOLLIN | EPOLLET);

	struct epoll_event eventSet[MAX_SIZE_EPOLL_EVENTSET];

	while (1)
	{
		int num = epoll_wait(epollfd, eventSet, MAX_SIZE_EPOLL_EVENTSET, -1);
		if (num < 0)
		{
			if (errno = EINTR)
			{
				continue;
			}
			perror("epoll_wait");
			printf("num is less than zero\n");
			break;
		}
		else if (num == 0)
		{
			printf("epoll_wait timeout\n");
			continue;
		}
		else
		{
			handle_event(epollfd, eventSet, num, listenfd);
		}
	}
	close(epollfd);
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("The input must be %s <ip> <port>\n", argv[0]);
		return -1;
	}

#if 0
	int listenfd = setup(argv[1], atoi(argv[2]));

	do_epoll(listenfd);
#endif

	std::cout << "main" << std::endl;
	Epoll epoll;
	epoll.Setup(argv[1], atoi(argv[2]));
	epoll.do_epoll();
	return 0;
}
