#include "ConventionalMacros.h"

const int POW_OF_TWO(const int& n)
{
	static const int table[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192 };

	return table[n];
}

const int POW_OF_EIGHT(const int& n)
{
	static const int table[] = { 1, 8, 64, 512, 4096 };

	return table[n];
}
