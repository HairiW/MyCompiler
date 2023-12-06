#pragma once
#pragma once
#include<map>
#include<string>
#include<fstream>
#include<set>
#include<list>
#include"LexAnalyzer.h"
#include"Token.h"
using namespace std;
class Syntax_Analyzer
{
private:
	list<Token> allToken;
	list<Token>::iterator it;
	string strToken;
	int str_code;
	int str_row;
	string strToken_next;
	int str_code_next;
	int str_row_next;
	int error_code;
	map<string, int> reserve_word;
	//部分first集
	set<string> block_first;
	set<string> statement_first;
	set<string> lexp_fisrt;
	set<string> exp_first;
	set<string> lop_first;
	//部分follow集
	set<string> body_follow;
	set<string> exp_follow;
	set<string> term_follow;
	set<string> factor_follow;
	set<string> lop_follow;
	set<string> id_follow;
	set<string> integer_follow;
public:
	Syntax_Analyzer(string _filepath)
	{
		Lexical_Analyzer la(_filepath);
		la.Proc();
		allToken = la.GetAllToken();
		it = allToken.begin();
		strToken = "";
		str_code = -1;
		str_row = -1;
		strToken_next = (*it).GetSt();
		str_code_next = (*it).GetType();
		str_row_next = (*it).GetLine();
		error_code = -1;
		//保留字初始化
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
		//部分first集初始化
		block_first = { "const","var","procedure","begin" };
		statement_first = { "if","while","call","begin","read","write" };//id
		lexp_fisrt = { "+","-","(","odd" };//id,integer
		exp_first = { "+","-","(" };//id,integer
		lop_first = { "=","<>","<","<=",">",">=" };
		//部分follow集初始化
		body_follow = { ";","begin","end","else" };
		exp_follow = { "then","do",";","end","else",",",")","=","<>","<","<=",">",">=" };
		term_follow = { "+","-","then","do",";","end","else",",",")","=","<>","<","<=",">",">=" };
		factor_follow = { "*" , "/","+","-","then","do",";","end","else",",",")","=","<>","<","<=",">",">=" };
		lop_follow = { "+", "-", "(" };//id,integer
		id_follow = { ";",":=",",","(",")","*","/","+","-","end","else","=","<>","<","<=",">",">=","then","do" };
		integer_follow = { "*","/","+","-",";","end","else",",",")","=","<>","<","<=",">",">=","then","do" };
	}
	void OpenFile(string str);
	void Advance();
	void Prog();//程序
	void Block();//块、程序体
	void Condecl();//常量说明
	void Const();//常量
	void Vardecl();//变量说明
	void Proc();//分程序
	void Body();//复合语句
	void Statement();//语句
	void LExp();//表达式
	void Exp();//条件
	void Term();//项
	void Factor();//因子
	void Lop();//关系运算符
	void Id();//标识符
	int	Error(int ecode, string str);
};