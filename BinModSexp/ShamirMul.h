#ifndef _SHAMIR_MUL_H
#define _SHAMIR_MUL_H

#include "Field.h"

// R = X^a * Y^b mod P
void ShamirDecomposit(big k, big a, big b, big X, big Y, big P);

// R = g^k = y1^r1 * y2^r2 * y3^r3 mod P
void ShamirDecomposit3(big k, big r1, big r2, big r3, big y1, big y2, big y3, big P);
void ShamirDecomposit3_2(big k, big g, big *r, big *y, big P);

void ShamirDecompositn(int n, big k, big g, big *r, big *y, big P);

#endif

