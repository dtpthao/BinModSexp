#ifndef _BIN_H
#define _BIN_H

#include "Field.h"
#include "shr_decomp.h"
#include "Duration.h"

/* powbin.cpp */
void powmod_Bin(big X, big k, big P, big &Z);
void nrespowmod_Bin(big X, big k, big P, big &Z);
void test_HammingWeight_dBin(big P, csprng &Rng);

/* pow2bin.cpp */
void powmod2_Bin(big X, big a, big Y, big b, big P, big &Z);
void testBin(big P, csprng &Rng);
void test2Bin(big P, csprng &Rng);
void compare_bin2s(big P, csprng &Rng, Result &res);
void printBins(Result res[NUM_OF_P + 1], int *m);

/* pow3bin.cpp */
void powmod3_Bin(big *r, big *y, big P, big &R);
void powmod3_Bin(big r1, big r2, big r3, big y1, big y2, big y3, big P, big &R);
void powmod3_ShrBin(big X, big k, big P, big &Z);
void test3Bin(big P, csprng &Rng);
void test3Bin_2(big P, csprng &Rng);
void compare_bin3s(big P, csprng &Rng, Result &res);

/* pownbin.cpp */
void powmodn_Bin(int n, big *r, big *y, big P, big &R);
void testnBin(big P, csprng &Rng);
void compare_binns(big P, csprng &Rng, Result &res);

#endif // !_BIN_H

