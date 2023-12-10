#pragma once
#include<string>
using namespace std;
class PerSymbol
{
private:
	int type;	//����������������
	int value;	//��ʾ�����������ֵ
	int level;	//Ƕ�ײ��
	int address;//���������Ƕ�׹��̻���ַ�ĵ�ַ
	int size;	//��ʾ������������������ռ�Ĵ�С(��һ����ʵĬ��Ϊ0�� ��û���õ�)
	string name;//������������������
public:
	//����ר�ù��캯��
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
	//����������ר�ù��캯��
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