#include "BinomialTree.h"
#include <cmath>
#include <iostream>

BinomialTree::BinomialTree(double volatility, double timestep, double SharePrice0, double rate, double expiryTime) :
	myvolatility(volatility), mytimestep(timestep), mySharePrice0(SharePrice0), myrate(rate)
{
	mydelta_t = expiryTime / timestep;
	myStepUp = exp(myvolatility*sqrt(mydelta_t));
	myStepDown = exp(-myvolatility * sqrt(mydelta_t));
	myProbability = (exp(myrate*mydelta_t) - myStepDown) / (myStepUp - myStepDown);
}

double BinomialTree::GetSharePrice(unsigned int time, unsigned int latticeIndex) const
{
	return mySharePrice0 * pow(myStepUp, latticeIndex) * pow(myStepDown, time - latticeIndex);
}

//The following codes are just for Asian option
BinomialTreeNode * BinomialTree::MakeTreeNode(const double& element, BinomialTreeNode * parent)
{
	BinomialTreeNode * Node = new BinomialTreeNode(element, parent);
	return Node;
}

