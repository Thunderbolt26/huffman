#ifndef CODE_TABLE_H
#define CODE_TABLE_H

#include "tree.h"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <functional>
#include <climits>
#include <cmath>

class CodeTable
{
private:
	unsigned char N;
	void BuildTable(Tree *root);
	std::unordered_map<char, std::vector<int> > table;
public:
    std::vector<int> code;
	CodeTable(){}
	CodeTable(Tree *root);
	int count_nodes(){return N;}
	std::vector<int> value(char c){ return table[c];}
};

#endif