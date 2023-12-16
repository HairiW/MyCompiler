#pragma once
#include"PerSymbol.h"
#include<vector>
using namespace std;
class AllSymbol
{
private:
	vector<PerSymbol> allSymbol;
	int con = 1;	//����
	int var = 2;	//����
	int proc = 3;	//����
	int index = 0;
public:
	AllSymbol(){}
	AllSymbol(const AllSymbol &obj)
	{
		allSymbol = obj.allSymbol;
	}
	//���볣��
	void EnterCon(int value, int level, int address, string name)
	{
		PerSymbol* ptr = new PerSymbol(con, value, level, address, 0, name);
		allSymbol.push_back(*ptr);
		index++;
	}
	//�������
	void EnterVar(int level, int address, string name)
	{
		PerSymbol* ptr = new PerSymbol(var, level, address, 0, name);
		allSymbol.push_back(*ptr);
		index++;
	}
	//�������
	void EnterProc(int level, int address, string name)
	{
		PerSymbol* ptr = new PerSymbol(proc, level, address, 0, name);
		allSymbol.push_back(*ptr);
		index++;
	}
	//�ڵ�ǰ����ҷ����Ƿ����
	bool isCurExist(string name, int level)
	{
		for (int ptr = allSymbol.size() - 1; ptr >=0; ptr--)
		{
			if (allSymbol[ptr].GetName() == name && allSymbol[ptr].GetLevel() == level)
				return true;
		}
		return false;
	}
	//��֮ǰ�Ĳ����в��ҷ����Ƿ����
	bool isPreExist(string name, int level)
	{
		for (int ptr = 0; ptr < allSymbol.size(); ptr++)
		{
			if (allSymbol[ptr].GetName() == name && allSymbol[ptr].GetLevel() <= level)
				return true;
		}
		return false;
	}
	//�����Ʋ��ұ���
	PerSymbol GetSymbol(string name)
	{
		for (int ptr = allSymbol.size() - 1; ptr >= 0; ptr--)
		{
			if (allSymbol[ptr].GetName() == name)
				return allSymbol[ptr];
		}
		PerSymbol* null = new PerSymbol(0, 0, 0, 0, "");
		return *null;
	}
	int GetLevelProc(int level)
	{
		for (int ptr = allSymbol.size() - 1; ptr >= 0; ptr--)
		{
			if (allSymbol[ptr].GetType() == proc)
				return ptr;
		}
		return -1;
	}
	void Free(int level)//�ͷ�level����ű�
	{
		for (int i = allSymbol.size() - 1; i >= 0; i--)
		{
			if (allSymbol[i].GetLevel() == level)
				allSymbol.pop_back();
		}
	}
	vector<PerSymbol> GetAllSymbol()
	{
		return allSymbol;
	}
	void SetIndex(int _index)
	{
		index = _index;
	}
	int GetIndex()
	{
		return index;
	}
	int GetLength()
	{
		return allSymbol.size();
	}
	int GetCon()
	{
		return con;
	}
	int GetVar()
	{
		return var;
	}
	int GetProc()
	{
		return proc;
	}
};