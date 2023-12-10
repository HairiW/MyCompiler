#include"SynAnalyzer.h"
#include"AllPcode.h"
#include"AllSymbol.h"
#include<map>
#include<iostream>
#include<sstream>
#include<set>
#include<string>
#include<sstream>
using namespace std;

void Syntax_Analyzer::Advance()
{
	if (strToken_next == "\EOF")
		strToken = strToken;
	else
	{
		strToken = strToken_next;
		str_code = str_code_next;
		str_row = str_row_next;
		it++;
		if (it != allToken.end())
		{
			strToken_next = (*it).GetSt();
			str_code_next = (*it).GetType();
			str_row_next = (*it).GetLine();
		}
		else strToken_next = "\EOF";
	}
}
void Syntax_Analyzer::Prog()
{
	//<prog> -> program<id>;<block>
	Advance();
	if (strToken != "program" && str_code_next == 2)
	{
		Error(1, "program");//保留字拼写错误
		Advance();
	}
	else if (strToken != "program")
		Error(2, "program");//缺少保留字
	else
		Advance();
	if (str_code != 2)
		Error(11, "");//缺少id
	else
		Id();
	if (strToken != ";" && !block_first.count(strToken) && strToken_next != "\EOF")
	{
		Error(12, strToken);//多余字符
		cout << "正在同步中，跳过：";
		while (strToken != ";" && !block_first.count(strToken) && strToken_next != "\EOF")
		{
			cout << strToken << " ";
			Advance();
		}
		cout << endl << "同步至：" << strToken << " 继续编译中..." << endl;
	}
	if (strToken == ";")
	{
		Advance();
		Block();
	}
	else
	{
		Error(3, ";");//缺少;
		Block();
	}
	//if (strToken != "\EOF")
		//Error(10, "");//多余代码
	cout << "编译完成" << endl;
}
void Syntax_Analyzer::Block()
{
	//<block> -> [<condecl>][<vardecl>][<proc>]<body>
	
	//int dx_temp = dx;
	int start = allSymbol.GetIndex();//本层变量声明的初始位置
	int pos = 0;//本层过程声明在符号表中的位置
	if (start > 0)
	{
		pos = allSymbol.GetLevelProc(level);
	}
	//设置跳转指令，跳过声明部分，后面回填
	int cx1 = allPcode.GetAllPcodePtr();//cx1表示JMP指令需要回填在指令集中的位置
	allPcode.Gen(Operator::JMP,0,0);


	if (str_code == 2)
	{
		//进入block之前回填block开头的JMP指令
		allPcode.SetA(cx1, allPcode.GetAllPcodePtr());
		allPcode.Gen(Operator::INT, 0, dx);//申请空间
		if (start != 0)
		{
			//若非主函数，则需要在符号表的address填入该过程在Pcode代码中的起始位置
			allSymbol.GetAllSymbol()[pos].SetAddress(allPcode.GetAllPcodePtr() - 1);
		}
		Body();
		allPcode.Gen(Operator::OPR, 0, 0);
	}
	else
	{
		if (!block_first.count(strToken))
		{
			Error(2, "const/var/procedure/begin");
			cout << "正在同步，跳过：";
			while (!block_first.count(strToken) && strToken_next != "\EOF")
			{
				cout << strToken << " ";
				Advance();
			}
			cout << endl << "同步至：" << strToken << "	继续同步中..." << endl;
		}
		if (strToken == "const")
			Condecl();
		if (!block_first.count(strToken) && strToken_next != "\EOF")
		{
			Error(2, "var/procedure/begin");
			cout << "正在同步，跳过：";
			while (!block_first.count(strToken) && strToken_next != "\EOF")
			{
				cout << strToken << " ";
				Advance();
			}
			cout << endl << "同步至：" << strToken << "	继续同步中..." << endl;
		}
		if (strToken == "var")
			Vardecl();
		if (!block_first.count(strToken))
		{
			Error(2, "procedure/begin");
			cout << "正在同步，跳过：";
			while (!block_first.count(strToken))
			{
				cout << strToken << " ";
				Advance();
			}
			cout << endl << "同步至：" << strToken << "	继续同步中..." << endl;
		}

		if (strToken == "procedure")
			Proc();
		//进入block之前回填block开头的JMP指令

		//cout << allPcode.GetAllPcodePtr() << endl;

		//allPcode.GetAllPcode()[cx1].SetA(allPcode.GetAllPcodePtr());
		allPcode.SetA(cx1, allPcode.GetAllPcodePtr());

		//cout << allPcode.GetA(cx1) << endl;

		allPcode.Gen(Operator::INT, 0, dx);//申请空间
		if (start != 0)
		{
			//若非主函数，则需要在符号表的address填入该过程在Pcode代码中的起始位置
			allSymbol.GetAllSymbol()[pos].SetAddress(allPcode.GetAllPcodePtr() - 1);
		}
		Body();
		allPcode.Gen(Operator::OPR, 0, 0);
		//dx = dx_temp;
	}
}
void Syntax_Analyzer::Condecl()
{
	//<condecl> -> const <const>{,<const>};
	Advance();
	if (str_code != 2)
	{
		Error(11, "");
		cout << "正在同步，跳过：";
		while (str_code != 2 && strToken != ";" && strToken != "var" && strToken != "procedure" && strToken != "begin")
		{
			cout << strToken << " ";
			Advance();
		}
		cout << endl << "同步至：" << strToken << " 继续编译中..." << endl;
	}
	if (str_code == 2)
		Const();
	while (strToken != ";" && strToken != "var" && strToken != "procedure" && strToken != "begin")
	{
		while (strToken == "," && str_code_next == 2)
		{
			Advance();
			Const();
		}
		if (strToken == "," && str_code_next != 2)
		{
			Error(12, strToken);
			cout << "正在同步中，跳过：";
			while (strToken != "var" && strToken != "procedure" && strToken != "begin" && strToken != ";")
			{
				cout << strToken << " ";
				Advance();
			}
			cout << endl << "同步至：" << strToken << " 继续编译中..." << endl;
		}
		while (str_code == 2 && strToken_next == ":=")//<const>之间缺少","
		{
			Error(4, ",");
			Const();
		}
	}
	if (strToken == ";")//正常运行
		Advance();
	else if (strToken == "var" || strToken == "procedure" || strToken == "begin")//缺少;
		Error(3, ";");//缺少;
	else//缺少";"且含有多余字符
	{
		Error(3, ";");//缺少;
		Error(12, strToken);//含有多余字符
		cout << "正在同步中，跳过：";
		while (strToken != "var" && strToken != "procedure" && strToken != "begin")
		{
			cout << strToken << " ";
			Advance();
		}
		cout << endl << "同步至：" << strToken << " 继续编译中..." << endl;
	}
}
void Syntax_Analyzer::Const()
{
	//<const> -> <id>:=<integer>
	string name = strToken;
	Advance();
	if (strToken == ":=")
	{
		Advance();
		if (str_code == 1)
		{
			//string to int
			int value;
			istringstream ss(strToken);
			ss >> value;

			if (allSymbol.isCurExist(name, level))
				Error(15,name);//该标识符已存在
			else
				allSymbol.EnterCon(value, level, dx, name);
			Advance();
		}
		else
			Error(13, "");
	}
	else
	{
		Error(9, "");//非法赋值
		if (str_code_next == 1)
			Advance();
		if (str_code == 1)
			Advance();
		else
			Error(13, "");
	}
}
void Syntax_Analyzer::Vardecl()
{
	//<vardecl> -> var <id>{,<id>};
	string name;
	Advance();
	if (str_code != 2)
	{
		Error(11, "");
		cout << "正在同步，跳过：";
		while (str_code != 2 && strToken != ";" && strToken != "procedure" && strToken != "begin")
		{
			cout << strToken << " ";
			Advance();
		}
		cout << endl << "同步至：" << strToken << " 继续编译中..." << endl;
	}
	if (str_code == 2)
	{
		name = strToken;
		Id();
		if (allSymbol.isCurExist(name, level))
			Error(14, name);
		else
		{
			allSymbol.EnterVar(level, dx, name);
			dx++;
		}
	}
	while (strToken != "procedure" && strToken != "begin" && strToken != ";")
	{
		while (strToken == "," && str_code_next == 2)
		{
			Advance();
			if (allSymbol.isCurExist(strToken, level))
				Error(14, strToken);
			else
			{
				allSymbol.EnterVar(level, dx, strToken);
				dx++;
				Id();
			}
		}
		if (strToken == "," && str_code_next != 2)
		{
			Error(12, strToken);
			cout << "正在同步中，跳过：";
			while (strToken != "procedure" && strToken != "begin" && strToken != ";")
			{
				cout << strToken << " ";
				Advance();
			}
			cout << endl << "同步至：" << strToken << " 继续编译中..." << endl;
		}
		while (str_code == 2)//<id>之间缺少","
		{
			Error(4, ",");
			Id();
			if (allSymbol.isCurExist(name, level))
				Error(14, name);
			else
			{
				allSymbol.EnterVar(level, dx, name);
				dx++;
			}
		}
	}

	if (strToken == ";")//正常运行
		Advance();
	else if (strToken == "procedure" || strToken == "begin")//缺少;
		Error(3, ";");//缺少;
	else//缺少";"且含有多余字符
	{
		Error(3, ";");//缺少;
		Error(12, strToken);//含有多余字符
		cout << "正在同步中，跳过：";
		while (strToken != "procedure" && strToken != "begin")
		{
			cout << strToken << " ";
			Advance();
		}
		cout << endl << "同步至：" << strToken << " 继续编译中..." << endl;
	}
}
void Syntax_Analyzer::Proc()
{ 
	//procedure 与上一 block 为同一层级
	int dx_temp = dx;
	dx = 3;
	int para_cnt = 0;

	bool proc_id = false;
	bool l_bracket = false;
	//<proc> -> procedure <id> ([<id>{,<id>}]);<block>{;<proc>}
	Advance();
	if (str_code != 2)
	{
		Error(12, strToken);
		cout << "正在同步，跳过：";
		while (str_code != 2 && strToken != "(" && strToken != ")" && strToken != ";" && strToken != "begin" && strToken_next != "\EOF")
		{
			cout << strToken << " ";
			Advance();
		}
		cout << endl << "同步至：" << strToken << " 继续编译中..." << endl;
	}
	else
	{
		string name = strToken;
		Id();	
		if (allSymbol.isCurExist(name, level))
			Error(14, name);
		else
			allSymbol.EnterProc(level, allPcode.GetAllPcodePtr(), name);
		proc_id = true;
		level++;
	}
	if (proc_id == false && str_code == 2)
	{
		Id();
		proc_id = true;
	}
	else if (strToken != "(" && strToken != ")" && strToken != ";" && strToken != "begin" && strToken_next != "\EOF")
	{
		Error(12, strToken);
		cout << "正在同步，跳过：";
		while (strToken != "(" && strToken != ")" && strToken != ";" && strToken != "begin" && strToken_next != "\EOF")
		{
			cout << strToken << " ";
			Advance();
		}
		cout << endl << "同步至：" << strToken << " 继续编译中..." << endl;
	}
	if (strToken == "(")
	{
		l_bracket = true;
		Advance();
		while (strToken != ")" && strToken != ";" && !block_first.count(strToken) && strToken_next != "\EOF")
		{
			if (str_code == 2 && strToken_next == ",")
			{
				string name = strToken;
				Id();
				if (allSymbol.isCurExist(name, level))
					Error(14, name);
				else
				{
					allSymbol.EnterVar(level, dx, name);
					dx++;
					para_cnt++;
				}
				Advance();
			}
			else if (str_code == 2 && str_code_next == 2)
			{
				Error(4, ",");//缺少,
				string name = strToken;
				Id();
				if (allSymbol.isCurExist(name, level))
					Error(14, name);
				else
				{
					allSymbol.EnterVar(level, dx, name);
					dx++;
					para_cnt++;
				}
			}
			else if (strToken == ",")
			{
				Error(12, strToken);
				Advance();
			}
			else if (str_code == 2 && strToken_next == ")")
			{
				string name = strToken;
				Id();
				if (allSymbol.isCurExist(name, level))
					Error(14, name);
				else
				{
					allSymbol.EnterVar(level, dx, name);
					dx++;
					para_cnt++;
				}
			}
		}
	}
	if (strToken == ")" || strToken == ";")
	{
		if (l_bracket == false)
			Error(5, "(");//缺少(
		if (strToken == ";")
			Error(6, ")");//缺少)
		else
			Advance();
		Advance();
		//level++;
		Block();
		dx = dx_temp;//恢复dx
		level--;//恢复level
		while (strToken == ";" && strToken_next == "procedure")
		{
			Advance();
			Proc();
		}
	}
	else if (block_first.count(strToken))
	{
		Error(5, "");//缺少(
		Error(6, "");//缺少)
		Error(3, "");//缺少;
		Block();
		while (strToken == ";" && strToken_next == "procedure")
		{
			Advance();
			Proc();
		}
	}
}
void Syntax_Analyzer::Body()
{
	//<body> -> begin <statement>{;<statement>}end
	if (strToken != "begin" && statement_first.count(strToken_next) && strToken_next != "\EOF")
		Error(1, "begin");
	else if (strToken != "begin" && !statement_first.count(strToken) && str_code != 2 && strToken_next != "\EOF")
	{
		Error(12, strToken);
		cout << "正在同步，跳过：";
		while (strToken != "begin" && !statement_first.count(strToken) && str_code != 2 && strToken_next != "\EOF")
		{
			cout << strToken << " ";
			Advance();
		}
		cout << endl << "同步至：" << strToken << " 继续编译中..." << endl;
	}
	if (strToken == "begin")
	{
		Advance();
		Statement();
	}
	else if (strToken == "\EOF")
		return;
	else
	{
		Error(7, "");//缺少begin
		Statement();
	}
	while ((strToken == ";" && (statement_first.count(strToken_next) || str_code_next == 2)) || (statement_first.count(strToken) || ((strToken_next == ":=" || strToken_next == "=") && str_code == 2)))
	{
		while (strToken == ";" && (statement_first.count(strToken_next) || str_code_next == 2))
		{
			Advance();
			while (str_code == 2 || statement_first.count(strToken))
				Statement();
		}
		while (statement_first.count(strToken) || ((strToken_next == ":=" || strToken_next == "=") && str_code == 2))
		{
			Error(3, ";");
			Statement();
		}
	}
	if (strToken != "end" && strToken != "else" && strToken != ";" && strToken != "begin" && strToken_next != "\EOF")
	{
		Error(12, strToken);
		cout << "正在同步，跳过：";
		while (strToken != "end" && strToken != "else" && strToken != ";" && strToken != "begin" && strToken_next != "\EOF")
		{
			cout << strToken << " ";
			Advance();
		}
		cout << endl << "同步至：" << strToken << " 继续编译中..." << endl;
	}
	if (strToken == "end")
	{
		Advance();
	}
	else
	{
		Error(8, "");//缺少end
		//Advance();
	}
	/*while (strToken != ";" && !statement_first.count(strToken) && strToken != "else" && strToken != "end")
	{
		Error(3, ";");//缺少;
		Statement();
	}
	if (strToken == "end")
		Advance();
	else
	{
		Error(8, "end");//缺少end
		while (!body_follow.count(strToken))
			Advance();
	}*/
}
void Syntax_Analyzer::Statement()
{
	/*<statement> -> <id> : = <exp>
					 |if <lexp> then <statement>[else <statement>]
					 |while <lexp> do <statement>
					 |call <id> ([<exp>{,<exp>}])
					 |<body>
					 |read (<id>{，<id>})
					 |write (<exp>{,<exp>})*/
	string name;
	int jmp_pos = 0;//用于回填while语句跳转
	if (str_code == 2 && (strToken_next == ":=" || strToken_next == "="))
	{
		//<id> : = <exp>
		name = strToken;
		Id();
		if (strToken == ":=")
		{
			Advance();
			Exp();
			if (!allSymbol.isPreExist(name, level))
				Error(15, name);
			PerSymbol temp = allSymbol.GetSymbol(name);
			if (temp.GetType() == allSymbol.GetVar())
				allPcode.Gen(Operator::STO, level - temp.GetLevel(), temp.GetAddress());
			else
				Error(16, name);
		}
		else if (strToken == "=")
		{
			Error(9, "");//非法赋值
			Advance();
			Exp();
		}
	}
	else if (str_code == 2 || lexp_fisrt.count(strToken) || statement_first.count(strToken))
	{
		int tag = 0;
		if (!statement_first.count(strToken) && strToken != "then" && strToken != "do" && strToken != "(" && strToken_next != "\EOF")
		{
			Error(2, "");
			cout << "正在同步，跳过：";
			while (!statement_first.count(strToken) && strToken != "then" && strToken != "do" && strToken != "(" && strToken_next != "\EOF")
			{
				cout << strToken << " ";
				Advance();
			}
			cout << endl << "同步至：" << strToken << " 继续编译中..." << endl;
		}
		if (strToken == "if" || strToken == "while")
		{
			if (strToken == "if") tag = 1;
			else tag = 2;
			if (strToken == "while")
				jmp_pos = allPcode.GetAllPcodePtr();
			Advance();
			LExp();
		}
		else if (strToken == "call")
		{
			tag = 3;
			Advance();
			//Id();
		}
		else if (strToken == "begin")
		{
			tag = 4;
			//Advance();
			Body();
		}
		else if (strToken == "read" || strToken == "write")
		{
			if (strToken == "read") tag = 5;
			else tag = 6;
			Advance();
		}

		if (strToken == "then")
		{
			//if <lexp> then <statement>[else <statement>]
			if (!tag)
				Error(2, "if");
			Advance();
			while (str_code != 2 && !statement_first.count(strToken))
			{
				Error(12, strToken);
				Advance();
			}

			int cx1 = allPcode.GetAllPcodePtr();//表示JPC指令需要回填在指令集中的位置
			allPcode.Gen(Operator::JPC, 0, 0);//if语句先做JPC跳转到else的地方 后面回填
			Statement();

			if (strToken == "else")
			{

				int cx2 = allPcode.GetAllPcodePtr();
				allPcode.Gen(Operator::JMP, 0, 0);
				//回填if语句不满足执行else的地址
				allPcode.SetA(cx1, allPcode.GetAllPcodePtr());

				Advance();
				Statement();

				//回填if语句结束的地址
				//allPcode.GetAllPcode()[cx2].SetA(allPcode.GetAllPcodePtr());
				allPcode.SetA(cx2, allPcode.GetAllPcodePtr());
			}
			else
				allPcode.SetA(cx1, allPcode.GetAllPcodePtr());//回填if语句不满足执行else的地址
		}
		else if (strToken == "do")
		{
			//while <lexp> do <statement>
			int pos = allPcode.GetAllPcodePtr();//记录JPC指令回填位置
			allPcode.Gen(Operator::JPC, 0, 0);

			if (!tag)
				Error(2, "while");
			Advance();
			Statement();

			allPcode.Gen(Operator::JMP, 0, jmp_pos);
			//allPcode.GetAllPcode()[pos].SetA(allPcode.GetAllPcodePtr());
			allPcode.SetA(pos, allPcode.GetAllPcodePtr());
		}
		else if (strToken == "(" || statement_first.count(strToken) || str_code == 2)
		{
			if (!tag)
			{
				Error(12, strToken);
				return;
			}
			else if (tag == 1)
			{
				Error(2, "then");
				Statement();
				if (strToken == "else")
				{
					Advance();
					Statement();
				}
			}
			else if (tag == 2)
			{
				Error(2, "do");
				Statement();
			}
			else if (tag == 3)//call <id> ([<exp>{,<exp>}])
			{
				name = strToken;
				int _a = 3;//FLA中a的值，默认从3开始
				if (str_code == 2 && strToken_next == "(")
				{
					Id();
					Advance();
					Exp();

					//每次Exp()退出后传一个参数
					allPcode.Gen(Operator::STO, -1, _a);
					_a++;

					while (strToken != ")")
					{
						if (strToken == "," && (exp_first.count(strToken_next) || str_code_next == 2))
						{
							Advance();
							Exp();
							allPcode.Gen(Operator::STO, -1, _a);
							_a++;
						}
						else
						{
							Error(12, strToken);
							Advance();
						}
					}
					if (strToken == ")")
					{
						Advance();

						if (!allSymbol.isPreExist(name, level))
							Error(15, name);
						else if (allSymbol.isPreExist(name, level))
						{
							PerSymbol temp = allSymbol.GetSymbol(name);
							if(temp.GetType() == allSymbol.GetProc())
								allPcode.Gen(Operator::CAL, level - temp.GetLevel(), temp.GetAddress());
						}
						else Error(17, name);
					}
					else
						Error(6, ")");
				}
			}
			else if (tag == 4)
			{
				Error(7, "begin");
				Statement();
			}
			else if (tag == 5)//read (<id>{，<id>})
			{
				Advance();

				string name = strToken;
				if (!allSymbol.isPreExist(name, level))
					Error(15, name);
				else
				{
					PerSymbol temp = allSymbol.GetSymbol(name);
					if (temp.GetType() == allSymbol.GetVar())
					{
						allPcode.Gen(Operator::RED, 0, 0);
						allPcode.Gen(Operator::STO, level - temp.GetLevel(), temp.GetAddress());
					}
				}
				
				Id();
				if (strToken != ")")
				{
					while (strToken != ")")
					{
						if (strToken == "," && str_code_next == 2)
						{
							Advance();
							string name = strToken;
							if (!allSymbol.isPreExist(name, level))
								Error(15, name);
							else
							{
								PerSymbol temp = allSymbol.GetSymbol(name);
								if (temp.GetType() == allSymbol.GetVar())
								{
									allPcode.Gen(Operator::RED, 0, 0);
									allPcode.Gen(Operator::STO, level - temp.GetLevel(), temp.GetAddress());
								}
							}
							Id();
						}
						else if (str_code == 2 && strToken_next == ")")
						{
							string name = strToken;
							if (!allSymbol.isPreExist(name, level))
								Error(15, name);
							else
							{
								PerSymbol temp = allSymbol.GetSymbol(name);
								if (temp.GetType() == allSymbol.GetVar())
								{
									allPcode.Gen(Operator::RED, 0, 0);
									allPcode.Gen(Operator::STO, level - temp.GetLevel(), temp.GetAddress());
								}
							}
							Id();
						}
						else
						{
							Error(12, strToken);
							Advance();
						}
					}
				}
				Advance();
			}
			else if (tag == 6)//write (<exp>{,<exp>})
			{
				Advance();
				Exp();
				allPcode.Gen(Operator::WRT, 0, 0);
				if (strToken != ")")
				{
					while (strToken != ")")
					{
						if (strToken == "," && (exp_first.count(strToken_next) || str_code_next == 2))
						{
							Advance();
							Exp();
							allPcode.Gen(Operator::WRT, 0, 0);
						}
						else
						{
							Error(12, strToken);
							Advance();
						}
					}
				}
				allPcode.Gen(Operator::OPR, 0, 13);//输出换行符
				Advance();
			}
		}
	}
}
void Syntax_Analyzer::LExp()
{
	//<lexp> → <exp> <lop> <exp>|odd <exp>
	if (strToken == "odd")
	{
		while (!exp_first.count(strToken) && str_code != 1 && str_code != 2)
			Advance();
		Exp();//Exp()退出后得到的结果放在栈顶
		allPcode.Gen(Operator::OPR, 0, 6);
	}
	else
	{
		Exp();
		while (!exp_follow.count(strToken))
		{
			Advance();
		}
		string op = strToken;
		Lop();
		while (!lop_follow.count(strToken) && str_code != 1 && str_code != 2)
		{
			Advance();
		}
		Exp();

		if (op == "=") allPcode.Gen(Operator::OPR, 0, 7);
		else if (op == "<>") allPcode.Gen(Operator::OPR, 0, 8);
		else if (op == "<") allPcode.Gen(Operator::OPR, 0, 9);
		else if (op == "<=")	allPcode.Gen(Operator::OPR, 0, 12);
		else if (op == ">")	allPcode.Gen(Operator::OPR, 0, 11);
		else if (op == ">=")	allPcode.Gen(Operator::OPR, 0, 10);
		else Error(12, op);
	}
}
void Syntax_Analyzer::Exp()
{
	//<exp> → [+|-]<term>{<aop><term>}
	string sym;
	while (!exp_first.count(strToken) && str_code != 1 && str_code != 2)
		Advance();
	sym = strToken;
	if (strToken == "+" || strToken == "-")
		Advance();
	Term();
	if (sym == "-")
		allPcode.Gen(Operator::OPR, 0, 1);
	while (strToken == "+" || strToken == "-")
	{
		sym = strToken;
		Advance();
		Term();
		if (sym == "+")
			allPcode.Gen(Operator::OPR, 0, 2);
		else if (sym == "-")
			allPcode.Gen(Operator::OPR, 0, 3);
	}
}
void Syntax_Analyzer::Term()
{
	//<term> → <factor>{<mop><factor>}
	string sym;
	while (str_code != 1 && str_code != 2 && strToken != "(")
		Advance();
	Factor();
	while (strToken == "*" || strToken == "/")
	{
		sym = strToken;
		Advance();
		Factor();
		if (sym == "*")
			allPcode.Gen(Operator::OPR, 0, 4);
		else if (sym == "/")
			allPcode.Gen(Operator::OPR, 0, 5);
	}
}
void Syntax_Analyzer::Factor()
{
	//<factor>→<id>|<integer>|(<exp>)
	string name;
	while (str_code != 1 && str_code != 2 && strToken != "(")
		Advance();
	if (str_code == 2)//<id>
	{
		name = strToken;
		Id();
		if (!allSymbol.isPreExist(name, level))
			Error(15, name);
		else
		{
			PerSymbol temp = allSymbol.GetSymbol(name);
			if (temp.GetType() == allSymbol.GetCon())//常量
				allPcode.Gen(Operator::LIT, 0, temp.GetValue());
			else if (temp.GetType() == allSymbol.GetVar())//变量
				allPcode.Gen(Operator::LOD, level - temp.GetLevel(), temp.GetAddress());
			else if (temp.GetType() == allSymbol.GetProc())//过程
				Error(18, name);
		}
	}
	else if (str_code == 1)//<integer>
	{
		int value;
		istringstream ss(strToken);
		ss >> value;
		allPcode.Gen(Operator::LIT, 0, value);
		Advance();
	}
	else if (strToken == "(")//(<exp>)
	{
		Advance();
		Exp();
	}
}
void Syntax_Analyzer::Lop()
{
	//<lop> → =|<>|<|<=|>|>=
	if (strToken == "=" || strToken == "<>" || strToken == "<" || strToken == "<=" || strToken == ">" || strToken == ">=")
		Advance();
}
void Syntax_Analyzer::Id()
{
	//<id> → l{l|d}
	if (str_code == 2)
		Advance();
}
int Syntax_Analyzer::Error(int ecode, string str)
{
	ErrorTag = true;
	if (ecode == 1)//保留字拼写错误
		cout << "line:" << str_row << "	关键字" << str << "拼写错误！" << endl;
	else if (ecode == 2)//缺少保留字
		cout << "line:" << str_row << "	缺少关键字:" << str << endl;
	else if (ecode == 3)//缺少;
		cout << "line:" << str_row - 1 << "	缺少 ; ！" << endl;
	else if (ecode == 4)//缺少,
		cout << "line:" << str_row << "	缺少 , ！" << endl;
	else if (ecode == 5)//缺少(
		cout << "line:" << str_row << "	缺少 ( ！" << endl;
	else if (ecode == 6)
		cout << "line:" << str_row << "	缺少 ) ！" << endl;
	else if (ecode == 7)
		cout << "line:" << str_row - 1 << "	缺少 begin ！" << endl;
	else if (ecode == 8)
		cout << "line:" << str_row << "	缺少 end ！" << endl;
	else if (ecode == 9)
		cout << "line:" << str_row << "	非法赋值号 = ！" << endl;
	else if (ecode == 10)//多余代码
		cout << "多余代码！" << endl;
	else if (ecode == 11)
		cout << "line:" << str_row << "	缺少id！" << endl;
	else if (ecode == 12)
		cout << "line:" << str_row << "	错误字符: " << str << endl;
	else if (ecode == 13)
		cout << "line:" << str_row << "	缺少数字！" << endl;
	else if (ecode == 14)
		cout << "line:" << str_row << "	该标识符：" << str << "	已存在！" << endl;
	else if (ecode == 15)
		cout << "line:" << str_row << "	该标识符：" << str << "	不存在！" << endl;
	else if (ecode == 16)
		cout << "line:" << str_row << "	该标识符：" << str << "	不是Var类型！" << endl;
	else if (ecode == 17)
		cout << "line:" << str_row << "	过程：" << str << "	未定义！" << endl;
	else if(ecode == 18)
		cout << "line:" << str_row << "	参数：" << str << "	类型错误！" << endl;
	else
	{
		cout << "终止编译" << endl;
		exit(0);
	}
}
void Syntax_Analyzer::Output()
{
	allPcode.Output();
}