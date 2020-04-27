#include"European.h"
#include<vector>
#include<iostream>

European::European(double interestrate, double strike, double expirytime, double volatility, double sharePrice0, double dividend, int timeStep) :
	Option(interestrate, strike, expirytime, volatility, sharePrice0, dividend, timeStep) , myBinomialTree(volatility, timeStep, sharePrice0, interestrate,expirytime){}

double European::CDF_normal(double x)
{
	return 0.5 * erfc(-x / sqrt(2));
}

// First pricing method: Binomial Tree.Please see Part1 1.1 in PDF.
double European::PriceEuropeanOptionWithBinomialTree(void)
{
	std::vector<double> currentPrice(myBinomialTree.mytimestep + 1, 0);
	for (int i = 0; i < myBinomialTree.mytimestep + 1; ++i)
	{
		currentPrice[i] = EuropeanPayoff(myBinomialTree.GetSharePrice(myBinomialTree.mytimestep, i));
	}

	std::vector<double> previousPrices = currentPrice;
	for (int n = myBinomialTree.mytimestep - 1; n >= 0; n--)
	{
		for (int i = 0; i < n + 1; i++)
		{
			currentPrice[i] = (myBinomialTree.myProbability * previousPrices[i + 1] + (1 - myBinomialTree.myProbability) * previousPrices[i]) *exp(-myInterestRate * myBinomialTree.mydelta_t);
		}
		
		previousPrices = currentPrice;
	}
	return currentPrice[0];
}

// Second pricing method: PDE. Please see Part1 2.1 in PDF.
vector<double> European::PriceEuropeanOptionWithPDE(void)
{
	int numberOfTimeSteps = 100000 * myexpiryTime;
	double deltaT = myexpiryTime / numberOfTimeSteps;  //For time

	double domainSize = 200.0;  // S=[0,200]
	int numberOfSpatialElements = 201;
	double deltaX = domainSize / (numberOfSpatialElements - 1);  //For shareprice

	//The vector "initialFunctionValue" is the value of V when t is determined
	std::vector<double> initialFunctionValue(numberOfSpatialElements, 0.0);

	// boundary condition
	initialFunctionValue[0] = Boundary();
	initialFunctionValue.back() = Boundary();
	for (int i = 1; i < numberOfSpatialElements - 1; ++i)
	{
		initialFunctionValue[i] = EuropeanPayoff(deltaX*i);
	}

	//The iterative formula of this method is to first know the value of V at time t, and then find the value of V at time t-delta_t based on this.
	//Use vector "previousFunctionValue" to store the V value at time t, and calculate the V value at t-delta_t and store in "currentFunctionValue"
	std::vector<double> previousFunctionValue = initialFunctionValue;
	std::vector<double> currentFunctionValue = previousFunctionValue;

	double currentTime = myexpiryTime;

	while (currentTime > 0)
	{
		previousFunctionValue = currentFunctionValue;
		for (int i = 1; i < numberOfSpatialElements - 1; ++i)
		{
			currentFunctionValue[i] = currentFunctionValue[i] - deltaT * (myInterestRate*previousFunctionValue[i]
				- myInterestRate * i*(previousFunctionValue[i + 1] - previousFunctionValue[i])
				- myVolatility * myVolatility*i*i*(previousFunctionValue[i + 1] + previousFunctionValue[i - 1]
				- 2 * previousFunctionValue[i]) / 2);
		}
		currentTime -= deltaT;
	}
	return currentFunctionValue;
}

// Third pricing method: Monte Carlo.Please see Part1 3.1 in PDF.
double European::PriceEuropeanOptionWithMonteCarlo(int NumberOfSimulation)
{
	double SimulateSharePrice;
	double Sum_Of_Payoff_Of_Simulate = 0;
	for (int i = 0; i < NumberOfSimulation; i++)
	{
		SimulateSharePrice = mySharePrice * exp((myInterestRate - myVolatility * myVolatility / 2)*myexpiryTime + myVolatility * MC.MC()*sqrt(myexpiryTime));
		Sum_Of_Payoff_Of_Simulate += exp(-myInterestRate * myexpiryTime)*EuropeanPayoff(SimulateSharePrice);
	}
	double Average = Sum_Of_Payoff_Of_Simulate / NumberOfSimulation;
	return Average;
}


