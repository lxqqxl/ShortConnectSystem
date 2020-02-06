#include "Algorithm.h"

string Algorithm::primaryKeytoString(int primaryKey)
{
	string res = "";
	do
	{
		res += basestring[primaryKey % (basestring.size())];
		primaryKey /= basestring.size();
	} while (primaryKey > 0);
	oriKeytoString = res;
	return res;
}

string Algorithm::random_Shuffle(string &str)
{
	random_shuffle(str.begin(), str.end());
	return str;
}