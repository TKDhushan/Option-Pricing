#pragma once
class BinomialTreeNode
{
public:
	double data;
	BinomialTreeNode * Parent;
	BinomialTreeNode(const double& e);
	BinomialTreeNode(const double& e, BinomialTreeNode *parent);
};

