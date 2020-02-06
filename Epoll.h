#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <string.h>

#include "Myhttp.h"

using namespace std;

class Epoll
{
private:
	int epollfd;
	int listenfd;
	const int MAX_LISTEN_QUEUE = 10;
	const int MAX_SIZE_EPOLL_EVENTSET = 1024;
	const int MAX_READ_BUFFER_LEN = 1024;
	const int MAX_WRITE_BUFFER_LEN = 1024;
public:
	Epoll();
	~Epoll();
	int Setup(char *ip, int port);
	void do_epoll(void);
private:
	void add_event(int fd, int state);
	void delete_event(int fd, int state);
	void change_event(int fd, int state);

	void handle_accept(int fd);
	void handle_read(int fd);
	void handle_write(int fd);

	void handle_event(struct epoll_event *eventSet, int num, int listenfd);
};

#endif