#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_
#include <algorithm>
#include <string>
#include <string.h> 
#include <unistd.h>
using namespace std;

class Algorithm
{
private:
	string basestring = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string oriKeytoString;
public:
	string random_Shuffle(string &str);
	string primaryKeytoString(int primaryKey);
};

#endif