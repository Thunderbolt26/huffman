#ifndef DECODER_H
#define DECODER_H
#include "tree.h"
#include "code_table.h"
#include "stdio.h"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <functional>
#include <climits>
#include <cmath>
#include <string>
#define ILL_FORM 1
#ifndef FREQ_TABLE
#define FREQ_TABLE
typedef std::unordered_map<char, unsigned long> freq_table_t;
#endif

int decoder(char * fname);
#endif