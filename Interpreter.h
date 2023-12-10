#pragma once
#include"PerPcode.h"
#include"AllPcode.h"
#include"Operator.h"
#include<string>
#include<vector>
using namespace std;

class Interpreter
{
private:
	//���¼ [0]=lsp��[1]=��̬��,[2]=���ص�ַ
	int STACK_SIZE = 1000;
	int* dataStack = new int[STACK_SIZE];
	vector<PerPcode> pcode;
public:
	Interpreter()
	{
		for (int i = 0; i < STACK_SIZE; i++) dataStack[i] = 0;
	}
	Interpreter(AllPcode allPcode)
	{
		for (int i = 0; i < STACK_SIZE; i++) dataStack[i] = 0;
		pcode = allPcode.GetAllPcode();
	}
	void interpreter()
	{
		int sp = 0;//����ַ
		int top = 0;//ջ��
		int npc = 0;//�����������ָ����һ��ָ��
		do
		{
			PerPcode curPcode = pcode[npc];
			npc++;

			/*for (int i = 0; i < top; i++)
				cout << dataStack[i] << " ";
			cout << endl;

			string op;
			switch (curPcode.GetF())
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
			cout << op << " " << curPcode.GetL() << " " << curPcode.GetA() << endl;*/



			//cout << dataStack[top - 1] <<endl;

			if (curPcode.GetF() == Operator::LIT)
			{
				//LIT 0 a	������ֵȡ��ջ����aΪ����ֵ
				dataStack[top] = curPcode.GetA();
				top++;
			}
			else if (curPcode.GetF() == Operator::LOD)
			{
				//LOD l a ������ֵȡ��ջ����aΪƫ������lΪ���
				//���� lsp(����ַ) + a(ƫ����) ȡ��������ֵ
				dataStack[top] = dataStack[curPcode.GetA() + GetSL(sp, curPcode.GetL())];
				top++;
			}
			else if (curPcode.GetF() == Operator::STO)
			{
				//STO l a ��ջ����������ĳ������Ԫ�У�aΪƫ������lΪ���
				if (curPcode.GetL() == -1)
					dataStack[curPcode.GetA() + top - 1] = dataStack[top - 1];
				else
					dataStack[curPcode.GetA() + GetSL(sp, curPcode.GetL())] = dataStack[top - 1];
				top--;
			}
			else if (curPcode.GetF() == Operator::CAL)
			{
				//CAL�����ù��̣���ʱa��Ϊ�����ù��̵Ĺ����壨������֮ǰһ��ָ���Ŀ�����������ڵ�ַ��
				//��תʱ�����ò����ַ����ת�����ַ��npcָ�뱣����ջ��
				//����ַsp��Ϊ��ʱջ��top��npcָ��Ҫ��ת�ĵط�
				//����INT�������޸�topֵ
				dataStack[top] = sp;//����lsp
				dataStack[top + 1] = GetSL(sp, curPcode.GetL());//���澲̬��
				dataStack[top + 2] = npc;//���淵�ص�ַ
				sp = top;//����sp
				npc = curPcode.GetA();//npcָ����תĿ��
			}
			else if (curPcode.GetF() == Operator::INT)
			{
				//INT 0 a ������ջ��Ϊ�����õĹ��̿���a����Ԫ��������
				top += curPcode.GetA();
			}
			else if (curPcode.GetF() == Operator::JMP)
			{
				//JMP 0 a ��������ת��a��ַ
				npc = curPcode.GetA();
			}
			else if (curPcode.GetF() == Operator::JPC)
			{
				//JPC 0 a	������ת����ջ������ֵ��������ת��a��ַ������˳��ִ��
				if (dataStack[top - 1] == 0) npc = curPcode.GetA();
				top--;
			}
			else if (curPcode.GetF() == Operator::WRT)
			{
				//WRT 0 0	���ջ��
				cout << dataStack[top - 1];
				top--;
			}
			else if (curPcode.GetF() == Operator::RED) 
			{
				//RED 0 0 �������ж���һ����������ջ��
				cout << "������һ�����֣�int��:" << endl;
				int number;
				cin >> number;
				dataStack[top] = number;
				top++;
			}
			else if (curPcode.GetF() == Operator::OPR)
			{
				switch (curPcode.GetA())
				{
				case 0:
				{
					//OPR 0 0 ���̵��ý����󣬷��ص��õ㲢��ջ
					top = sp;
					npc = dataStack[sp + 2];//���ص�ַ
					sp = dataStack[sp];
					break;
				}
				case 1:
				{
					//OPR 0 1 ջ��Ԫ��ȡ��
					dataStack[top - 1] = -dataStack[top - 1];
					break;
				}
				case 2:
				{
					//OPR 0 2 ��ջ����ջ����ӣ�������ջԪ�أ����ֵ��ջ
					dataStack[top - 2] = dataStack[top - 1] + dataStack[top - 2];
					//cout << endl << dataStack[top - 2] << "+" << dataStack[top - 1] << endl;
					top--;
					break;
				}
				case 3:
				{
					//OPR 0 3 ��ջ����ȥջ����������ջԪ�أ����ֵ��ջ
					dataStack[top - 2] = dataStack[top - 2] - dataStack[top - 1];
					top--;
					break;
				}
				case 4:
				{
					//OPR 0 4 ��ջ������ջ����������ջԪ�أ����ֵ��ջ
					dataStack[top - 2] = dataStack[top - 1] * dataStack[top - 2];
					top--;
					break;
				}
				case 5:
				{
					//OPR 0 5 ��ջ������ջ����������ջԪ�أ����ֵ��ջ
					dataStack[top - 2] = dataStack[top - 2] / dataStack[top - 1];
					top--;
					break;
				}
				case 6:
				{
					//OPR 0 6 ջ��Ԫ�ص���ż�жϣ����ֵ��ջ��
					dataStack[top - 1] = dataStack[top - 1] % 2;
					break;
				}
				case 7:
				{
					//OPR 0 7 ��ջ����ջ���Ƿ���ȣ�������ջԪ�أ����ֵ��ջ
					dataStack[top - 2] = (dataStack[top - 1] == dataStack[top - 2]) ? 1 : 0;
					top--;
					break;
				}
				case 8:
				{
					//OPR 0 8	��ջ����ջ���Ƿ񲻵ȣ�������ջԪ�أ����ֵ��ջ
					dataStack[top - 2] = (dataStack[top - 1] != dataStack[top - 2]) ? 1 : 0;
					top--;
					break;
				}
				case 9:
				{
					//OPR 0 9	��ջ���Ƿ�С��ջ����������ջԪ�أ����ֵ��ջ
					dataStack[top - 2] = (dataStack[top - 1] > dataStack[top - 2]) ? 1 : 0;
					top--;
					break;
				}
				case 10:
				{
					//OPR 0 10 ��ջ���Ƿ���ڵ���ջ����������ջԪ�أ����ֵ��ջ
					dataStack[top - 2] = (dataStack[top - 1] <= dataStack[top - 2]) ? 1 : 0;
					top--;
					break;
				}
				case 11:
				{
					//OPR 0 11 ��ջ���Ƿ����ջ����������ջԪ�أ����ֵ��ջ
					dataStack[top - 2] = (dataStack[top - 1] < dataStack[top - 2]) ? 1 : 0;
					top--;
					break;
				}
				case 12:
				{
					//OPR 0 12 ��ջ���Ƿ�С�ڵ���ջ����������ջԪ�أ����ֵ��ջ
					dataStack[top - 2] = (dataStack[top - 1] >= dataStack[top - 2]) ? 1 : 0;
					top--;
					break;
				}
				case 13:
				{
					//OPR 0 13 ��Ļ�������
					cout << endl;
					break;
				}
				default:
					break;
				}
			}
		}while (npc != 0 && npc<pcode.size());
	}
	int GetSL(int sp, int level)
	{
		int lsp = sp;
		while (level > 0)
		{
			lsp = dataStack[sp+1];
			level--;
		}
		return lsp;
	}
};