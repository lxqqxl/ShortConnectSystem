OBJS = Myserver.o Redis.o Mysql.o Algorithm.o DatabaseSyn.o Myhttp.o Epoll.o
XX = g++
MYSQLPATH = -I /usr/include/mysql

Myserver : $(OBJS) 
	$(XX) $(OBJS) -o Myserver $(MYSQLPATH) -lmysqlclient -lhiredis

Myserver.o : Myserver.cpp
	$(XX) -c Myserver.cpp -o Myserver.o $(MYSQLPATH)

Redis.o : Redis.cpp Redis.h
	$(XX) -c Redis.cpp -o Redis.o 

Mysql.o : Mysql.cpp Mysql.h
	$(XX) -c Mysql.cpp -o Mysql.o $(MYSQLPATH)

Algorithm.o : Algorithm.cpp Algorithm.h
	$(XX) -c Algorithm.cpp -o Algorithm.o

DatabaseSyn.o : DatabaseSyn.cpp DatabaseSyn.h
	$(XX) -c DatabaseSyn.cpp -o DatabaseSyn.o $(MYSQLPATH)

Myhttp.o : Myhttp.cpp Myhttp.h
	$(XX) -c Myhttp.cpp -o Myhttp.o $(MYSQLPATH)
	
Epoll.o : Epoll.cpp Epoll.h
	$(XX) -c Epoll.cpp -o Epoll.o $(MYSQLPATH)

clean:
	rm -rf *.o agent2
	