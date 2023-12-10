#pragma once
#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>
#include<Utility>
#include<algorithm>
#include<iomanip>
#include<vector>
#include"Token.h"
using namespace std;

class Lexical_Analyzer
{
private:
	char ch;
	int code;
	int index;
	int const_cnt;
	int row;
	ifstream file;
	string strToken;
	string filepath;
	string line;
	map<string, int> reserve_word;
	vector<string> id_vector;
	vector<string> const_vector;
	vector<Token> allToken;
public:
	Lexical_Analyzer(string _filepath)
	{
		ch = ' ';
		code = 0;
		index = 0;
		const_cnt = -1;
		strToken = "";
		filepath = _filepath;
		line = "";
		row = 0;
		reserve_word["const"] = 1;
		reserve_word["id"] = 2;
		reserve_word["var"] = 3;
		reserve_word["procedure"] = 4;
		reserve_word[","] = 5;
		reserve_word[";"] = 6;
		reserve_word["begin"] = 7;
		reserve_word["end"] = 8;
		reserve_word["if"] = 9;
		reserve_word["then"] = 10;
		reserve_word["while"] = 11;
		reserve_word["do"] = 12;
		reserve_word["call"] = 13;
		reserve_word["read"] = 14;
		reserve_word["write"] = 15;
		reserve_word[":="] = 16;
		reserve_word["="] = 17;
		reserve_word["<>"] = 18;
		reserve_word["+"] = 19;
		reserve_word["-"] = 20;
		reserve_word["*"] = 21;
		reserve_word["/"] = 22;
		reserve_word[">"] = 23;
		reserve_word["<"] = 24;
		reserve_word[">="] = 25;
		reserve_word["<="] = 26;
		reserve_word["odd"] = 27;
		reserve_word["("] = 28;
		reserve_word[")"] = 29;
	}
	~Lexical_Analyzer()
	{
		file.close();
	}
	void GetPath(string str);
	void OpenFile();
	void GetChar();
	void GetBC();
	void Concat();
	int Reserve();
	bool IsLetter();
	bool IsDigit();
	void Retract();
	void ProcError(int type);//1.非法标识符:error_code = 1; 2.:后无=:error_code = 2; 3.非法字符:error_code = 3
	void Proc();
	void Scanner();
	void Flush();
	void SetAllToken();
	vector<Token> GetAllToken();
};