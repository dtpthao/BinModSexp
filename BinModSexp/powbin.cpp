#include "Bin.h"

void powmod_Bin(big X, big k, big P, big &Z)
{
	unsigned int len, i = 31;

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

#define TEST1 100
void testBin(big P, csprng &Rng)
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

