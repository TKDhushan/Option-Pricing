#include"American.h"
#include<vector>
#include<iostream>
using namespace std;

American::American(double interestrate, double strike, double expirytime, double volatility, double SharePrice0, double dividend, int timeStep) :
	Option(interestrate, strike, expirytime, volatility, SharePrice0, dividend, timeStep), myBinomialTree(volatility, timeStep, SharePrice0, interestrate, expirytime) {}

// First pricing method: Binomial Tree.Please see Part1 1.2 in PDF.
double American::PriceAmericanOptionWithBinomialTree(void)
{
	std::vector<double> currentPrice(myBinomialTree.mytimestep + 1, 0);
	for (int i = 0; i < myBinomialTree.mytimestep + 1; ++i)
	{
		currentPrice[i] = AmericanPayoff(myBinomialTree.GetSharePrice(myBinomialTree.mytimestep, i), currentPrice[i]);
	}

	std::vector<double> previousPrice = currentPrice;
	for (int n = myBinomialTree.mytimestep - 1; n >= 0; n--)
	{
		for (int i = 0; i < n + 1; i++)
		{
			double NewOptionPrice = (myBinomialTree.myProbability * previousPrice[i + 1] + (1 - myBinomialTree.myProbability) * previousPrice[i]) *exp(-myInterestRate * myBinomialTree.mydelta_t);
			currentPrice[i] = AmericanPayoff(myBinomialTree.GetSharePrice(n, i), NewOptionPrice);
		}
		previousPrice = currentPrice;
	}
	return currentPrice[0];
}

// Second pricing method: PDE. Please see Part1 2.2 in PDF.
std::vector<double> American::PriceAmericanOptionWithPDE(void)
{
	int numberOfTimeSteps = 100000 * myexpiryTime;
	double deltaT = myexpiryTime / numberOfTimeSteps;
	double domainSize = 200.0;  // S=[0,200]
	const int numberOfSpatialElements = 201;
	const double deltaX = domainSize / (numberOfSpatialElements - 1);

	std::vector<double> initialFunctionValue(numberOfSpatialElements, 0.0);
	initialFunctionValue[0] = Boundary();
	initialFunctionValue.back() = Boundary();

	for (int i = 1; i < numberOfSpatialElements - 1; ++i)
	{
		initialFunctionValue[i] = AmericanPayoff(deltaX*i,0);
	}
	std::vector<double> previousFunctionValue = initialFunctionValue;
	std::vector<double> currentFunctionValue = previousFunctionValue;

	double currentTime = myexpiryTime;

	while (currentTime > 0)
	{
		previousFunctionValue = currentFunctionValue;

		for (int i = 1; i < numberOfSpatialElements - 1; ++i)
		{
			double optionValue = currentFunctionValue[i] - deltaT * (myInterestRate*previousFunctionValue[i]
				- myInterestRate * i*(previousFunctionValue[i + 1] - previousFunctionValue[i])
				- myVolatility * myVolatility*i*i*(previousFunctionValue[i + 1] + previousFunctionValue[i - 1] - 2 * previousFunctionValue[i]) / 2);
			
			// Here is the only difference between European option and American option.
			currentFunctionValue[i] = AmericanPayoff(deltaX*i, optionValue);
		}
		currentTime -= deltaT;
	}
	return currentFunctionValue;
}

// Third pricing method: Least Square Monte Carlo. Please see Part1 3.2 in PDF.

//This function is to calculate the parameters of Least Square 
vector<double> LSparameters(vector<double>y, vector<double>x2, vector<double>x3)
{
	double sumy = 0, sumx2 = 0, sumx3 = 0;
	for (int i = 0; i < y.size(); i++)
	{
		sumy += y[i];
		sumx2 += x2[i];
		sumx3 += x3[i];
	}
	double average_y = sumy / y.size(), average_x2 = sumx2 / x2.size(), average_x3 = sumx3 / x3.size();

	for (int i = 0; i < y.size(); i++)
	{
		y[i] -= average_y;
		x2[i] -= average_x2;
		x3[i] -= average_x3;
	}

	double t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0;
	for (int i = 0; i < y.size(); i++)
	{
		t1 += y[i] * x2[i];
		t2 += x3[i] * x3[i];
		t3 += y[i] * x3[i];
		t4 += x2[i] * x3[i];
		t5 += x2[i] * x2[i];
	}

	double b2 = (t1*t2 - t3 * t4) / (t5*t2 - t4 * t4);
	double b3 = (t3*t5 - t1 * t4) / (t5*t2 - t4 * t4);
	double b1 = average_y - b2 * average_x2 - b3 * average_x3;

	vector<double> parameters = { b1,b2,b3 };
	return parameters;
}


double American::PriceAmericanOptionWithMonteCarlo(int NumberOfSimulation)
{
	//First step: Generate simulation paths, storing in a matrix.
	int M = NumberOfSimulation;
	int N = 365* myexpiryTime;
	double delta_t = myexpiryTime / N;
	
	vector<vector<double>> matrix;
	for (int i = 0; i < M; i++)
	{
		double simulate_everydayPrice = mySharePrice;
		vector<double> v;
		for (int j = 0; j <= N; j++)
		{
			simulate_everydayPrice = simulate_everydayPrice * exp((myInterestRate - myVolatility * myVolatility / 2)*delta_t + myVolatility * MC.MC()*sqrt(delta_t));
			v.push_back(simulate_everydayPrice);
		}
		matrix.push_back(v);
	}

	//The second step.This is the complicated core, you can get explanation in pdf.
	vector<double> y;
	vector<double> x2; 
	vector<double> x3;
	vector<double> besttime(M, N);

	for (int j = N-1; j >= 0; j--)
	{
		for (int i = 0; i < M; i++)
		{
			if (if_share_exercise(matrix[i][j]))
			{
				y.push_back(AmericanPayoff(matrix[i][besttime[i]], 0)*exp(-myInterestRate * delta_t*(besttime[i]-j)));
				x2.push_back(matrix[i][j]);
				x3.push_back(pow(matrix[i][j], 2));
			}
		}
		double b1 = LSparameters(y, x2, x3)[0];
		double b2 = LSparameters(y, x2, x3)[1];
		double b3 = LSparameters(y, x2, x3)[2];

		for (int i = 0; i < M; i++)
		{
			double expectValue = b1 + b2 * matrix[i][j] + b3 * pow(matrix[i][j], 2);
			if (exercisenow(matrix[i][j]) > expectValue)
			{
				besttime[i] = j;    // choose which time we should exercise the option
			}
		}

		x2.clear();
		x3.clear();
		y.clear();
	}
	
	// The third step: calculate the average of all paths
	double sum = 0;
	for (int i = 0; i < M; i++)
	{
		sum += AmericanPayoff(matrix[i][besttime[i]], 0)*exp(-myInterestRate * delta_t*(besttime[i]));
	}

	double OptionPrice = sum / M;
	return OptionPrice;
}