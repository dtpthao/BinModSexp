#include "JSF.h"

inline void subGenJSF(big Var1, big Var2, char &JSFi, bool &d, big RS)	//in use
{
	DWORD v1 = Var1->w[0], v2 = Var2->w[0];
	if (!(v1 & 1)) JSFi = 0;
	else {
		JSFi = (v1 & 2) ? -1 : 1;
		if ((!(v1 & 7 ^ 3) || !(v1 & 7 ^ 5)) && ((v2 & 3) == 2))
			JSFi = -JSFi;
	}
	if (((int)d << 1) == (JSFi + 1)) d = 1 - d;
	sftbit(RS, -1, RS);
}

DWORD GenJSF(big R, big S, char *JSFr, char *JSFs)	
{
	big L1 = mirvar(1), L2 = mirvar(1),
		R1 = mirvar(0), S1 = mirvar(0);
	bool d1 = 0, d2 = 0;
	DWORD lenJSF = 0;

	copy(R, L1); copy(S, L2); copy(R, R1); copy(S, S1);
	while (L1->len > 0 || L2->len > 0) {
		lenJSF++;
		subGenJSF(L1, L2, JSFr[lenJSF], d1, R1);
		subGenJSF(L2, L1, JSFs[lenJSF], d2, S1);
		incr(R1, d1, L1);
		incr(S1, d2, L2);
	}
	mirkill(L1); mirkill(L2);
	mirkill(R1); mirkill(S1);
	return lenJSF;
}                                                                                         

//	0	1	2	3	4	5	 6	  7		8
// {X*Y, X, X/Y, Y, 0, 1/Y, Y/X, 1/X, 1/XY}
inline void prePowModJSF(big X, big Y, big P, big *lst)	
{
	for (int i = 0; i < 9; i++)
		lst[i] = mirvar(1);
	mulmod(X, Y, P, lst[0]);	//X*Y mod P

	copy(X, lst[1]);			//X
	copy(Y, lst[3]);			//Y

	copy(X, lst[7]);
	xgcd(lst[7], P, lst[7], lst[7], lst[7]);	//1/X mod P
	copy(Y, lst[5]);
	xgcd(lst[5], P, lst[5], lst[5], lst[5]);	//1/Y mod P

	mulmod(X, lst[5], P, lst[2]);	//X/Y mod P
	mulmod(Y, lst[7], P, lst[6]);	//Y/X mod P

	copy(lst[0], lst[8]);
	xgcd(lst[8], P, lst[8], lst[8], lst[8]);	//1/XY mod P
}

void powmod2_JSF(big X, big a, big Y, big b, big P, big Z)
{
	char JSFa[300] = { 0 };
	char JSFb[300] = { 0 };
	DWORD lenJSF;
	int index;
	big lst[9];
	
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
	for (int i = 0; i < 9; i++)
		mirkill(lst[i]);
}

void powmod_ShrJSF(big X, big k, big P, big Z)
{
	big a = mirvar(1),
		b = mirvar(1),
		Y = mirvar(1);
	ShamirDecomposit(k, a, b, X, Y, P);
	powmod2_JSF(X, a, Y, b, P, Z);
	mirkill(a); mirkill(b); mirkill(Y);
}

void powmod_JSF(big *lst, big X, big a, big Y, big b, big P, big Z)
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

		powmod2_JSF(X, a, Y, b, P, Z);
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
	mirkill(Z); mirkill(Z1);
}