#include "Bin.h"

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

// R = g^k = y0^r0 * y1^r1 * y2^r2
// Bit length of r[i] (i = [0, 1]) is greater than bit length of r2 by 1 
// ==> seems a stupid idea
void powmod3_Bin(big r1, big r2, big r3, big y1, big y2, big y3, big P, big &R)
{
	int bit1, bit2, bit3, i, j = 0, index;
	DWORD last32, w1, w2, w3;
	big y[8];
	prepowmod3_Bin(y1, y2, y3, y, P);

	i = r3->len - 1;
	last32 = r3->w[i];
	while (last32 >> j && j != 32) j++;

	R->len = 1; R->w[0] = 1;
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

// R = g^k = y0^r0 * y1^r1 * y2^r2
// Bit length of r2 is greater than bit length of r0, r1 by bitlen of k mod 3
// This is currently effective.
void powmod3_Bin(big *r, big *y, big P, big &R)
{
	int bit1, bit2, bit3, i, j = 0, index;
	DWORD last32, w1, w2, w3;
	big pre_lst[8];
	prepowmod3_Bin(y[0], y[1], y[2], pre_lst, P);

	i = r[2]->len - 1;
	last32 = r[2]->w[i];
	while (last32 >> j && j != 32) j++;

	R->len = 1; R->w[0] = 1;
	for (--j; i >= 0; i--, j = 31) {
		w1 = r[0]->w[i];
		w2 = r[1]->w[i];
		w3 = r[2]->w[i];
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
		//cout << endl;
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

#define TEST3 1000
void test3Bin(big P, csprng &Rng)
{
	big k = mirvar(0);
	big r1 = mirvar(0), r2 = mirvar(0), r3 = mirvar(0),
		y1 = mirvar(0), y2 = mirvar(0), y3 = mirvar(0);
	big	R = mirvar(0), R1 = mirvar(0);
	//r1 = r2 = r3 = y1 = y2 = y3 = R = R1 = mirvar(0); ==> just to remember that this is unusable
	//k = mirvar(0x69D); y[1] = mirvar(3);
	//nxprime(k, P);
	strong_bigrand(&Rng, P, y1);
	cout << "P: "; cotnum(P, stdout);

	int count = TEST3;
	for (int i = 0; i < TEST3; i++) {
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

void test3Bin_2(big P, csprng &Rng)
{
	big k = mirvar(0);
	big	R = mirvar(0), R1 = mirvar(0);
	big r[3], y[3];
	r[0] = mirvar(0); r[1] = mirvar(0); r[2] = mirvar(0);
	y[0] = mirvar(0); y[1] = mirvar(0); y[2] = mirvar(0);
	strong_bigrand(&Rng, P, y[0]);
	cout << "P: "; cotnum(P, stdout);
	//char sk[51] = "7214642831191146504103114534140063683838032640121";
	//cinstr(k, sk);
	//char g[51] = "2843960000033430704672362122080271010298877412814";
	//cinstr(y[0], g);

	int count = 0;
	for (int i = 0; i < TEST3; i++) {
		strong_bigrand(&Rng, P, k);
		ShamirDecomposit3(k, y[0], r, y, P);
		cout << "k: "; cotnum(k, stdout);
		cout << "r0: "; cotnum(r[0], stdout);
		cout << "r1: "; cotnum(r[1], stdout);
		cout << "r2: "; cotnum(r[2], stdout);
		cout << "y0: "; cotnum(y[0], stdout);
		cout << "y1: "; cotnum(y[1], stdout);
		cout << "y2: "; cotnum(y[2], stdout);

		powmod3_Bin(r, y, P, R1);
		cout << "R1: "; cotnum(R1, stdout);
		powmod(y[0], k, P, R);
		cout << "R : "; cotnum(R, stdout);
		count += !compare(R, R1);
	}
	printf("correct: %d\n\n", count);
	mirkill(r[0]); mirkill(r[1]); mirkill(r[2]);
	mirkill(y[0]); mirkill(y[1]); mirkill(y[2]);
	mirkill(R); mirkill(R1); mirkill(k);
}

