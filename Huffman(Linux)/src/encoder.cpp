#include "stdafx.h"
#include "encoder.h"

int init_freq_table(std::ifstream& input, freq_table_t& ft)
{
	char buffer[4096];
	std::streamsize n;
	input.exceptions(std::ifstream::badbit);
	try
	{
		// Read file and collect info about symbols' frequency
		while (true)
		{
			input.read(buffer, 4096);
			n = input.gcount();
			if (n == 0) throw EMPTY_FILE;
			for (std::streamsize i = n - 1; i >= 0; i--)
			{
				char c = buffer[i];
				if (ft[c]!=ULONG_MAX)
				ft[c]++;
			}
			if (n < 4096) break;
		}
		// set offset = 0 for next pass
	    input.clear(); 
		input.seekg(0);
	}
	catch(int e)
	{
		return 1;
	}
	catch(std::ifstream::failure e)
	{
		std::cerr << "Input file is not valid" << std::endl;
		exit(1);
	}
	return 0;
	
}

// Function to make name for output file 
void ofname(const char * fname, std::string& outname)
{
	std::string str(fname);
	size_t pos = str.find_last_of(".");
	if (pos != std::string::npos)
		outname = str.substr(0, pos) + ".huf";
	else
		outname = str + ".huf";
}

void writehuf(std::ifstream& input, std::ofstream& output, freq_table_t& freq_table,
						const std::vector<char>& chars, unsigned long max, CodeTable& table)
{
	char count = 0; 
	unsigned char wbuf = 0;
	char rbuf[4096];
	unsigned char count_bits = (char)log2(max) + 1;
	unsigned char count_chars = table.count_nodes();
	output.exceptions ( std::ofstream::badbit);
	input.exceptions ( std::ifstream::badbit);
	try
	{

		output << count_chars;  // First char = count of different characters
		output << count_bits;   // Second char = count of the necessary bits for the highest frequency

		// Sequence: symbol - symbol's frequency ...
		for (char c : chars)
		{
			for (int n = 7; n >= 0; n--)
			{
				wbuf = wbuf | (((c >> n) & 1) << (7 - count));
				count++;
				if (count == 8) 
				{ 
					count = 0; 
					output << wbuf;  
					wbuf = 0; 
				}
			}
			for (int n = count_bits - 1 ; n >= 0; n--)
			{
				wbuf = wbuf | (((freq_table[c] >> n) & 1) << (7 - count));
				count++;
				if (count == 8) 
				{ 
					count = 0; 
					output << wbuf; 
					wbuf = 0; 
				}
			}
		}
		// The encrypted input file
		while (true)
		{
			input.read(rbuf, 4096);
			std::streamsize size = input.gcount();
			if (size == 0) break;
			for (std::streamsize i = 0; i < size; i++)
			{
				char c = rbuf[i];
				std::vector<int> x = table.value(c);
				for (unsigned int n = 0; n < x.size(); n++)
				{
					wbuf = wbuf | x[n] << (7 - count);
					count++;
					if (count == 8) 
					{ 
						count = 0; 
						output << wbuf; 
						wbuf = 0; 
					}
				}		
			}
			if (size < 4096) break;
		}
		output << wbuf;
		// Last char = count of used bits in last byte of encrypted file
		output << count;
		input.close();
		output.close();
	}
	catch(std::ifstream::failure e)
	{
		std::cerr << "File is not valid" << std::endl;
		exit(1);
	}
	catch(std::exception e)
	{
		exit(1);
	}

}

int encoder(const char * fname)
{
    unsigned long max = 0;  // maximum of all symbol's frequency
	std::vector<char> chars;
	std::string outname;
	freq_table_t freq_table;
	forest_t forest;
	Tree *Root;
	
	std::ifstream input(fname, std::ios::in);
	ofname(fname,outname);
	std::ofstream output(outname, std::ios::out | std::ios::binary | std::ios::trunc);
	if(!output)
	{
		std::cerr << "Impossible to write in file: " << outname << std::endl;
		exit(1);
	}
	
	if(init_freq_table(input,freq_table)==1)
	{
		// Input file is empty
		output << (char)0;
	    output << (char)0;
		input.close();
	    output.close();
		return 0;
	}

	// Remember the order in which elements are put in a queue
	for (std::pair<char, unsigned long> c : freq_table)
	{
		Tree * x = new Tree(c.first, c.second);
		if (c.second > max) max = c.second;
		chars.push_back(c.first);
		forest.push(x);
	}
	// Make tree from queue of tree
	Root = forest2tree(forest);

	// Make code for each symbol in input file
	CodeTable table(Root);

	// input file -> output file
	writehuf(input, output, freq_table, chars, max, table);

	return 0;
}
