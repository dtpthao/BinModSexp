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
	prePowModJSF(X, Y, P, gl_bigs);
	lenJSF = GenJSF(a, b, JSFa, JSFb);

	index = 4 - 3 * JSFa[lenJSF] - JSFb[lenJSF];
	copy(gl_bigs[index], Z);
	for (int i = lenJSF - 1; i > 0; i--) {
		index = 4 - 3 * JSFa[i] - JSFb[i];
		mulmod(Z, Z, P, Z);
		if (index != 4) mulmod(Z, gl_bigs[index], P, Z);
	}
}

/*========== this version of GenJSF is worse approximately 2 times than the above one ===========*/
inline void subGenJSF(big Var1, big Var2, char &JSFi, bool &d, big RS)
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

DWORD GenJSF2(big R, big S, char *JSFr, char *JSFs)
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
/*===============================================================================================*/

/*===================================== OLD powmods =============================================*/
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
/*===============================================================================================*/

/*===================================== TESTING STUFF ===========================================*/
void compare_GenJSFs(big P, csprng &Rng)
{
	big *x = new big[2];
	DWORD lenJSF, lenJSF2, sum1, sum2;
	big k = mirvar(0x1ED627);
	stopWatch timer1, timer2, timer3;
	LONGLONG dur1, min1 = LONG_MAX,
		dur2, min2 = LONG_MAX,
		dur3, min3 = LONG_MAX;
	double t1 = 0, t2 = 0, w1 = 0, w2 = 0;
	int count1 = 0, count2 = 0;

	char **JSF = new char*[2];
	char **JSF2 = new char*[2];
	x[0] = mirvar(0);
	x[1] = mirvar(0);
	JSF[0] = new char[800];
	JSF[1] = new char[800];
	JSF2[0] = new char[800];
	JSF2[1] = new char[800];
	sum1 = sum2 = 0;
	const int TIMES = 5000;
	for (int i = 0; i < TIMES; i++) {
		strong_bigrand(&Rng, P, k);
		ShamirDecomposit_nk(2, k, x);
		count1 = 0; count2 = 0;
		for (int j = 0; j < 10; j++) {
			startTimer(&timer1);
			lenJSF = GenJSF(x[0], x[1], JSF[0], JSF[1]);
			stopTimer(&timer1);
			dur1 = getTickCount(&timer1);
			min1 = (min1 < dur1) ? min1 : dur1;

			startTimer(&timer2);
			//lenJSF2 = GendJSF(2, x, JSF2);
			lenJSF2 = GenJSF2(x[0], x[1], JSF2[0], JSF2[1]);
			stopTimer(&timer2);
			dur2 = getTickCount(&timer2);
			min2 = (min2 < dur2) ? min2 : dur2;
		}
		t1 += min1;
		t2 += min2;

		//if (lenJSF != lenJSF2) cout << "Blinchik!" << endl;
		for (int j = lenJSF; j > 0; j--) {
			if (!(JSF[0][j] | JSF[1][j])) count1++;
		}
		for (int j = lenJSF2 - 1; j >= 0; j--) {
			if (!(JSF2[0][j] | JSF2[1][j])) count2++;
		}
		w1 += (double)count1 / lenJSF;
		w2 += (double)count2 / lenJSF2;
		//if (count1 != count2) {
		//	cout << "Damnnnn!" << endl;
		//	break;
		//}
	}
	t1 /= TIMES;
	t2 /= TIMES;
	w1 /= TIMES;
	w2 /= TIMES;
	cout << "Average testing results of " << TIMES << " pairs of numbers:" << endl;
	std::cout << "weight GenJSF : " << (1 - w1) << endl;
	std::cout << "weight GenJSF2: " << (1 - w2) << endl;
	std::cout << "runtime GenJSF : " << t1 << endl;
	std::cout << "runtime GenJSF2: " << t2 << endl;

	mirkill(x[0]);
	mirkill(x[1]);
	delete[] JSF[0];
	delete[] JSF[1];
	delete[] JSF2[0];
	delete[] JSF2[1];
	delete[] JSF; delete[] JSF2;
	mirkill(k);
}

#define TESTJSF 5000
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

#define REPEAT2 10
void compare_JSFs(big P, csprng &Rng, Result &res)
{
	stopWatch timer1, timer2, timer3, timer4;
	LONGLONG dur1, min1 = LONG_MAX,
		dur2, min2 = LONG_MAX,
		dur3, min3 = LONG_MAX,
		dur4, min4 = LONG_MAX;
	big a = mirvar(1), b = mirvar(1),
		X = mirvar(1), Y = mirvar(1);
	big	g = mirvar(1), k = mirvar(1),
		R = mirvar(1), R1 = mirvar(1),
		R2 = mirvar(1), R3 = mirvar(1);

	strong_bigrand(&Rng, P, g);
	//ShamirDecomposit_ng(len, 2, g, y, P);
	copy(g, X);
	//copy(y[1], Y);

	int count1 = 0, count2 = 0;
	for (int i = 0; i < TESTJSF; i++) {
		strong_bigrand(&Rng, P, k);
		ShamirDecomposit(k, a, b, X, Y, P);

		for (int i = 0; i < REPEAT2; i++) {
			startTimer(&timer1);
			powmod2_Bin(X, a, Y, b, P, R1);    
			stopTimer(&timer1);
			dur1 = getTickCount(&timer1);
			min1 = (min1 < dur1) ? min1 : dur1;

			startTimer(&timer2);
			powmod_JSF(X, a, Y, b, P, R2);    // R2 = X^a * Y^b mod P
			stopTimer(&timer2);
			dur2 = getTickCount(&timer2);
			min2 = (min2 < dur2) ? min2 : dur2;

			startTimer(&timer3);
			//powmod_JSF_gl(X, a, Y, b, P, R3);
			stopTimer(&timer3);
			dur3 = getTickCount(&timer3);
			min3 = (min3 < dur3) ? min3 : dur3;

			startTimer(&timer4);
			powmod2(X, a, Y, b, P, R);         // R = X^a * Y^b mod P
			stopTimer(&timer4);
			dur4 = getTickCount(&timer4);
			min4 = (min4 < dur4) ? min4 : dur4;
		}
		res.c[0] += !compare(R, R1);
		res.c[1] += !compare(R, R2);
		res.c[2] += !compare(R, R3);
		res.t[0] += min1;
		res.t[1] += min2;
		res.t[2] += min3;
		res.t[3] += min4;
	}
	res.t[0] /= TESTJSF;
	res.t[1] /= TESTJSF;
	res.t[2] /= TESTJSF;
	res.t[3] /= TESTJSF;
	res.p[1] = (res.t[1] / res.t[0]) * 100;
	res.p[2] = (res.t[2] / res.t[0]) * 100;
	res.p[3] = (res.t[3] / res.t[0]) * 100;

	mirkill(a); mirkill(b); mirkill(X); mirkill(Y);
	mirkill(g); mirkill(k);
	mirkill(R1); mirkill(R2); mirkill(R3); mirkill(R);
}
/*===============================================================================================*/