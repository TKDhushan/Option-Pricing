#pragma once
#include"Asian.h"

class AsianCall :
	public Asian
{
public:
	AsianCall(double interestrate, double strike, double expirytime, double volatility, double sharePrice0, double dividend, int timeStep);
	double AsianPayoff(double sharePrice);
};

