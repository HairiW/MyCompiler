#pragma once
#include"PerPcode.h"
#include<vector>
#include<string>
#include<fstream>
#include<iostream>
using namespace std;
class AllPcode
{
	vector<PerPcode> allPcode;
	fstream rf;
	/*
	FLA:
	LIT 0 a	将常数值取到栈顶，a为常数值
	LOD l a 将变量值取到栈顶，a为偏移量，l为层差
	STO l a 将栈顶内容送入某变量单元中，a为偏移量，l为层差
	CAL l a	调用过程，a为过程地址，l为层差
	INT 0 a 在运行栈中为被调用的过程开辟a个单元的数据区
	JMP 0 a 无条件跳转至a地址
	JPC 0 a	条件跳转，当栈顶布尔值非真则跳转至a地址，否则顺序执行
	WRT 0 0	输出栈顶
	OPR 0 0 过程调用结束后，返回调用点并退栈
	OPR 0 1 栈顶元素取反
	OPR 0 2 次栈顶与栈顶相加，退两个栈元素，结果值进栈
	OPR 0 3 次栈顶减去栈顶，退两个栈元素，结果值进栈
	OPR 0 4 次栈顶乘以栈顶，退两个栈元素，结果值进栈
	OPR 0 5 次栈顶除以栈顶，退两个栈元素，结果值进栈
	OPR 0 6 栈顶元素的奇偶判断，结果值在栈顶
	OPR 0 7 次栈顶与栈顶是否相等，退两个栈元素，结果值进栈
	OPR 0 8	次栈顶与栈顶是否不等，退两个栈元素，结果值进栈
	OPR 0 9	次栈顶是否小于栈顶，退两个栈元素，结果值进栈
	OPR 0 10 次栈顶是否大于等于栈顶，退两个栈元素，结果值进栈
	OPR 0 11 次栈顶是否大于栈顶，退两个栈元素，结果值进栈
	OPR 0 12 次栈顶是否小于等于栈顶，退两个栈元素，结果值进栈
	OPR 0 13 屏幕输出换行
	RED 0 0 从命令行读入一个输入置于栈顶
	*/
public:
	AllPcode(){}
	AllPcode(const AllPcode& obj)
	{
		allPcode = obj.allPcode;
	}
	~AllPcode()
	{
		rf.close();
	}
	vector<PerPcode> GetAllPcode()
	{
		return allPcode;
	}
	int GetAllPcodePtr()
	{
		return allPcode.size();
	}
	void Gen(PerPcode pcode)
	{
		allPcode.push_back(pcode);
	}
	void Gen(Operator F, int L, int A)
	{
		PerPcode* temp = new PerPcode(F, L, A);
		allPcode.push_back(*temp);
		string op;
		switch (F)
		{
		case Operator::LIT: {op = "LIT"; break; }
		case Operator::LOD: {op = "LOD"; break; }
		case Operator::STO: {op = "STO"; break; }
		case Operator::CAL: {op = "CAL"; break; }
		case Operator::INT: {op = "INT"; break; }
		case Operator::JMP: {op = "JMP"; break; }
		case Operator::JPC: {op = "JPC"; break; }
		case Operator::WRT: {op = "WRT"; break; }
		case Operator::RED: {op = "RED"; break; }
		case Operator::OPR: {op = "OPR"; break; }
		default:break;
		}
		rf << op << " " << L << " " << A << endl;
	}
	void Openfile(string _rfPath)
	{
		rf.open(_rfPath, ios::out);
		if (rf.fail())
		{
			cout << "文件打开失败！" << endl;
			exit(0);
		}
	}
};