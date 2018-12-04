#ifndef _FIELD_H
#define _FIELD_H

#include <iostream>
#include <Windows.h>
#include <time.h>
#include <stdio.h>
extern "C" {
#include"miracl.h"
}

using namespace std;

#define NUM_OF_P	5
#define LIB			2

struct CONSTANTS_F2m {
	DWORD m, k3, k2, k1;
} ;

void InitStrongRNG(csprng *Rng);

void getGF(big &P, CONSTANTS_F2m f2m);

void mulmod(big X, big Y, big P, big Z);

#endif
