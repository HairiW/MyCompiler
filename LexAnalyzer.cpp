#include"LexAnalyzer.h"
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>
#include<Utility>
#include<algorithm>
#include<iomanip>
using namespace std;
void Lexical_Analyzer::GetPath(string str)
{
	filepath = str;
}
void Lexical_Analyzer::OpenFile()
{
	file.open(filepath);
	if (!file)
	{
		cout << "�ļ���ʧ�ܣ�\n" << endl;
		exit(0);
	}
}
void Lexical_Analyzer::Scanner()
{
	Flush();
	GetChar();
	GetBC();
	if (ch == '\n' || ch == ' ')
		return;
	if (IsLetter())
	{
		while (IsLetter() or IsDigit())//��ch����ĸ������ʱ
		{
			Concat();
			GetChar();
		}
		if (index <= line.length())
			Retract();
		code = Reserve();
		if (code)//����Ǳ�����
			SetAllToken();
		else//����Ϊ��ʶ��
		{
			code = reserve_word["id"];
			SetAllToken();
		}
	}
	else if (IsDigit())
	{
		while (IsDigit())
		{
			Concat();
			GetChar();
		}
		if (IsLetter())
		{
			//ProcError(1);
			while (IsLetter())
				GetChar();
			Retract();
		}
		else
		{
			Retract();
			code = reserve_word["const"];
			SetAllToken();
		}
	}
	else if (ch == ',')
	{
		Concat();
		code = Reserve();
		SetAllToken();
	}
	else if (ch == ';')
	{
		Concat();
		code = Reserve();
		SetAllToken();
	}
	else if (ch == ':')
	{
		Concat();
		GetChar();
		if (ch == '=')
		{
			Concat();
			code = Reserve();
			SetAllToken();
		}
		else
		{
			Retract();
		}
	}
	else if (ch == '=')
	{
		Concat();
		code = Reserve();
		SetAllToken();
	}
	else if (ch == '<')
	{
		Concat();
		GetChar();
		if (ch == '>' || ch == '=')
			Concat();
		else
			Retract();
		code = Reserve();
		SetAllToken();
	}
	else if (ch == '+')
	{
		Concat();
		code = Reserve();
		SetAllToken();
	}
	else if (ch == '-')
	{
		Concat();
		code = Reserve();
		SetAllToken();
	}
	else if (ch == '*')
	{
		Concat();
		code = Reserve();
		SetAllToken();
	}
	else if (ch == '/')
	{
		Concat();
		code = Reserve();
		SetAllToken();
	}
	else if (ch == '>')
	{
		Concat();
		GetChar();
		if (ch == '=')
			Concat();
		else
			Retract();
		code = Reserve();
		SetAllToken();
	}
	else if (ch == '(')
	{
		Concat();
		code = Reserve();
		SetAllToken();
	}
	else if (ch == ')')
	{
		Concat();
		code = Reserve();
		SetAllToken();
	}
	else
		//ProcError(3);
		return;
}
void Lexical_Analyzer::GetChar()
{
	if (index + 1 <= line.length())
		ch = line[index++];
	else
	{
		ch = ' ';
		index++;
	}
}
void Lexical_Analyzer::GetBC()
{
	while (ch == ' ' || ch == '\n' || ch == '\t')
	{
		if (index + 1 <= line.length())
			ch = line[index++];
		else
		{
			ch = '\n';
			break;
		}
	}
}
void Lexical_Analyzer::Concat()
{
	strToken = strToken + ch;
}
int Lexical_Analyzer::Reserve()
{
	if (reserve_word[strToken])
		return reserve_word[strToken];
	else
		return 0;

}
bool Lexical_Analyzer::IsLetter()
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return true;
	else
		return false;
}
bool Lexical_Analyzer::IsDigit()
{
	if ((ch >= '0' && ch <= '9'))
		return true;
	else
		return false;
}
void Lexical_Analyzer::Retract()
{
	index--;
}
void Lexical_Analyzer::ProcError(int type)
{
	if (type == 1)
		cout << "��ʶ�����������ֿ�ͷ��";
	else if (type == 2)
		cout << "':'��ȱ��'='��";
	else
		cout << "��������Ƿ��ַ���";
}
void Lexical_Analyzer::Proc()
{
	OpenFile();
	row = 1;
	while (getline(file, line))
	{
		index = 0;
		while (index < line.length())
			Scanner();
		row++;
	}
}
void Lexical_Analyzer::Flush()
{
	code = 0;
	ch = ' ';
	strToken = "";
}
void Lexical_Analyzer::SetAllToken()
{
	Token* ptr = new Token(strToken, code, row);
	allToken.push_back(*ptr);
}
vector<Token> Lexical_Analyzer::GetAllToken()
{
	return allToken;
}