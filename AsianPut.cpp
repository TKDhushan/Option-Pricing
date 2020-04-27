#include"AsianPut.h"

AsianPut::AsianPut(double interestrate, double strike, double expirytime, double volatility, double sharePrice0, double dividend, int timeStep) :
	Asian(interestrate, strike, expirytime, volatility, sharePrice0, dividend, timeStep) { }

double AsianPut::AsianPayoff(double sharePrice)
{
	if (myStrike > sharePrice)
	{
		return ( myStrike - sharePrice);
	}
	else
	{
		return 0;
	}
}