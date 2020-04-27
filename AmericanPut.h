#pragma once
#include "American.h"
class AmericanPut :
	public American
{
public:
	AmericanPut(double interestrate, double strike, double expirytime, double volatility, double sharePrice0, double dividend, int timeStep);
	double AmericanPayoff(double sharePrice, double OptionValue);
	double Boundary();
	bool if_share_exercise(double shareprice);
	double exercisenow(double shareprice);
};
