#pragma once
#include"Option.h"
#include"BinomialTree.h"
#include"MonteCarlo.h"

class European:public Option
{   
public:
	European(double interestrate, double strike, double expirytime, double volatility, double sharePrice0, double dividend, int timeStep);
	~European() {};

	virtual double EuropeanPayoff(double sharePrice) =0; // EuropeanCallValue = max(sharePrice - strike, 0)

	// First pricing method: Binomial Tree
	virtual double PriceEuropeanOptionWithBinomialTree(void);

	// Second pricing method: PDE
	virtual double Boundary()=0;
	virtual vector<double> PriceEuropeanOptionWithPDE(void);

	// Third pricing method: Monte Carlo
	double PriceEuropeanOptionWithMonteCarlo(int NumberOfSimulation);

protected:
	BinomialTree myBinomialTree;
	MonteCarlo MC;

	// parameters of Black-Scholes equation
	double CDF_normal(double x);
	double d1(void) { return 1.0 / myVolatility / sqrt(myexpiryTime) * (log(mySharePrice / myStrike) + (myInterestRate - myDividendRate + myVolatility * myVolatility / 2.0) * (myexpiryTime)); }
	double d2(void) { return d1() - myVolatility * sqrt(myexpiryTime); };
};
