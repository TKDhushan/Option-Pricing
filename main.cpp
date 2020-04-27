#include<iostream>
#include"BinomialTree.h"
#include"Option.h"
#include"European.h"
#include"EuropeanCall.h"
#include"EuropeanPut.h"
#include"American.h"
#include"AmericanCall.h"
#include"AmericanPut.h"
#include"Asian.h"
#include"AsianCall.h"
#include"AsianPut.h"

using namespace std;

int main(void)
{
	double InterestRate = 0.04;
	double strike = 100;
	double volatility = 0.4;
	double expirytime = 1.0;
	int timestep = 22;
	double dividend = 0;
	double sharePrice = 100;
	
	cout << "EuropeanCall Price:" << endl;
	EuropeanCall A1(InterestRate, strike, expirytime, volatility, sharePrice, dividend, timestep);
	// To verify the result, I also use BS equation
	cout << "    BS:   " << A1.PriceOptionAnalytically() << endl;
	cout << "    Tree:  " << A1.PriceEuropeanOptionWithBinomialTree() << endl;
	cout << "    PDE:  " << A1.PriceEuropeanOptionWithPDE()[sharePrice] << endl;
	cout << "    MC:  " << A1.PriceEuropeanOptionWithMonteCarlo(10000) << endl;


	cout << endl;
	cout << "EuropeanPut Price:" << endl;
	EuropeanPut A2(InterestRate, strike, expirytime, volatility, sharePrice, dividend, timestep);
	cout << "    BS:   " << A2.PriceOptionAnalytically() << endl;
	cout << "    Tree:  " << A2.PriceEuropeanOptionWithBinomialTree() << endl;
	cout << "    PDE:  " << A2.PriceEuropeanOptionWithPDE()[sharePrice] << endl;
	cout << "    MC:  " << A2.PriceEuropeanOptionWithMonteCarlo(10000) << endl;

	cout << endl;
	cout << "AmericanCall Price:" << endl;
	AmericanCall USA1(InterestRate, strike, expirytime, volatility, sharePrice, dividend, timestep);
	cout << "    Tree:  " << USA1.PriceAmericanOptionWithBinomialTree() << endl;
	cout << "    PDE:  " << USA1.PriceAmericanOptionWithPDE()[sharePrice] << endl;
	//This MonteCarlo method can only be used when strike<sharePrice
	cout << "    MC:  " << USA1.PriceAmericanOptionWithMonteCarlo(10000) << endl;

	cout << endl;
	cout << "AmericanPut Price:" << endl;
	AmericanPut USA2(InterestRate, strike, expirytime, volatility, sharePrice, dividend, timestep);
	cout << "    Tree:  " << USA2.PriceAmericanOptionWithBinomialTree() << endl;
	cout << "    PDE:  " << USA2.PriceAmericanOptionWithPDE()[sharePrice] << endl;
	//This MonteCarlo method can only be used when strike>sharePrice
	cout << "    MC:  " << USA2.PriceAmericanOptionWithMonteCarlo(10000) << endl;


	cout << endl;
	cout << "AsianCall Price:" << endl;
	AsianCall AS1(InterestRate, strike, expirytime, volatility, sharePrice, dividend, timestep);
	cout << "    Tree:   " << AS1.PriceAsianOptionWithBinomialTree() << endl;
	cout << "    MC:  " << AS1.PriceAsianOptionWithMonteCarlo(10000) << endl;

	cout << endl;
	cout << "AsianPut Price:" << endl;
	AsianPut AS2(InterestRate, strike, expirytime, volatility, sharePrice, dividend, timestep);
	cout << "    Tree:   " << AS2.PriceAsianOptionWithBinomialTree() << endl;
	cout << "    MC:  " << AS2.PriceAsianOptionWithMonteCarlo(10000) << endl;

	system("pause");
	return 0;
}


