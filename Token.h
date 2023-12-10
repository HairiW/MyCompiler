#pragma once
#include<string>
using namespace std;
class Token
{
private:
	string strToken;
	int type;
	int line;
public:
	Token()
	{
		strToken = "";
		type = -1;
		line = -1;
	}
	Token(const Token& obj)
	{
		strToken = obj.strToken;
		type = obj.type;
		line = obj.line;
	}
	Token(string _strToken, int _type, int _line)
	{
		strToken = _strToken;
		type = _type;
		line = _line;
	}
	string GetSt()
	{
		return strToken;
	}
	int GetType()
	{
		return type;
	}
	int GetLine()
	{
		return line;
	}
};