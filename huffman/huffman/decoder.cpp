#include "stdafx.h"
#include "decoder.h"

int init_freq_table(std::ifstream& input, std::vector<char>& chars, 
	freq_table_t& freq_table, char& byte)
{
	char buf;
	unsigned char n;
	unsigned char count_bits;
	int count = 0; 
	input.exceptions ( std::ifstream::badbit);
	try
	{
		// Read count of different characters and count of the necessary bits for the highest frequency 
		//+ throw illegal files
		input.read(&buf, 1);
		if(input.gcount()==0) throw ILL_FORM;
		n = (unsigned char)buf;
		input.read(&buf, 1);
		if(input.gcount()==0) throw ILL_FORM; 
		count_bits = (unsigned char)buf;
		if (count_bits==0 || n == 0 ) return -1;
		// Read sequence: symbol - symbol's frequency ...
		byte = input.get();
		for (unsigned char i = 0; i < n; i++)
		{
			char c = 0;
			unsigned long amount = 0;
			for (int n = 7; n >= 0; n--)
			{
				int b = byte & (1 << (7 - count));
				c = c | ((b && 1) << n);
				count++;
				if (count == 8) 
				{ 
					count = 0; 
					byte = input.get(); 
				}
			}
			for (int n = (int)count_bits - 1; n >= 0; n--)
			{
				int b = byte & (1 << (7 - count));
				amount = amount | ((b && 1) << n);
				count++;
				if (count == 8) 
				{
					count = 0; 
					byte = input.get(); 
				}
			}
			chars.push_back(c);
			freq_table[c] = amount;
		}
	}
	catch(int e)
	{
		std::cerr << "Input file is not valid" << std::endl;
		exit(1);
	}
	catch(std::ifstream::failure e)
	{
		std::cerr << "Input file is not valid" << std::endl;
		exit(1);
	}
	return count;
}

// Function to make name for output file 
void ofnamehuf(const char * fname, std::string& outname)
{
	std::string str(fname);
	size_t pos = str.find_last_of(".");
	if (pos != std::string::npos)
		outname = str.substr(0, pos) + ".txt";
	else
		outname = str + ".txt";
}

void writetxt(std::ifstream& input, std::ofstream& output, Tree *root, char& byte, int c)
{
	try
	{
		int count = c;
		std::streampos pos = input.tellg();
		input.seekg(-1,input.end);
		std::streampos end = input.tellg();
		char last_count = input.get();  // Last char = count of used bits in last byte of encrypted file
		input.seekg(pos);
	
		Tree *ptr = root;
	
		bool flag = true; // flag = false if the file is already read
		while (flag || last_count!=0)
		{
			int b = byte & (1 << (7 - count));
			if (b != 0) 
				ptr = ptr->right; 
			else
				ptr = ptr->left;
			if (ptr->left == NULL && ptr->right == NULL) 
			{ 
				output << ptr->c; 
				ptr = root;
			}
			count++;
			if(!flag) last_count--;
			if (count == 8) 
			{ 
				count = 0; 
				byte = input.get();
				if(end == input.tellg()) 
				flag=false;
			}
		}
		input.close();
		output.close();
	}
	catch(std::ofstream::failure e)
	{
		std::cerr << "Output file is not valid" << std::endl;
		exit(1);
	}
	catch(std::exception e)
	{
		exit(1);
	}
}
int decoder(char * fname)
{
	char byte;
	std::string outname;
	std::vector<char> chars;
	freq_table_t freq_table;
	std::ifstream input(fname, std::ios::in | std::ios::binary);
	forest_t forest;

	ofnamehuf(fname,outname);
	std::ofstream output(outname, std::ios::out | std::ios::trunc);
	if(!output)
	{
		std::cerr << "Impossible to write in file: " << outname << std::endl;
		exit(1);
	}

	int count = init_freq_table(input, chars, freq_table, byte);
	if(count<0)
	{
		// Close empty output file  
		input.close();
		output.close();
		return 1;
	}
	// Creat queue of trees 
	for (char c : chars)
	{
		Tree * x = new Tree(c, freq_table[c]);
		forest.push(x);
	}
	// From queue to one tree
	Tree *ptr = forest2tree(forest);
	// Finding the code tree and encoded text
	writetxt(input, output, ptr, byte, count);
	return 0;
}