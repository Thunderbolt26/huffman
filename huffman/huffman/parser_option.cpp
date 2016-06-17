#include "stdafx.h"
#include "parser_option.h"

int parser_opt(char argc, char *argv[])
{
	int rc=0;
	try
	{
	    if(argc < COUNT_OF_ARGS) throw FEW_ARGS;
        if(strcmp(argv[KEY], "-d") == 0 || strcmp(argv[KEY], "--decoder") == 0)
		    rc = DECODER;
		if(strcmp(argv[KEY], "-e") == 0 || strcmp(argv[KEY], "--encoder") == 0)
			rc = ENCODER;
		if(rc == 0) throw BAD_KEY;
		if (!std::ifstream(argv[FNAME], std::ios::in)) throw BAD_FNAME;
	}
	catch(int a)
    {
		switch(a)
		{
		case FEW_ARGS:
		case BAD_KEY:
			std::cerr << "Usage: " << std::endl << "1) bin/app -d/--decoder filename" << std::endl << "2) bin/app -e/--encoder filename" << std::endl;
			exit(a);
		case BAD_FNAME:
			std::cerr << argv[FNAME] << ": No such file" << std::endl;
			exit(a);
		}
    }
	return rc;
}