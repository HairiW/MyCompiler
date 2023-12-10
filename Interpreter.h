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
	//活动记录 [0]=lsp，[1]=静态链,[2]=返回地址
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
		int sp = 0;//基地址
		int top = 0;//栈顶
		int npc = 0;//程序计数器，指向下一条指令
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
				//LIT 0 a	将常数值取到栈顶，a为常数值
				dataStack[top] = curPcode.GetA();
				top++;
			}
			else if (curPcode.GetF() == Operator::LOD)
			{
				//LOD l a 将变量值取到栈顶，a为偏移量，l为层差
				//根据 lsp(基地址) + a(偏移量) 取出变量的值
				dataStack[top] = dataStack[curPcode.GetA() + GetSL(sp, curPcode.GetL())];
				top++;
			}
			else if (curPcode.GetF() == Operator::STO)
			{
				//STO l a 将栈顶内容送入某变量单元中，a为偏移量，l为层差
				if (curPcode.GetL() == -1)
					dataStack[curPcode.GetA() + top - 1] = dataStack[top - 1];
				else
					dataStack[curPcode.GetA() + GetSL(sp, curPcode.GetL())] = dataStack[top - 1];
				top--;
			}
			else if (curPcode.GetF() == Operator::CAL)
			{
				//CAL：调用过程，这时a段为被调用过程的过程体（过程体之前一条指令）在目标程序区的入口地址。
				//跳转时，将该层基地址，跳转层基地址，npc指针保存在栈中
				//基地址sp变为此时栈顶top，npc指向要跳转的地方
				//产生INT的语句会修改top值
				dataStack[top] = sp;//保存lsp
				dataStack[top + 1] = GetSL(sp, curPcode.GetL());//保存静态链
				dataStack[top + 2] = npc;//保存返回地址
				sp = top;//更新sp
				npc = curPcode.GetA();//npc指向跳转目标
			}
			else if (curPcode.GetF() == Operator::INT)
			{
				//INT 0 a 在运行栈中为被调用的过程开辟a个单元的数据区
				top += curPcode.GetA();
			}
			else if (curPcode.GetF() == Operator::JMP)
			{
				//JMP 0 a 无条件跳转至a地址
				npc = curPcode.GetA();
			}
			else if (curPcode.GetF() == Operator::JPC)
			{
				//JPC 0 a	条件跳转，当栈顶布尔值非真则跳转至a地址，否则顺序执行
				if (dataStack[top - 1] == 0) npc = curPcode.GetA();
				top--;
			}
			else if (curPcode.GetF() == Operator::WRT)
			{
				//WRT 0 0	输出栈顶
				cout << dataStack[top - 1];
				top--;
			}
			else if (curPcode.GetF() == Operator::RED) 
			{
				//RED 0 0 从命令行读入一个输入置于栈顶
				cout << "请输入一个数字（int）:" << endl;
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
					//OPR 0 0 过程调用结束后，返回调用点并退栈
					top = sp;
					npc = dataStack[sp + 2];//返回地址
					sp = dataStack[sp];
					break;
				}
				case 1:
				{
					//OPR 0 1 栈顶元素取反
					dataStack[top - 1] = -dataStack[top - 1];
					break;
				}
				case 2:
				{
					//OPR 0 2 次栈顶与栈顶相加，退两个栈元素，结果值进栈
					dataStack[top - 2] = dataStack[top - 1] + dataStack[top - 2];
					//cout << endl << dataStack[top - 2] << "+" << dataStack[top - 1] << endl;
					top--;
					break;
				}
				case 3:
				{
					//OPR 0 3 次栈顶减去栈顶，退两个栈元素，结果值进栈
					dataStack[top - 2] = dataStack[top - 2] - dataStack[top - 1];
					top--;
					break;
				}
				case 4:
				{
					//OPR 0 4 次栈顶乘以栈顶，退两个栈元素，结果值进栈
					dataStack[top - 2] = dataStack[top - 1] * dataStack[top - 2];
					top--;
					break;
				}
				case 5:
				{
					//OPR 0 5 次栈顶除以栈顶，退两个栈元素，结果值进栈
					dataStack[top - 2] = dataStack[top - 2] / dataStack[top - 1];
					top--;
					break;
				}
				case 6:
				{
					//OPR 0 6 栈顶元素的奇偶判断，结果值在栈顶
					dataStack[top - 1] = dataStack[top - 1] % 2;
					break;
				}
				case 7:
				{
					//OPR 0 7 次栈顶与栈顶是否相等，退两个栈元素，结果值进栈
					dataStack[top - 2] = (dataStack[top - 1] == dataStack[top - 2]) ? 1 : 0;
					top--;
					break;
				}
				case 8:
				{
					//OPR 0 8	次栈顶与栈顶是否不等，退两个栈元素，结果值进栈
					dataStack[top - 2] = (dataStack[top - 1] != dataStack[top - 2]) ? 1 : 0;
					top--;
					break;
				}
				case 9:
				{
					//OPR 0 9	次栈顶是否小于栈顶，退两个栈元素，结果值进栈
					dataStack[top - 2] = (dataStack[top - 1] > dataStack[top - 2]) ? 1 : 0;
					top--;
					break;
				}
				case 10:
				{
					//OPR 0 10 次栈顶是否大于等于栈顶，退两个栈元素，结果值进栈
					dataStack[top - 2] = (dataStack[top - 1] <= dataStack[top - 2]) ? 1 : 0;
					top--;
					break;
				}
				case 11:
				{
					//OPR 0 11 次栈顶是否大于栈顶，退两个栈元素，结果值进栈
					dataStack[top - 2] = (dataStack[top - 1] < dataStack[top - 2]) ? 1 : 0;
					top--;
					break;
				}
				case 12:
				{
					//OPR 0 12 次栈顶是否小于等于栈顶，退两个栈元素，结果值进栈
					dataStack[top - 2] = (dataStack[top - 1] >= dataStack[top - 2]) ? 1 : 0;
					top--;
					break;
				}
				case 13:
				{
					//OPR 0 13 屏幕输出换行
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