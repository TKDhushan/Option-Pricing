#include"Asian.h"
#include"MonteCarlo.h"
#include<vector>
#include<iostream>


Asian::Asian(double interestrate, double strike, double expirytime, double volatility, double sharePrice0, double dividend, int timeStep) :
	Option(interestrate, strike, expirytime, volatility, sharePrice0, dividend, timeStep), myBinomialTree(volatility, timeStep, sharePrice0, interestrate, expirytime) {}

// First pricing method: Binomial Tree. Please see Part1 1.3 in PDF.
double Asian::PriceAsianOptionWithBinomialTree()
{
	//This function is processed by layer, after processing all nodes of one layer, then process the next layer
	//First step, generate a binary tree

	BinomialTreeNode * TreeRoot = myBinomialTree.MakeTreeNode(mySharePrice, 0);  //set share price as "root" of the tree 
	vector<BinomialTreeNode *> Nodes = { TreeRoot };  //Vector "Nodes" are used to store the nodes
	vector<BinomialTreeNode *> PreviousNodes = Nodes;
	//"Nodes" will be iterated later, so use vector "PreviousNodes" as a copy of "Nodes"

	for (int i = 0; i < mytimeSteps; i++)    // i is the number of levels of the binary tree, and iterates "mytimeSteps" times
	{
		Nodes.clear();    // The "Nodes" vector is a container that is used to store the current lowest-level binary tree nodes, so it must be emptied every loop.

		//"PreviousNodes" stores the current layer of nodes, loop through each node in PreviousNodes. Let each note generates two left and right children nodes, these newly generated nodes are stored in vector Nodes
		vector<BinomialTreeNode *>::iterator it = PreviousNodes.begin();
		for (it; it != PreviousNodes.end(); it++)
		{
			BinomialTreeNode *currentNode = *it;   //currentNode is the current node and will generate two nodes on the left and right, and put it into "Nodes"

			BinomialTreeNode * LeftNode = myBinomialTree.MakeTreeNode(currentNode->data*myBinomialTree.myStepDown, currentNode);
			BinomialTreeNode * RightNode = myBinomialTree.MakeTreeNode(currentNode->data*myBinomialTree.myStepUp, currentNode);

			Nodes.push_back(LeftNode);
			Nodes.push_back(RightNode);
		}
		PreviousNodes = Nodes;
	}

	// At this time, all the nodes in the lowest binary tree are in the "previousnodes". Next, we enter the second step. Let's find the average value of the path corresponding to each node;
	// The basic idea is that, for each node at the bottom, through the pointer, use the "for" loop to continuously find the node at the upper layer and add up to the root

	vector<double> Payoff_of_AsianOption;
	for (int i = 0; i < pow(2, mytimeSteps); i++)    //This "for" loop is traversing every point of "previousnodes"
	{
		double sum = 0;   //Each point of the "previousnodes" must be traced upwards, and the value of all nodes on the path is accumulated, and the sum is stored in the "sum"
		BinomialTreeNode * current = PreviousNodes[i];
		for (int j = mytimeSteps; j >= 0; j--)   //This loop is to use "Parent" pointer to trace back to the upper layer and sum
		{
			sum += current->data;
			current = current->Parent;
		}
		double average = sum / (mytimeSteps + 1);
		Payoff_of_AsianOption.push_back(AsianPayoff(average));
	}

	//The third step, find the corresponding option value when t = 0
	//Payoff_of_AsianOption will be used to store all nodes in the current layer, because it needs to be iterated, so use the "previousOptionValue" as a copy
	std::vector<double> previousOptionValue = Payoff_of_AsianOption;

	//Since the level number of the binary tree is "mytimeSteps + 1", it is necessary to discount "mytimeSteps" forward
	for (int n = mytimeSteps; n > 0; n--)
	{
		Payoff_of_AsianOption.clear();
		for (int i = 0; i < pow(2, n); )   //This "for" loop is to traverse each node of the current layer, the number of nodes in the nth layer is pow (2, n)
		{
			Payoff_of_AsianOption.push_back((myBinomialTree.myProbability * previousOptionValue[i + 1] + (1 - myBinomialTree.myProbability) * previousOptionValue[i]) *exp(-myInterestRate * myBinomialTree.mydelta_t));
			i += 2;
		}
		previousOptionValue = Payoff_of_AsianOption;
	}
	return Payoff_of_AsianOption[0];
}


// Third pricing method: Monte Carlo. Please see Part1 3.3 in PDF.
double Asian::PriceAsianOptionWithMonteCarlo(int NumberOfSimulation)
{
	double delta_t = 1.0 / 365;    //Asian option performs arithmetic sampling once a day
	double Sum_Of_Payoff_Of_Simulate = 0;
	
	for (int i = 0; i < NumberOfSimulation; i++)   //A total of "NumberOfSimulation" samples are obtained
	{
		double Sum_Of_EachDaySimulate = mySharePrice;
		double SimulateEachDayPrice = mySharePrice;
		for (int n = 1; n < myexpiryTime * 365; n++)   //Perform a simulation once a day and take the average
		{
			SimulateEachDayPrice = SimulateEachDayPrice * exp((myInterestRate - myVolatility * myVolatility / 2)*delta_t + myVolatility * MC.MC()*sqrt(delta_t));
			Sum_Of_EachDaySimulate += SimulateEachDayPrice;
		}
		Sum_Of_Payoff_Of_Simulate += exp(-myInterestRate * myexpiryTime)*AsianPayoff(Sum_Of_EachDaySimulate / (myexpiryTime * 365));
	}
	double Average = Sum_Of_Payoff_Of_Simulate / NumberOfSimulation;
	return Average;
}


