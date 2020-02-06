#ifndef _MYSQL_H_
#define _MYSQL_H_
#include <mysql/mysql.h>
#include <string>
#include <string.h>
#include <iostream>
#include <unistd.h>
using namespace std;
class Mysql
{
public:
	Mysql();
	~Mysql();
	bool Instructions(void);
	void Window_Display(void);
	bool Mysql_Insert(string sql);
	bool Mysql_Inquire(string sql);
	void Change_Order(string database, string id);
	string Mysql_Get_URL(string sql);
protected:
private:
	MYSQL conn;
	bool mode;//false:select true:insert
};

#endif