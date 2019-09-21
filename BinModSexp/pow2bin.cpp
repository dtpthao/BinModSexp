#include "Bin.h"

inline void prepowmod2_Bin(big X, big Y, big *list, big P)
{
	list[0] = mirvar(1);
	list[1] = mirvar(0);
	list[2] = mirvar(0);
	list[3] = mirvar(0);
	copy(X, list[1]);
	copy(Y, list[2]);
	mulmod(X, Y, P, list[3]);
}

void powmod2_Bin(big X, big a, big Y, big b, big P, big &Z)
{
	int bita, bitb, i, j = 0, index;
	DWORD lastw, a1, b1;
	i = b->len - 1;
	lastw = b->w[i];
	while (lastw >> j && j != 32) j++;

	big lst[4];
	prepowmod2_Bin(X, Y, lst, P);
	Z = mirvar(1);
	for (j; i >= 0; i--, j = 31) {
		a1 = a->w[i];
		b1 = b->w[i];
		while (j) {
			bita = (a1 >> j) & 1;
			bitb = (b1 >> (j - 1)) & 2;
			mulmod(Z, Z, P, Z);
			index = bitb + bita;
			if (index) {
				mulmod(Z, lst[index], P, Z);
			}
			j--;
		}
		index = (a1 & 1) + ((b1 & 1) << 1);
		mulmod(Z, Z, P, Z);
		mulmod(Z, lst[index], P, Z);
	}
	mirkill(lst[0]); mirkill(lst[1]);
	mirkill(lst[2]); mirkill(lst[3]);
}

void powmod_ShrBin(big X, big k, big P, big &Z)
{
	big a = mirvar(1),
		b = mirvar(1),
		Y = mirvar(1);
	ShamirDecomposit(k, a, b, X, Y, P);
	powmod2_Bin(X, a, Y, b, P, Z);
	mirkill(a); mirkill(b); mirkill(Y);
}

#define TEST2 1000
void test2Bin(big P, csprng &Rng)
{
	big X = mirvar(0), Y = mirvar(0), k = mirvar(0);
	big a = mirvar(0), b = mirvar(0),
		Z = mirvar(0), Z1 = mirvar(0);
	strong_bigrand(&Rng, P, X);

	int count = TEST2;
	for (int i = 0; i < TEST2; i++) {
		strong_bigrand(&Rng, P, k);
		ShamirDecomposit(k, a, b, X, Y, P);
		/*cout << "k: "; cotnum(k, stdout);
		cout << "a: "; cotnum(a, stdout);
		cout << "b: "; cotnum(b, stdout);
		cout << "X: "; cotnum(X, stdout);*/

		powmod2_Bin(X, a, Y, b, P, Z);
		powmod2(X, a, Y, b, P, Z1);
		count -= compare(Z, Z1);
	}
	printf("%d\n", count);
	mirkill(X); mirkill(Y);
	mirkill(k); mirkill(a); mirkill(b);
	mirkill(Z); mirkill(Z1);
}
