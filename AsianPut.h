#pragma once
#include"Asian.h"

class AsianPut :public Asian
{
public:
	AsianPut(double interestrate, double strike, double expirytime, double volatility, double sharePrice0, double dividend, int timeStep);
	double AsianPayoff(double sharePrice);
};