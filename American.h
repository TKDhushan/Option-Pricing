#pragma once
#include"Option.h"
#include"BinomialTree.h"
#include"MonteCarlo.h"
#include<vector>

class American :public Option
{
public:
	American(double interestrate, double strike, double expirytime, double volatility, double SharePrice0, double dividend, int timeStep);
	~American() {};

	// First pricing method: Binomial Tree
	virtual double AmericanPayoff(double sharePrice, double OptionValue)=0;  // AmericanCallValue = max(sharePrice - strike, OptionValue)
	virtual double PriceAmericanOptionWithBinomialTree(void);

	// Second pricing method: PDE
	virtual double Boundary() = 0;
	virtual std::vector<double> PriceAmericanOptionWithPDE(void);

	// Third pricing method: Monte Carlo
	virtual bool if_share_exercise(double shareprice) = 0;
	virtual double exercisenow(double shareprice) = 0;
	virtual double PriceAmericanOptionWithMonteCarlo(int NumberOfSimulation);

protected:
	BinomialTree myBinomialTree;
	MonteCarlo MC;
};
