#pragma once
#include "European.h"
class EuropeanPut :
	public European
{
public:
	EuropeanPut(double interestrate, double strike, double expirytime, double volatility, double sharePrice0, double dividend, int timeStep);
	double EuropeanPayoff(double sharePrice);
	double Boundary();

	double PriceOptionAnalytically(void);
};

