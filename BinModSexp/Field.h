#ifndef _FIELD_H
#define _FIELD_H

#include <iostream>
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <iomanip>
extern "C" {
#include"miracl.h"
}

using namespace std;

#define NUM_OF_P	5
#define LIB			2
#define LEN_GL_BIGS 300
extern big gl_bigs[LEN_GL_BIGS];

struct CONSTANTS_F2m {
	DWORD m, k3, k2, k1;
};

void InitStrongRNG(csprng *Rng);

void getGF(big &P, CONSTANTS_F2m f2m);

// multiply and divide
void mulmod(big X, big Y, big P, big Z);

#endif
