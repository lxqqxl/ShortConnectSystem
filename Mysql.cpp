#include "Mysql.h"

/*
Test Module
*/

/*
int main()
{
	Mysql My;
#if 0
	string sql;
	getline(cin, sql);
	cout << sql << endl;
	My.Mysql_Insert(sql);
	fflush(stdin);
	getline(cin, sql);
	cout << sql << endl;
	My.Mysql_Inquire(sql);
#endif

#if 0
	string sql;
	while (1)
	{
		getline(cin, sql);
		My.Mysql_Get_URL(sql);
		fflush(stdin);
	}
#endif

#if 0
	My.Instructions();
#endif
	return 0;
}
*/

Mysql::Mysql():
	mode(false)
{
	mysql_init(&conn);

	if (!mysql_real_connect(&conn, "localhost", "root", "LXQqh8349532", "example", 3306, NULL, 0))
	{
		cout << "Mysql connect failed" << endl;
		exit(-1);
	}
	//cout << "----------mysql conenct success----------" << endl;
}

Mysql::~Mysql()
{
	mysql_close(&conn);
}


bool Mysql::Instructions(void)
{
	char buf[2] = { 0 };
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		Window_Display();
		printf("Please input operator\n");
		fflush(stdout);
		int len = read(0, buf, sizeof(buf));
		fflush(stdout);
		if (len < 0)
		{
			printf("Instructions read error\n");
			continue;
		}

		string sql;

		if (buf[0] == '1')
		{
			printf("Insert data to DB\n");
			getline(cin, sql);
			Mysql_Insert(sql);
			//Change_Order();//这里看如何修改
			fflush(stdout);
		}
		else
		{
			printf("Inquire data to DB\n");
			getline(cin, sql);
			if (Mysql_Inquire(sql))
			{
				cout << "find data in DB" << endl;
			}
			else
			{
				cout << "data not in DB" << endl;
			}
			fflush(stdout);
		}
	}
}

bool Mysql::Mysql_Insert(string sql)
{
	if (sql.empty())
	{
		printf("sql sentence is null\n");
		return false;
	}
	if (mysql_query(&(this->conn), sql.c_str()))//error
	{
		printf("Mysql insert error: %s\n", mysql_error(&(this->conn)));
		return false;
	}
	return true;
}

bool Mysql::Mysql_Inquire(string sql)
{
	if (sql.empty())
	{
		printf("sql sentence is null\n");
		return false;;
	}

	if (mysql_query(&(this->conn), sql.c_str()))
	{
		printf("Mysql inquire error: %s\n", mysql_error(&(this->conn)));
		return false;
	}

	MYSQL_RES *res;
	res = mysql_store_result(&(this->conn));

	my_ulonglong rows = mysql_num_rows(res);
	if ((unsigned long)rows == 0)
	{
		return false;
	}
	int cols = mysql_num_fields(res);

	printf("+------------------------------|\n");
	printf("|Total rows:%lu Total fields:%d|\n", (unsigned long)rows, cols);
	printf("+------------------------------|\n");

	MYSQL_FIELD *field;

	while ((field = mysql_fetch_field(res)))
	{
		printf("%-20s", field->name);
	}
	printf("\n");

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(res)))
	{
		for (int i = 0; i < cols; i++)
		{
			//cout << (row[i] ? row[i] : "NULL") << " ";
			printf("%-20s", row[i] ? row[i] : "NULL");
		}
		printf("\n");
	}
	mysql_free_result(res);
	return true;
}

void Mysql::Window_Display(void)
{
	printf("+-------Instructions-------+\n");
	printf("|        1: insert         |\n");
	printf("|        0: select         |\n");
	printf("+-------Instructions-------+\n");
}

void Mysql::Change_Order(string database, string id)
{
	//string str = "alter table category drop cid; alter table category add cid int(3) not null first; alter table category modify column cid int(3) not null auto_increment, add primary key(cid)";
	string str = "ALTER TABLE " + database + " drop " + id + "; ";
	str += "alter table category add " + id + " int(10) not null first; ";
	str += "alter table category modify column " + id + " int(10) not null auto_increment, add primary key" + "(" + id + ")";

	mysql_query(&(this->conn), str.c_str());
}

string Mysql::Mysql_Get_URL(string sql)
{
	//The key is surely in Mysql
	if (mysql_query(&(this->conn), sql.c_str()))
	{
		printf("Mysql inquire error: %s\n", mysql_error(&(this->conn)));
		return "";
	}

	MYSQL_RES *res;
	res = mysql_store_result(&(this->conn));

	MYSQL_ROW row;
	/*
	while ((row = mysql_fetch_row(res)))
	{
		for (int i = 0; i < cols; i++)
		{
			//cout << (row[i] ? row[i] : "NULL") << " ";
			printf("%-20s", row[i] ? row[i] : "NULL");
		}
		printf("\n");
	}
	*/
	row = mysql_fetch_row(res);
	string url = row[0];
	cout << url << endl;
	mysql_free_result(res);
	return url;
}