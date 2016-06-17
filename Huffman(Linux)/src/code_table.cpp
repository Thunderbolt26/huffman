#include "stdafx.h"
#include "code_table.h"


CodeTable::CodeTable(Tree *root)
{
	N = 0;
	if (root == NULL) return;
	BuildTable(root);
}
void CodeTable::BuildTable(Tree *root)
{
	if (root->left != NULL)
	{
		code.push_back(0);
		BuildTable(root->left);
		code.pop_back();
	}
	if (root->right != NULL)
	{
		code.push_back(1);
		BuildTable(root->right);
		code.pop_back();
	}
	if (root->left == NULL && root->right == NULL) 
	{
		table[root->c] = code; 
		N++;
	}
}
