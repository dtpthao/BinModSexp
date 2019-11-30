#ifndef _BIN_H
#define _BIN_H

#include "Field.h"
#include "shr_decomp.h"
#include "Duration.h"

// powbin.cpp
void powmod_Bin(big X, big k, big P, big &Z);
void nrespowmod_Bin(big X, big k, big P, big &Z);

// pow2bin.cpp
void powmod2_Bin(big X, big a, big Y, big b, big P, big &Z);
void testBin(big P, csprng &Rng);
void test2Bin(big P, csprng &Rng);
void compare_2_n(int len, big P, csprng &Rng, Result &res);

// pow3bin.cpp
void powmod3_Bin(big r1, big r2, big r3, big y1, big y2, big y3, big P, big &R);
void powmod3_Bin(big *r, big *y, big P, big &R);
void powmod3_ShrBin(big X, big k, big P, big &Z);
void test3Bin(big P, csprng &Rng);
void test3Bin_2(big P, csprng &Rng);

// pownbin.cpp
void powmodn_Bin(int n, big *r, big *y, big P, big &R);
void testnBin(big P, csprng &Rng);

#endif // !_BIN_H

