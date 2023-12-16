#pragma once
#include"PerSymbol.h"
#include<vector>
using namespace std;
class AllSymbol
{
private:
	vector<PerSymbol> allSymbol;
	int con = 1;	//常量
	int var = 2;	//变量
	int proc = 3;	//过程
	int index = 0;
public:
	AllSymbol(){}
	AllSymbol(const AllSymbol &obj)
	{
		allSymbol = obj.allSymbol;
	}
	//插入常量
	void EnterCon(int value, int level, int address, string name)
	{
		PerSymbol* ptr = new PerSymbol(con, value, level, address, 0, name);
		allSymbol.push_back(*ptr);
		index++;
	}
	//插入变量
	void EnterVar(int level, int address, string name)
	{
		PerSymbol* ptr = new PerSymbol(var, level, address, 0, name);
		allSymbol.push_back(*ptr);
		index++;
	}
	//插入过程
	void EnterProc(int level, int address, string name)
	{
		PerSymbol* ptr = new PerSymbol(proc, level, address, 0, name);
		allSymbol.push_back(*ptr);
		index++;
	}
	//在当前层查找符号是否存在
	bool isCurExist(string name, int level)
	{
		for (int ptr = allSymbol.size() - 1; ptr >=0; ptr--)
		{
			if (allSymbol[ptr].GetName() == name && allSymbol[ptr].GetLevel() == level)
				return true;
		}
		return false;
	}
	//在之前的层数中查找符号是否存在
	bool isPreExist(string name, int level)
	{
		for (int ptr = 0; ptr < allSymbol.size(); ptr++)
		{
			if (allSymbol[ptr].GetName() == name && allSymbol[ptr].GetLevel() <= level)
				return true;
		}
		return false;
	}
	//按名称查找变量
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
	void Free(int level)//释放level层符号表
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