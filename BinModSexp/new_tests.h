#ifndef _NEW_TESTS
#define _NEW_TESTS

#include "Field.h"
#include "shr_decomp.h"
#include "Duration.h"
#include "Bin.h"
#include "JSF.h"

#define TEST	500
#define LIB		3

void compares(int len, big P, csprng &Rng, Result &res);

void printcompares_bin(Result res[NUM_OF_P + 1], int *m);


#endif // !_NEW_TESTS
