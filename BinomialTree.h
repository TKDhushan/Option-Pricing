#pragma once
#include"BinomialTreeNode.h"

class BinomialTree
{
public:
	BinomialTree(double volatility, double timestep, double SharePrice0, double rate,double expiryTime);
	double mytimestep;
	double myProbability;
	double mydelta_t;
	double myStepUp;
	double myStepDown;

	//double mDiscountFactorOneTimeStep;
	double GetSharePrice(unsigned int time, unsigned int latticeIndex) const;

	//The following codes are just for Asian option
	BinomialTreeNode * MakeTreeNode(const double& element, BinomialTreeNode * parent);

private:
	double myvolatility;
	double mySharePrice0;
	double myrate;
};
