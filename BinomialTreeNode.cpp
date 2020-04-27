#include"BinomialTreeNode.h"
BinomialTreeNode::BinomialTreeNode(const double& e)
{
	data = e;
	Parent = 0;
}

BinomialTreeNode::BinomialTreeNode(const double& e, BinomialTreeNode *parent)
{
	data = e;
	Parent = parent;
}

