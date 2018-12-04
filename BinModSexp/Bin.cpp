#include "Bin.h"

void powmod_Bin(big X, big k, big P, big &Z)
{
	unsigned int len, i = 31;
	Z = mirvar(1);

	len = k->w[k->len - 1];
	if (len == 0) return;
	while (!(len & (1 << i))) i--;
	len = (k->len << 5) - (31 - i);

	copy(X, Z);
	for (int j = len - 2; j >= 0; j--) {
		mulmod(Z, Z, P, Z);
		if (k->w[j >> 5] & (1 << j)) {
			mulmod(Z, X, P, Z);
		}
	}
}

void nrespowmod_Bin(big X, big k, big P, big &Z)	//testing
{
	unsigned int len, i = 31;
	Z = mirvar(1);
	big Z1 = mirvar(1), Z2 = mirvar(1);
	prepare_monty(P);

	len = k->w[k->len - 1];
	if (len == 0) return;
	while (!(len & (1 << i))) i--;
	len = (k->len << 5) - (31 - i);

	copy(X, Z);
	copy(X, Z1);
	cout << "Z : "; cotnum(Z, stdout);
	cout << "Z1: "; cotnum(Z1, stdout);
	for (int j = len - 2; j >= 0; j--) {
		mulmod(Z1, Z1, P, Z1);
		nres_modmult(Z, Z, Z2);
		cout << "Z : "; cotnum(Z2, stdout);
		cout << "Z1: "; cotnum(Z1, stdout);
		if (k->w[j >> 5] & (1 << j)) {
			mulmod(Z1, X, P, Z1);
			nres_modmult(Z2, X, Z);
			cout << "Z : "; cotnum(Z, stdout);
			cout << "Z1: "; cotnum(Z1, stdout);
		}
		cout << endl;
	}
	mirkill(Z1);
}

inline void prePowModBin(big X, big Y, big *list, big P)
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
	prePowModBin(X, Y, lst, P);
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

#define MUL1 100
void testMulBin(big P, csprng &Rng)
{
	big k = mirvar(0), X = mirvar(0),
		Z = mirvar(0), Z1 = mirvar(0);
	//strong_bigrand(&Rng, P, X);

	//int count = MUL1;
	//for (int i = 0; i < MUL1; i++) {
	//	strong_bigrand(&Rng, P, k);
	//	powmod_Bin(X, k, P, Z);
	//	nrespowmod_Bin(X, k, P, Z2);
	//	powmod(X, k, P, Z1);
	//	count -= compare(Z, Z1);
	//}
	//count -= compare(Z, Z1);
	//printf("%d\n", count);

	strong_bigdig(&Rng, 2, 10, P);
	P->w[0] |= 1;
	//strong_bigrand(&Rng, P, X);
	//strong_bigrand(&Rng, P, k);
	strong_bigdig(&Rng, 1, 10, X);
	strong_bigdig(&Rng, 1, 10, k);
	cout << "P : "; cotnum(P, stdout);
	cout << "X : "; cotnum(X, stdout);
	cout << "k : "; cotnum(k, stdout);
	cout << endl;
	//powmod_Bin(X, k, P, Z);
	nrespowmod_Bin(X, k, P, Z);
	powmod(X, k, P, Z1);
	
	cout << endl << endl;
	cout << "Z : "; cotnum(Z, stdout);
	cout << "Z1: "; cotnum(Z1, stdout);
	
	mirkill(k); mirkill(Z); mirkill(Z1);
}

#define MUL2 1000
void testMul2Bin(big P, csprng &Rng)
{
	big X = mirvar(0), Y = mirvar(0), k = mirvar(0);
	big a = mirvar(0), b = mirvar(0),
		Z = mirvar(0), Z1 = mirvar(0);
	strong_bigrand(&Rng, P, X);

	int count = MUL2;
	for (int i = 0; i < MUL2; i++) {
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

