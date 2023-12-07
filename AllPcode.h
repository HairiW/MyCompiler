#pragma once
#include"PerPcode.h"
#include<vector>
using namespace std;
class AllPcode
{
	vector<PerPcode> allPcode;
	/*
	FLA:
	LIT 0 a	������ֵȡ��ջ����aΪ����ֵ
	LOD l a ������ֵȡ��ջ����aΪƫ������lΪ���
	STO l a ��ջ����������ĳ������Ԫ�У�aΪƫ������lΪ���
	CAL l a	���ù��̣�aΪ���̵�ַ��lΪ���
	INT 0 a ������ջ��Ϊ�����õĹ��̿���a����Ԫ��������
	JMP 0 a ��������ת��a��ַ
	JPC 0 a	������ת����ջ������ֵ��������ת��a��ַ������˳��ִ��
	WRT 0 0	���ջ��
	OPR 0 0 ���̵��ý����󣬷��ص��õ㲢��ջ
	OPR 0 1 ջ��Ԫ��ȡ��
	OPR 0 2 ��ջ����ջ����ӣ�������ջԪ�أ����ֵ��ջ
	OPR 0 3 ��ջ����ȥջ����������ջԪ�أ����ֵ��ջ
	OPR 0 4 ��ջ������ջ����������ջԪ�أ����ֵ��ջ
	OPR 0 5 ��ջ������ջ����������ջԪ�أ����ֵ��ջ
	OPR 0 6 ջ��Ԫ�ص���ż�жϣ����ֵ��ջ��
	OPR 0 7 ��ջ����ջ���Ƿ���ȣ�������ջԪ�أ����ֵ��ջ
	OPR 0 8	��ջ����ջ���Ƿ񲻵ȣ�������ջԪ�أ����ֵ��ջ
	OPR 0 9	��ջ���Ƿ�С��ջ����������ջԪ�أ����ֵ��ջ
	OPR 0 10 ��ջ���Ƿ���ڵ���ջ����������ջԪ�أ����ֵ��ջ
	OPR 0 11 ��ջ���Ƿ����ջ����������ջԪ�أ����ֵ��ջ
	OPR 0 12 ��ջ���Ƿ�С�ڵ���ջ����������ջԪ�أ����ֵ��ջ
	OPR 0 13 ��Ļ�������
	RED 0 0 �������ж���һ����������ջ��
	*/
public:
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
	}
};