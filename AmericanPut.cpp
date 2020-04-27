#include "AmericanPut.h"

AmericanPut::AmericanPut(double interestrate, double strike, double expirytime, double volatility, double sharePrice0, double dividend, int timeStep) :
	American(interestrate, strike, expirytime, volatility, sharePrice0, dividend, timeStep) { }

double AmericanPut::AmericanPayoff(double sharePrice, double OptionValue)
{
	if ( myStrike - sharePrice < OptionValue)
	{
		return (OptionValue);
	}
	else
	{
		return (myStrike - sharePrice);
	}
}

double AmericanPut::Boundary()
{
	return 0;
}

// For Monte Carlo
bool AmericanPut::if_share_exercise(double shareprice)
{
	return shareprice < myStrike ? true : false;
}

double AmericanPut::exercisenow(double shareprice0)
{
	return  myStrike - shareprice0;
}
