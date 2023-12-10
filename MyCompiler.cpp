#include"AllPcode.h"
#include"AllSymbol.h"
#include"SynAnalyzer.h"
#include<fstream>
using namespace std;
int main()
{
	string filePath, resultPath;
	filePath = "origin_code.txt";
	resultPath = "result_code.txt";
	Syntax_Analyzer sa(filePath, resultPath);
	sa.Prog();
	sa.Output();
	return 0;
}