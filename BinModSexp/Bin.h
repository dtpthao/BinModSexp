#ifndef _BIN_H
#define _BIN_H

#include "Field.h"
#include "shr_decomp.h"
#include "Duration.h"

/* powbin.cpp */
void powmod_Bin(big X, big k, big P, big &Z);
void nrespowmod_Bin(big X, big k, big P, big &Z);
void testBin(big P, csprng &Rng);

/* pow2bin.cpp */
void powmod2_Bin(big X, big a, big Y, big b, big P, big &Z);
void test2Bin(big P, csprng &Rng);
void compare_bin2s(big P, csprng &Rng, Result &res);
void printBins(Result res[NUM_OF_P + 1], int *m);

/* pow3bin.cpp */
void powmod3_Bin(big *r, big *y, big P, big &R);
void test3Bin(big P, csprng &Rng);
void compare_bin3s(big P, csprng &Rng, Result &res);

/* pownbin.cpp */
void powmodn_Bin(int n, big *r, big *y, big P, big &R);
void testnBin(big P, csprng &Rng);
void compare_binns(big P, csprng &Rng, Result &res);

#endif // !_BIN_H

