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
		Error(1, "program");//������ƴд����
		Advance();
	}
	else if (strToken != "program")
		Error(2, "program");//ȱ�ٱ�����
	else
		Advance();
	if (str_code != 2)
		Error(11, "");//ȱ��id
	else
		Id();
	if (strToken != ";" && !block_first.count(strToken) && strToken_next != "\EOF")
	{
		Error(12, strToken);//�����ַ�
		cout << "����ͬ���У�������";
		while (strToken != ";" && !block_first.count(strToken) && strToken_next != "\EOF")
		{
			cout << strToken << " ";
			Advance();
		}
		cout << endl << "ͬ������" << strToken << " ����������..." << endl;
	}
	if (strToken == ";")
	{
		Advance();
		Block();
	}
	else
	{
		Error(3, ";");//ȱ��;
		Block();
	}
	//if (strToken != "\EOF")
		//Error(10, "");//�������
	cout << "�������" << endl;
}
void Syntax_Analyzer::Block()
{
	//<block> -> [<condecl>][<vardecl>][<proc>]<body>
	
	//int dx_temp = dx;
	int start = allSymbol.GetIndex();//������������ĳ�ʼλ��
	int pos = 0;//������������ڷ��ű��е�λ��
	if (start > 0)
	{
		pos = allSymbol.GetLevelProc(level);
	}
	//������תָ������������֣��������
	int cx1 = allPcode.GetAllPcodePtr();//cx1��ʾJMPָ����Ҫ������ָ��е�λ��
	allPcode.Gen(Operator::JMP,0,0);


	if (str_code == 2)
	{
		//����block֮ǰ����block��ͷ��JMPָ��
		allPcode.SetA(cx1, allPcode.GetAllPcodePtr());
		allPcode.Gen(Operator::INT, 0, dx);//����ռ�
		if (start != 0)
		{
			//����������������Ҫ�ڷ��ű��address����ù�����Pcode�����е���ʼλ��
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
			cout << "����ͬ����������";
			while (!block_first.count(strToken) && strToken_next != "\EOF")
			{
				cout << strToken << " ";
				Advance();
			}
			cout << endl << "ͬ������" << strToken << "	����ͬ����..." << endl;
		}
		if (strToken == "const")
			Condecl();
		if (!block_first.count(strToken) && strToken_next != "\EOF")
		{
			Error(2, "var/procedure/begin");
			cout << "����ͬ����������";
			while (!block_first.count(strToken) && strToken_next != "\EOF")
			{
				cout << strToken << " ";
				Advance();
			}
			cout << endl << "ͬ������" << strToken << "	����ͬ����..." << endl;
		}
		if (strToken == "var")
			Vardecl();
		if (!block_first.count(strToken))
		{
			Error(2, "procedure/begin");
			cout << "����ͬ����������";
			while (!block_first.count(strToken))
			{
				cout << strToken << " ";
				Advance();
			}
			cout << endl << "ͬ������" << strToken << "	����ͬ����..." << endl;
		}

		if (strToken == "procedure")
			Proc();
		//����block֮ǰ����block��ͷ��JMPָ��

		//cout << allPcode.GetAllPcodePtr() << endl;

		//allPcode.GetAllPcode()[cx1].SetA(allPcode.GetAllPcodePtr());
		allPcode.SetA(cx1, allPcode.GetAllPcodePtr());

		//cout << allPcode.GetA(cx1) << endl;

		allPcode.Gen(Operator::INT, 0, dx);//����ռ�
		if (start != 0)
		{
			//����������������Ҫ�ڷ��ű��address����ù�����Pcode�����е���ʼλ��
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
		cout << "����ͬ����������";
		while (str_code != 2 && strToken != ";" && strToken != "var" && strToken != "procedure" && strToken != "begin")
		{
			cout << strToken << " ";
			Advance();
		}
		cout << endl << "ͬ������" << strToken << " ����������..." << endl;
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
			cout << "����ͬ���У�������";
			while (strToken != "var" && strToken != "procedure" && strToken != "begin" && strToken != ";")
			{
				cout << strToken << " ";
				Advance();
			}
			cout << endl << "ͬ������" << strToken << " ����������..." << endl;
		}
		while (str_code == 2 && strToken_next == ":=")//<const>֮��ȱ��","
		{
			Error(4, ",");
			Const();
		}
	}
	if (strToken == ";")//��������
		Advance();
	else if (strToken == "var" || strToken == "procedure" || strToken == "begin")//ȱ��;
		Error(3, ";");//ȱ��;
	else//ȱ��";"�Һ��ж����ַ�
	{
		Error(3, ";");//ȱ��;
		Error(12, strToken);//���ж����ַ�
		cout << "����ͬ���У�������";
		while (strToken != "var" && strToken != "procedure" && strToken != "begin")
		{
			cout << strToken << " ";
			Advance();
		}
		cout << endl << "ͬ������" << strToken << " ����������..." << endl;
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
				Error(15,name);//�ñ�ʶ���Ѵ���
			else
				allSymbol.EnterCon(value, level, dx, name);
			Advance();
		}
		else
			Error(13, "");
	}
	else
	{
		Error(9, "");//�Ƿ���ֵ
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
		cout << "����ͬ����������";
		while (str_code != 2 && strToken != ";" && strToken != "procedure" && strToken != "begin")
		{
			cout << strToken << " ";
			Advance();
		}
		cout << endl << "ͬ������" << strToken << " ����������..." << endl;
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
			cout << "����ͬ���У�������";
			while (strToken != "procedure" && strToken != "begin" && strToken != ";")
			{
				cout << strToken << " ";
				Advance();
			}
			cout << endl << "ͬ������" << strToken << " ����������..." << endl;
		}
		while (str_code == 2)//<id>֮��ȱ��","
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

	if (strToken == ";")//��������
		Advance();
	else if (strToken == "procedure" || strToken == "begin")//ȱ��;
		Error(3, ";");//ȱ��;
	else//ȱ��";"�Һ��ж����ַ�
	{
		Error(3, ";");//ȱ��;
		Error(12, strToken);//���ж����ַ�
		cout << "����ͬ���У�������";
		while (strToken != "procedure" && strToken != "begin")
		{
			cout << strToken << " ";
			Advance();
		}
		cout << endl << "ͬ������" << strToken << " ����������..." << endl;
	}
}
void Syntax_Analyzer::Proc()
{ 
	//procedure ����һ block Ϊͬһ�㼶
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
		cout << "����ͬ����������";
		while (str_code != 2 && strToken != "(" && strToken != ")" && strToken != ";" && strToken != "begin" && strToken_next != "\EOF")
		{
			cout << strToken << " ";
			Advance();
		}
		cout << endl << "ͬ������" << strToken << " ����������..." << endl;
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
		cout << "����ͬ����������";
		while (strToken != "(" && strToken != ")" && strToken != ";" && strToken != "begin" && strToken_next != "\EOF")
		{
			cout << strToken << " ";
			Advance();
		}
		cout << endl << "ͬ������" << strToken << " ����������..." << endl;
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
				Error(4, ",");//ȱ��,
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
			Error(5, "(");//ȱ��(
		if (strToken == ";")
			Error(6, ")");//ȱ��)
		else
			Advance();
		Advance();
		//level++;
		Block();
		dx = dx_temp;//�ָ�dx
		level--;//�ָ�level
		while (strToken == ";" && strToken_next == "procedure")
		{
			Advance();
			Proc();
		}
	}
	else if (block_first.count(strToken))
	{
		Error(5, "");//ȱ��(
		Error(6, "");//ȱ��)
		Error(3, "");//ȱ��;
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
		cout << "����ͬ����������";
		while (strToken != "begin" && !statement_first.count(strToken) && str_code != 2 && strToken_next != "\EOF")
		{
			cout << strToken << " ";
			Advance();
		}
		cout << endl << "ͬ������" << strToken << " ����������..." << endl;
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
		Error(7, "");//ȱ��begin
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
		cout << "����ͬ����������";
		while (strToken != "end" && strToken != "else" && strToken != ";" && strToken != "begin" && strToken_next != "\EOF")
		{
			cout << strToken << " ";
			Advance();
		}
		cout << endl << "ͬ������" << strToken << " ����������..." << endl;
	}
	if (strToken == "end")
	{
		Advance();
	}
	else
	{
		Error(8, "");//ȱ��end
		//Advance();
	}
	/*while (strToken != ";" && !statement_first.count(strToken) && strToken != "else" && strToken != "end")
	{
		Error(3, ";");//ȱ��;
		Statement();
	}
	if (strToken == "end")
		Advance();
	else
	{
		Error(8, "end");//ȱ��end
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
					 |read (<id>{��<id>})
					 |write (<exp>{,<exp>})*/
	string name;
	int jmp_pos = 0;//���ڻ���while�����ת
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
			Error(9, "");//�Ƿ���ֵ
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
			cout << "����ͬ����������";
			while (!statement_first.count(strToken) && strToken != "then" && strToken != "do" && strToken != "(" && strToken_next != "\EOF")
			{
				cout << strToken << " ";
				Advance();
			}
			cout << endl << "ͬ������" << strToken << " ����������..." << endl;
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

			int cx1 = allPcode.GetAllPcodePtr();//��ʾJPCָ����Ҫ������ָ��е�λ��
			allPcode.Gen(Operator::JPC, 0, 0);//if�������JPC��ת��else�ĵط� �������
			Statement();

			if (strToken == "else")
			{

				int cx2 = allPcode.GetAllPcodePtr();
				allPcode.Gen(Operator::JMP, 0, 0);
				//����if��䲻����ִ��else�ĵ�ַ
				allPcode.SetA(cx1, allPcode.GetAllPcodePtr());

				Advance();
				Statement();

				//����if�������ĵ�ַ
				//allPcode.GetAllPcode()[cx2].SetA(allPcode.GetAllPcodePtr());
				allPcode.SetA(cx2, allPcode.GetAllPcodePtr());
			}
			else
				allPcode.SetA(cx1, allPcode.GetAllPcodePtr());//����if��䲻����ִ��else�ĵ�ַ
		}
		else if (strToken == "do")
		{
			//while <lexp> do <statement>
			int pos = allPcode.GetAllPcodePtr();//��¼JPCָ�����λ��
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
				int _a = 3;//FLA��a��ֵ��Ĭ�ϴ�3��ʼ
				if (str_code == 2 && strToken_next == "(")
				{
					Id();
					Advance();
					Exp();

					//ÿ��Exp()�˳���һ������
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
			else if (tag == 5)//read (<id>{��<id>})
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
				allPcode.Gen(Operator::OPR, 0, 13);//������з�
				Advance();
			}
		}
	}
}
void Syntax_Analyzer::LExp()
{
	//<lexp> �� <exp> <lop> <exp>|odd <exp>
	if (strToken == "odd")
	{
		while (!exp_first.count(strToken) && str_code != 1 && str_code != 2)
			Advance();
		Exp();//Exp()�˳���õ��Ľ������ջ��
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
	//<exp> �� [+|-]<term>{<aop><term>}
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
	//<term> �� <factor>{<mop><factor>}
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
	//<factor>��<id>|<integer>|(<exp>)
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
			if (temp.GetType() == allSymbol.GetCon())//����
				allPcode.Gen(Operator::LIT, 0, temp.GetValue());
			else if (temp.GetType() == allSymbol.GetVar())//����
				allPcode.Gen(Operator::LOD, level - temp.GetLevel(), temp.GetAddress());
			else if (temp.GetType() == allSymbol.GetProc())//����
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
	//<lop> �� =|<>|<|<=|>|>=
	if (strToken == "=" || strToken == "<>" || strToken == "<" || strToken == "<=" || strToken == ">" || strToken == ">=")
		Advance();
}
void Syntax_Analyzer::Id()
{
	//<id> �� l{l|d}
	if (str_code == 2)
		Advance();
}
int Syntax_Analyzer::Error(int ecode, string str)
{
	ErrorTag = true;
	if (ecode == 1)//������ƴд����
		cout << "line:" << str_row << "	�ؼ���" << str << "ƴд����" << endl;
	else if (ecode == 2)//ȱ�ٱ�����
		cout << "line:" << str_row << "	ȱ�ٹؼ���:" << str << endl;
	else if (ecode == 3)//ȱ��;
		cout << "line:" << str_row - 1 << "	ȱ�� ; ��" << endl;
	else if (ecode == 4)//ȱ��,
		cout << "line:" << str_row << "	ȱ�� , ��" << endl;
	else if (ecode == 5)//ȱ��(
		cout << "line:" << str_row << "	ȱ�� ( ��" << endl;
	else if (ecode == 6)
		cout << "line:" << str_row << "	ȱ�� ) ��" << endl;
	else if (ecode == 7)
		cout << "line:" << str_row - 1 << "	ȱ�� begin ��" << endl;
	else if (ecode == 8)
		cout << "line:" << str_row << "	ȱ�� end ��" << endl;
	else if (ecode == 9)
		cout << "line:" << str_row << "	�Ƿ���ֵ�� = ��" << endl;
	else if (ecode == 10)//�������
		cout << "������룡" << endl;
	else if (ecode == 11)
		cout << "line:" << str_row << "	ȱ��id��" << endl;
	else if (ecode == 12)
		cout << "line:" << str_row << "	�����ַ�: " << str << endl;
	else if (ecode == 13)
		cout << "line:" << str_row << "	ȱ�����֣�" << endl;
	else if (ecode == 14)
		cout << "line:" << str_row << "	�ñ�ʶ����" << str << "	�Ѵ��ڣ�" << endl;
	else if (ecode == 15)
		cout << "line:" << str_row << "	�ñ�ʶ����" << str << "	�����ڣ�" << endl;
	else if (ecode == 16)
		cout << "line:" << str_row << "	�ñ�ʶ����" << str << "	����Var���ͣ�" << endl;
	else if (ecode == 17)
		cout << "line:" << str_row << "	���̣�" << str << "	δ���壡" << endl;
	else if(ecode == 18)
		cout << "line:" << str_row << "	������" << str << "	���ʹ���" << endl;
	else
	{
		cout << "��ֹ����" << endl;
		exit(0);
	}
}
void Syntax_Analyzer::Output()
{
	allPcode.Output();
}