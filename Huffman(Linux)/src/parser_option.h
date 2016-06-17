#ifndef PARSER_OPTION_H
#define PARSER_OPTION_H
#include <fstream>
#include "string.h"
#include <iostream>
#define KEY 1
#define FNAME 2
#define COUNT_OF_ARGS 3
#define DECODER 1
#define ENCODER 2
#define FEW_ARGS 1
#define BAD_KEY 2
#define BAD_FNAME 3
int parser_opt(char argc, char *argv[]);

#endif