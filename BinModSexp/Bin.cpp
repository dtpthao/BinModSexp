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

//  0   1   2    3     4    5      6        7
// [1, y1, y2, y1*y2, y3, y1*y3, y2*y3, y1*y2*y3]
void prepowmod3_Bin(big y1, big y2, big y3, big *y, big P)
{
	for (int i = 0; i < 8; i++) y[i] = mirvar(1);
	copy(y1, y[1]);
	copy(y2, y[2]);
	copy(y3, y[4]);
	mulmod(y1, y2, P, y[3]);
	mulmod(y1, y3, P, y[5]);
	mulmod(y2, y3, P, y[6]);
	mulmod(y[6], y1, P, y[7]);
	//for (int i = 0; i < 8; i++) {
	//	printf("y[%d]: ", i); cotnum(y[i], stdout);
	//}
}

// R = y1^r1 + y2^r2 + y3^r3;   |r1| >= |r2| >= |r3|; || - bit len
void powmod3_Bin(big r1, big r2, big r3, big y1, big y2, big y3, big P, big &R)
{
	int bit1, bit2, bit3, i, j = 0, index;
	DWORD last32, w1, w2, w3;
	big y[8];
	prepowmod3_Bin(y1, y2, y3, y, P);

	i = r3->len - 1;
	last32 = r3->w[i];
	while (last32 >> j && j != 32) j++;

	R = mirvar(1);
	for (j; i >= 0; i--, j = 31) {
		w1 = r1->w[i];
		w2 = r2->w[i];
		w3 = r3->w[i];
		while (j) {
			bit1 = (w1 >> j) & 1;
			bit2 = ((w2 >> j) & 1) << 1;
			bit3 = ((w3 >> j) & 1) << 2;
			index = bit1 + bit2 + bit3;
			//printf("index: %d\n", index);
			mulmod(R, R, P, R);
			//cout << "R^2: "; cotnum(R, stdout);
			if (index) mulmod(R, y[index], P, R);
			//cout << "R: "; cotnum(R, stdout);
			j--;
		}
		index = (w1 & 1) + ((w2 & 1) << 1) + ((w3 & 1) << 2);
		//printf("index: %d\n", index);
		mulmod(R, R, P, R);
		//cout << "R^2: "; cotnum(R, stdout);
		if (index) mulmod(R, y[index], P, R);
		//cout << "R: "; cotnum(R, stdout);
	}
	for (i = 0; i < 8; i++) mirkill(y[i]);
}

// R = y1^r1 + y2^r2 + y3^r3;   |r1| = |r2| <= |r3|; || - bit len
void powmod3_Bin2(big *r, big *y, big P, big &R)
{
	int bit1, bit2, bit3, i, j = 0, index;
	DWORD last32, w1, w2, w3;
	big pre_lst[8];
	prepowmod3_Bin(y[1], y[2], y[3], pre_lst, P);

	i = r[3]->len - 1;
	last32 = r[3]->w[i];
	while (last32 >> j && j != 32) j++;

	R = mirvar(1);
	for (--j; i >= 0; i--, j = 31) {
		w1 = r[1]->w[i];
		w2 = r[2]->w[i];
		w3 = r[3]->w[i];
		while (j) {
			bit1 = (w1 >> j) & 1;
			bit2 = ((w2 >> j) & 1) << 1;
			bit3 = ((w3 >> j) & 1) << 2;
			index = bit1 + bit2 + bit3;
			//printf("index: %d\n", index);
			mulmod(R, R, P, R);
			//cout << "R^2: "; cotnum(R, stdout);
			if (index) mulmod(R, pre_lst[index], P, R);
			//cout << "R: "; cotnum(R, stdout);
			j--;
		}
		index = (w1 & 1) + ((w2 & 1) << 1) + ((w3 & 1) << 2);
		//printf("index: %d\n", index);
		mulmod(R, R, P, R);
		//cout << "R^2: "; cotnum(R, stdout);
		if (index) mulmod(R, pre_lst[index], P, R);
		//cout << "R: "; cotnum(R, stdout);
	}
	for (i = 0; i < 8; i++) mirkill(pre_lst[i]);
}

void powmod3_ShrBin(big X, big k, big P, big &Z)
{
	big r1 = mirvar(1),
		r2 = mirvar(1),
		r3 = mirvar(1),
		y1 = mirvar(1),
		y2 = mirvar(1),
		y3 = mirvar(1);
	ShamirDecomposit3(k, r1, r2, r3, y1, y2, y3, P);
	powmod3_Bin(r1, r2, r3, y1, y2, y3, P, Z);
	mirkill(r1); mirkill(r2); mirkill(r3);
	mirkill(y1); mirkill(y2); mirkill(y3);
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

#define MUL3 1000
void testMul3Bin(big P, csprng &Rng)
{
	big k = mirvar(0);
	big r1 = mirvar(0), r2 = mirvar(0), r3 = mirvar(0),
		y1 = mirvar(0), y2 = mirvar(0), y3 = mirvar(0);
	big	R = mirvar(0), R1 = mirvar(0);
	//r1 = r2 = r3 = y1 = y2 = y3 = R = R1 = mirvar(0); -> we can't use this
	//k = mirvar(0x69D); y[1] = mirvar(3);
	//nxprime(k, P);
	strong_bigrand(&Rng, P, y1);
	cout << "P: "; cotnum(P, stdout);

	int count = MUL3;
	for (int i = 0; i < MUL3; i++) {
		//strong_bigrand(&Rng, P, k);
		ShamirDecomposit3(k, r1, r2, r3, y1, y2, y3, P);
		cout << "k: "; cotnum(k, stdout);
		cout << "r1: "; cotnum(r1, stdout);
		cout << "r2: "; cotnum(r2, stdout);
		cout << "r3: "; cotnum(r3, stdout);
		cout << "y1: "; cotnum(y1, stdout);
		cout << "y2: "; cotnum(y2, stdout);
		cout << "y3: "; cotnum(y3, stdout);

		powmod3_Bin(r1, r2, r3, y1, y2, y3, P, R1);
		//cout << "R1: "; cotnum(R1, stdout);
		powmod(y1, k, P, R);
		//cout << "R : "; cotnum(R, stdout);
		count -= compare(R, R1);
	}
	printf("correct: %d\n\n", count);
	mirkill(r1); mirkill(r2); mirkill(r3);
	mirkill(y1); mirkill(y2); mirkill(y3);
	mirkill(R); mirkill(R1); mirkill(k);
}

void testMul3Bin_2(big P, csprng &Rng)
{
	big k = mirvar(0);
	big	R = mirvar(0), R1 = mirvar(0);
	big r[3], y[3];
	r[1] = mirvar(0); r[2] = mirvar(0); r[3] = mirvar(0);
	y[1] = mirvar(0); y[2] = mirvar(0); y[3] = mirvar(0);
	strong_bigrand(&Rng, P, y[1]);
	cout << "P: "; cotnum(P, stdout);

	int count = MUL3;
	for (int i = 0; i < MUL3; i++) {
		strong_bigrand(&Rng, P, k);
		ShamirDecomposit3_2(k, y[1], r, y, P);
		//cout << "k: "; cotnum(k, stdout);
		//cout << "r1: "; cotnum(r[1], stdout);
		//cout << "r2: "; cotnum(r[2], stdout);
		//cout << "r3: "; cotnum(r[3], stdout);
		//cout << "y1: "; cotnum(y[1], stdout);
		//cout << "y2: "; cotnum(y[2], stdout);
		//cout << "y3: "; cotnum(y[3], stdout);

		powmod3_Bin2(r, y, P, R1);
		//cout << "R1: "; cotnum(R1, stdout);
		powmod(y[1], k, P, R);
		//cout << "R : "; cotnum(R, stdout);
		count -= compare(R, R1);
	}
	printf("correct: %d\n\n", count);
	mirkill(r[1]); mirkill(r[2]); mirkill(r[3]);
	mirkill(y[1]); mirkill(y[2]); mirkill(y[3]);
	mirkill(R); mirkill(R1); mirkill(k);
}
