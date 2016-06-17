#ifndef TREE_H
#define TREE_H
#include "stdafx.h"
#include "stdio.h"
#include <queue>

class Tree
{
public:
	unsigned long amount;
	char c;
	Tree *left, *right;

	Tree();
	Tree(char _c, unsigned long _amount);
	Tree(Tree *L, Tree *R);
};
struct MyCompare
{
	bool operator()(const Tree* left, const Tree* right) const { return left->amount > right->amount; }
};

typedef std::priority_queue<Tree *, std::vector<Tree *>, MyCompare > forest_t;

Tree * forest2tree(forest_t& forest);
#endif