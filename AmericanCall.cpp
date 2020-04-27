#include "AmericanCall.h"

AmericanCall::AmericanCall(double interestrate, double strike, double expirytime, double volatility, double sharePrice0, double dividend, int timeStep) :
	American(interestrate, strike, expirytime, volatility, sharePrice0, dividend, timeStep){ }

double AmericanCall::AmericanPayoff(double stockprice, double optionvalue)
{
	if (stockprice - myStrike <= optionvalue)
	{
		return (optionvalue);
	}
	else
	{
		return (stockprice - myStrike);
	}
}

double AmericanCall::Boundary()
{
	return myStrike;
}

bool AmericanCall::if_share_exercise(double shareprice)
{
	return shareprice > myStrike ? true : false;
}

double AmericanCall::exercisenow(double shareprice)
{
	return shareprice - myStrike;
}