#include "JSF.h"

DWORD GenJSF(big R, big S, char *JSFr, char *JSFs)
{
	big R1 = mirvar(0), S1 = mirvar(0);
	DWORD lenJSF = 0;

	copy(R, R1); copy(S, S1);
	while (S1->len > 0 || R1->len > 0) {
		lenJSF++;
		JSFr[lenJSF] = R1->w[0] & 1;
		JSFs[lenJSF] = S1->w[0] & 1;
		if (JSFr[lenJSF] & JSFs[lenJSF]) {
			if (R1->w[0] & 2) JSFr[lenJSF] = -JSFr[lenJSF];
			if (S1->w[0] & 2) JSFs[lenJSF] = -JSFs[lenJSF];
		}
		else if (JSFr[lenJSF] ^ JSFs[lenJSF]) {
			if ((R1->w[0] & 2) ^ (S1->w[0] & 2)) {
				JSFr[lenJSF] = -JSFr[lenJSF];
				JSFs[lenJSF] = -JSFs[lenJSF];
			}
		}
		sftbit(R1, -1, R1);
		sftbit(S1, -1, S1);
		if (JSFr[lenJSF] == -1) {
			if (R1->len == 0) R1->len = 1;
			if (R1->w[0] ^ 0xffffffff) R1->w[0]++;
			else incr(R1, 1, R1);
		}
		if (JSFs[lenJSF] == -1) {
			if (S1->len == 0) S1->len = 1;
			if (S1->w[0] ^ 0xffffffff) S1->w[0]++;
			else incr(S1, 1, S1);
		}
	}
	mirkill(R1); mirkill(S1);
	return lenJSF;
}

//	0	1	2	3	4	5	 6	  7		8
// {X*Y, X, X/Y, Y, 0, 1/Y, Y/X, 1/X, 1/XY}
inline void prePowModJSF(big X, big Y, big P, big *lst)
{
	mulmod(X, Y, P, lst[0]);	//X*Y mod P

	copy(X, lst[1]);			//X
	copy(Y, lst[3]);			//Y

	copy(X, lst[7]);
	xgcd(lst[7], P, lst[7], lst[7], lst[7]);	//1/X mod P
	copy(Y, lst[5]);
	xgcd(lst[5], P, lst[5], lst[5], lst[5]);	//1/Y mod P

	mulmod(X, lst[5], P, lst[2]);	//X/Y mod P
	mulmod(Y, lst[7], P, lst[6]);	//Y/X mod P

	mulmod(lst[7], lst[5], P, lst[8]);
}

void powmod_JSF(big X, big a, big Y, big b, big P, big Z)
{
	char JSFa[1000] = { 0 };
	char JSFb[1000] = { 0 };
	DWORD lenJSF;
	int index;
	big lst[9];
	for (int i = 0; i < 9; i++) lst[i] = mirvar(1);
	prePowModJSF(X, Y, P, lst);
	lenJSF = GenJSF(a, b, JSFa, JSFb);

	index = 4 - 3 * JSFa[lenJSF] - JSFb[lenJSF];
	copy(lst[index], Z);
	for (int i = lenJSF - 1; i > 0; i--) {
		index = 4 - 3 * JSFa[i] - JSFb[i];
		mulmod(Z, Z, P, Z);
		if (index != 4)
			mulmod(Z, lst[index], P, Z);
	}

	for (int i = 0; i < 9; i++) mirkill(lst[i]);
}

// why did I do this???
//	0	1	2	3	4	5	 6	  7		8
// {X*Y, X, X/Y, Y, 0, 1/Y, Y/X, 1/X, 1/XY}
inline void prePowModJSF(big X, big X1, big Y, big Y1, big P, big *lst)
{
	mulmod(X, Y, P, lst[0]);	//X*Y mod P

	copy(X, lst[1]);			//X
	copy(Y, lst[3]);			//Y

	copy(X1, lst[7]);
	//xgcd(lst[7], P, lst[7], lst[7], lst[7]);	//1/X mod P
	copy(Y1, lst[5]);
	//xgcd(lst[5], P, lst[5], lst[5], lst[5]);	//1/Y mod P

	mulmod(X, lst[5], P, lst[2]);	//X/Y mod P
	mulmod(Y, lst[7], P, lst[6]);	//Y/X mod P

	mulmod(X1, Y1, P, lst[8]);
	//copy(lst[0], lst[8]);
	//xgcd(lst[8], P, lst[8], lst[8], lst[8]);	//1/XY mod P
}

// why did I do this???
void powmod2_JSF(big X, big X1, big a, big Y, big Y1, big b, big P, big Z)
{
	char JSFa[300] = { 0 };
	char JSFb[300] = { 0 };
	DWORD lenJSF;
	int index;
	big lst[9];
	for (int i = 0; i < 9; i++) lst[i] = mirvar(1);
	prePowModJSF(X, X1, Y, Y1, P, lst);
	lenJSF = GenJSF(a, b, JSFa, JSFb);

	index = 4 - 3 * JSFa[lenJSF] - JSFb[lenJSF];
	copy(lst[index], Z);
	for (int i = lenJSF - 1; i > 0; i--) {
		index = 4 - 3 * JSFa[i] - JSFb[i];
		mulmod(Z, Z, P, Z);
		if (index != 4)
			mulmod(Z, lst[index], P, Z);
	}
	for (int i = 0; i < 9; i++)
		mirkill(lst[i]);
}

// just let it be here
void powmod_ShrJSF(big X, big k, big P, big Z)
{
	big a = mirvar(1),
		b = mirvar(1),
		X1 = mirvar(1),
		Y = mirvar(1), Y1 = mirvar(1);
	ShamirDecomposit(k, a, b, X, Y, P);
	copy(X, X1); copy(Y, Y1);
	xgcd(X1, P, X1, X1, X1);
	xgcd(Y1, P, Y1, Y1, Y1);
	powmod2_JSF(X, X1, a, Y, Y1, b, P, Z);
	mirkill(a); mirkill(b); 
	mirkill(X1); mirkill(Y1); mirkill(Y);
}

// just let it be here
void powmod_JSF(big *lst, big a, big b, big P, big Z)
{
	char JSFa[300] = { 0 };
	char JSFb[300] = { 0 };
	DWORD lenJSF;
	int index;
	//big lst[9];
	/*prePowModJSF(X, Y, P, lst);*/
	lenJSF = GenJSF(a, b, JSFa, JSFb);

	index = 4 - 3 * JSFa[lenJSF] - JSFb[lenJSF];
	copy(lst[index], Z);
	for (int i = lenJSF - 1; i > 0; i--) {
		index = 4 - 3 * JSFa[i] - JSFb[i];
		mulmod(Z, Z, P, Z);
		if (index != 4)
			mulmod(Z, lst[index], P, Z);
	}
}

#define TESTJSF 100
void testJSF(big P, csprng &Rng)
{
	big X = mirvar(0), Y = mirvar(0), k = mirvar(0);
	big a = mirvar(0), b = mirvar(0),
		Z = mirvar(0), Z1 = mirvar(0), Z2 = mirvar(0);
	strong_bigrand(&Rng, P, X);
	//strong_bigdig(&Rng, 2, 16, X);
	//nxprime(X, P);
	//X = mirvar(0x32);
	//k = mirvar(0x15);
	//P = mirvar(0x35);
	int count = 0;
	for (int i = 0; i < TESTJSF; i++) {
		strong_bigrand(&Rng, P, k);
		//strong_bigdig(&Rng, 3, 16, k);
		ShamirDecomposit(k, a, b, X, Y, P);
		//cout << "k: "; cotnum(k, stdout);
		//cout << "a: "; cotnum(a, stdout);
		//cout << "b: "; cotnum(b, stdout);
		//cout << "X: "; cotnum(X, stdout);
		//cout << "Y: "; cotnum(Y, stdout);
		//cout << "P: "; cotnum(P, stdout);

		//powmod2_JSF(X, a, Y, b, P, Z);
		powmod2(X, a, Y, b, P, Z1);
		//cout << "Z : "; cotnum(Z, stdout);
		//cout << "Z1: "; cotnum(Z1, stdout);
		count += !compare(Z, Z1);
		if (compare(Z, Z1)) {
			cout << "k: "; cotnum(k, stdout);
			cout << "a: "; cotnum(a, stdout);
			cout << "b: "; cotnum(b, stdout);
			cout << "X: "; cotnum(X, stdout);
			cout << "Y: "; cotnum(Y, stdout);
			cout << "P: "; cotnum(P, stdout);
			cout << "Z : "; cotnum(Z, stdout);
			cout << "Z1: "; cotnum(Z1, stdout);
			powmod2_Bin(X, a, Y, b, P, Z2);
			cout << "Z2: "; cotnum(Z2, stdout);
			break;
		}
	}
	printf("%d\n", count);
	mirkill(X); mirkill(Y);
	mirkill(k); mirkill(a); mirkill(b);
	mirkill(Z); mirkill(Z1); mirkill(Z2);
}