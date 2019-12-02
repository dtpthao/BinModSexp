#include "Field.h"

big gl_bigs[LEN_GL_BIGS];

void InitStrongRNG(csprng *Rng)
{
	srand(time(NULL));
	char raw[100];
	for (int i = 0; i < 100; i++)
		raw[i] = (char)rand();
	strong_init(Rng, 100, raw, time(NULL));
}

void getGF(big &P, CONSTANTS_F2m f2m) {
	int i;
	P = mirvar(1);
	i = f2m.m >> 5;
	P->len = i + 1;
	P->w[i] = 1 << (f2m.m & 0x1F);

	for (int j = 0; j < i; P->w[j++] = 0);
	i = f2m.k3 >> 5;
	P->w[i] += 1 << (f2m.k3 & 0x1F);
	
	i = f2m.k2 >> 5;
	P->w[i] += 1 << (f2m.k2 & 0x1F);
	
	i = f2m.k1 >> 5;
	P->w[i] += 1 << (f2m.k1 & 0x1F);
}

void mulmod(big X, big Y, big P, big Z)
{
	multiply(X, Y, Z);
	divide(Z, P, P);
}

