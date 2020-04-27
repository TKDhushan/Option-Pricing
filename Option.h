#pragma once
#include <cmath>

class Option
{
public:
	Option(double interestrate, double strike, double expirytime, double volatility, double sharePrice0, double dividend, int timeStep);
	~Option();

protected:
	double const myInterestRate;
	double const myStrike;
	double const myexpiryTime;
	double const myVolatility;
	double const myDividendRate;
	double mySharePrice;
	int mytimeSteps;
};




