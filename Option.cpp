#include"Option.h"

Option::Option(double interestrate, double strike, double expirytime, double volatility, double sharePrice0, double dividend, int timeStep) :
	myInterestRate(interestrate),
	myStrike(strike),
	myexpiryTime(expirytime),
	myVolatility(volatility),
	mySharePrice(sharePrice0),
	myDividendRate(dividend),
	mytimeSteps(timeStep)
{
}

Option::~Option()
{
}
