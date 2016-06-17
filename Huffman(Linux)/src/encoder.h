#ifndef ENCODER_H
#define ENCODER_H
#include "stdafx.h"
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
#define EMPTY_FILE 1

#ifndef FREQ_TABLE
#define FREQ_TABLE
typedef std::unordered_map<char, unsigned long> freq_table_t;
#endif
int encoder(const char * fname);
#endif