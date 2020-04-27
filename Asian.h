#pragma once
#include"Option.h"
#include"BinomialTree.h"
#include"MonteCarlo.h"

class Asian :public Option
{
public:
	Asian(double interestrate, double strike, double expirytime, double volatility, double sharePrice0, double dividend, int timeStep);
	~Asian() {};

	// First pricing method: Binomial Tree
	virtual double AsianPayoff(double sharePrice) = 0; // AsianCallValue = max(AverageSharePrice - strike, 0)
	virtual double PriceAsianOptionWithBinomialTree();

	virtual double PriceAsianOptionWithMonteCarlo(int NumberOfSimulation);


protected:
	BinomialTree myBinomialTree;
	MonteCarlo MC;
};
