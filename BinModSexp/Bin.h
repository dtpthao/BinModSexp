#ifndef _BIN_H
#define _BIN_H

#include "Field.h"
#include "ShamirMul.h"

void powmod_Bin(big X, big k, big P, big &Z);
void nrespowmod_Bin(big X, big k, big P, big &Z);

inline void prePowModBin(big X, big Y, big *list, big P);
void powmod2_Bin(big X, big a, big Y, big b, big P, big &Z);
void powmod_ShrBin(big X, big k, big P, big &Z);

void testMulBin(big P, csprng &Rng);
void testMul2Bin(big P, csprng &Rng);

#endif // !_BIN_H

