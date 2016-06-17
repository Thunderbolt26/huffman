#include "stdafx.h"
#include "tree.h"

Tree::Tree()
{ 
	left = NULL;
	right = NULL;
	amount = 0;
	c='!';
}
Tree::Tree(char _c, unsigned long _amount)
{ 
	left = NULL;
	right = NULL;
	amount = _amount; 
	c = _c;
}

Tree::Tree(Tree *L, Tree *R)
{
	left = L;
	right = R;
	amount = L->amount + R->amount;
	c='!';
}


Tree * forest2tree(forest_t& forest)
{
	while (forest.size()!=1)
	{
		Tree *L = forest.top();
		forest.pop();
		Tree *R = forest.top();
		forest.pop();
		Tree *LR = new Tree(L, R);
		forest.push(LR);
	}
	return forest.top();
}