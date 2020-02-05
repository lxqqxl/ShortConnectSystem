OBJS = Myserver.o Redis.o Mysql.o Algorithm.o DatabaseSyn.o Myhttp.o
XX = g++

Myserver : $(OBJS) 
	$(XX) $(OBJS) -o Myserver -I /usr/include/mysql -lmysqlclient -lhiredis

Myserver.o : Myserver.cpp
	$(XX) -c Myserver.cpp -o Myserver.o -I /usr/include/mysql -lmysqlclient -lhiredis

Redis.o : Redis.cpp Redis.h
	$(XX) -c Redis.cpp -o Redis.o 

Mysql.o : Mysql.cpp Mysql.h
	$(XX) -c Mysql.cpp -o Mysql.o -I /usr/include/mysql -lmysqlclient

Algorithm.o : Algorithm.cpp Algorithm.h
	$(XX) -c Algorithm.cpp -o Algorithm.o

DatabaseSyn.o : DatabaseSyn.cpp DatabaseSyn.h
	$(XX) -c DatabaseSyn.cpp -o DatabaseSyn.o -I /usr/include/mysql -lmysqlclient

Myhttp.o : Myhttp.cpp Myhttp.h
	$(XX) -c Myhttp.cpp -o Myhttp.o -I /usr/include/mysql -lmysqlclient

clean:
	rm -rf *.o agent2
	