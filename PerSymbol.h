#pragma once
#include<string>
using namespace std;
class PerSymbol
{
private:
	int type;	//常量、变量、过程
	int value;	//表示常量或变量的值
	int level;	//嵌套层次
	int address;//相对于所在嵌套过程基地址的地址
	int size;	//表示常量，变量，过程所占的大小(这一项其实默认为0， 并没有用到)
	string name;//常量、变量、过程名
public:
	//常量专用构造函数
	PerSymbol(const PerSymbol &obj)
	{
		type = obj.type;
		value = obj.value;
		level = obj.level;
		address = obj.address;
		size = obj.size;
		name = obj.name;
	}
	PerSymbol(int _type,int _value,int _level,int _address,int _size,string _name)
	{
		type = _type;
		value = _value;
		level = _level;
		address = _address;
		size = _size;
		name = _name;
	}
	//变量、过程专用构造函数
	PerSymbol(int _type, int _level, int _address, int _size, string _name)
	{
		type = _type;
		level = _level;
		address = _address;
		size = _size;
		name = _name;
	}
	void SetType(int _type)
	{
		type = _type;
	}
	int GetType()
	{
		return type;
	}
	void SetValue(int _value)
	{
		value = _value;
	}
	int GetValue()
	{
		return value;
	}
	void SetLevel(int _level)
	{
		level = _level;
	}
	int GetLevel()
	{
		return level;
	}
	void SetAddress(int _address)
	{
		address = _address;
	}
	int GetAddress()
	{
		return address;
	}
	void SetSize(int _size)
	{
		size = _size;
	}
	int GetSize()
	{
		return size;
	}
	void SetName(string _name)
	{
		name = _name;
	}
	string GetName()
	{
		return name;
	}
};