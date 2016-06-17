#include "stdafx.h"
#include "parser_option.h"
#include "encoder.h"
#include "decoder.h"

int main(int argc, char *argv[])
{
	int coder = parser_opt(argc, argv);
	switch (coder)
	{
	case DECODER:
		decoder(argv[2]);
		break;
	case ENCODER:
		encoder(argv[2]);
		break;
	default:
		return 1;
	}
	return 0;
}