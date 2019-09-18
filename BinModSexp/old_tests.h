#ifndef _OLD_TESTS
#define _OLD_TESTS

#include "Field.h"
#include "ShamirMul.h"
#include "Duration.h"
#include "Bin.h"
#include "JSF.h"

struct Result {
	double t[5] = { 0 };
	double p[5] = { 0 };
	unsigned int c[5] = { 0 };
};

#define TEST	5000
#define LIB		3

void Test(csprng &Rng, big X, big P, Result &res);

void printBinOption(Result res[NUM_OF_P + 1], CONSTANTS_F2m *f2m);


#endif // !_OLD_TESTS

