#pragma once
#include"Operator.h"
class PerPcode
{
private:
	Operator F;
	int L;
	int A;
public:
	PerPcode(Operator _F, int _L, int _A)
	{
		F = _F;
		L = _L;
		A = _A;
	}
	void SetF(Operator _F)
	{
		F = _F;
	}
	Operator GetF()
	{
		return F;
	}
	void SetL(int _L)
	{
		L = _L;
	}
	int GetL()
	{
		return L;
	}
	void SetA(int _A)
	{
		A = _A;
	}
	int GetA()
	{
		return A;
	}
};