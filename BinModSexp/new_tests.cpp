#include "new_tests.h"

#define TESTS 5000
#define REPEAT 10
void compares(int len, big P, csprng &Rng, Result &res)
{
	stopWatch timer1, timer2, timer3, timer4, timer5, timer6;
	LONGLONG dur1, min1 = LONG_MAX,
		dur2, min2 = LONG_MAX,
		dur3, min3 = LONG_MAX,
		dur4, min4 = LONG_MAX,
		dur5, min5 = LONG_MAX,
		dur6, min6 = LONG_MAX;
	big a = mirvar(1), b = mirvar(1),
		X = mirvar(1), Y = mirvar(1);
	big	g = mirvar(1), k = mirvar(1),
		R = mirvar(1), R1 = mirvar(1), R5 = mirvar(1),
		R2 = mirvar(1), R3 = mirvar(1), R4 = mirvar(1);
	big r[5], y[5];
	r[0] = mirvar(0); r[1] = mirvar(0); r[2] = mirvar(0); r[3] = mirvar(0); r[4] = mirvar(0);
	y[0] = mirvar(0); y[1] = mirvar(0); y[2] = mirvar(0); y[3] = mirvar(0); y[4] = mirvar(0);

	strong_bigrand(&Rng, P, g);
	//ShamirDecomposit_ng(len, 3, g, y, P);
	copy(g, X);
	for (int i = 0; i < TESTS; i++) {
		strong_bigrand(&Rng, P, k);
		//ShamirDecomposit_nk(3, k, r);

		for (int j = 0; j < REPEAT; j++) {
			// bin1
			startTimer(&timer1);
			powmod_Bin(g, k, P, R1);           // R1 = g^k mod P
			stopTimer(&timer1);
			dur1 = getTickCount(&timer1);
			min1 = (min1 < dur1) ? min1 : dur1;

			// bin2
			startTimer(&timer2);
			ShamirDecomposit(k, a, b, X, Y, P);
			powmod2_Bin(X, a, Y, b, P, R2);		// R2 = X^a * Y^b mod P
			//powmod_JSF(X, a, Y, b, P, R2);		// R2 = X^a * Y^b mod P
			//powmod2(X, a, Y, b, P, R2);
			stopTimer(&timer2);
			dur2 = getTickCount(&timer2);
			min2 = (min2 < dur2) ? min2 : dur2;

			// bin3
			startTimer(&timer3);
			ShamirDecomposit3(k, g, r, y, P);
			powmod3_Bin(r, y, P, R3);			// R3 = y1^r1 * y2^r2 * y3^r3
			//powmod_dJSF(3, y, r, P, R3);
			//powmodn(3, y, r, P, R3);
			stopTimer(&timer3);
			dur3 = getTickCount(&timer3);
			min3 = (min3 < dur3) ? min3 : dur3;

			// bin4
			startTimer(&timer4);
			ShamirDecomposit_n(4, k, g, r, y, P);
			powmodn_Bin(4, r, y, P, R4);		// R4 = y1^r1 * ...* y4^r4
			//powmod_dJSF(4, y, r, P, R4);
			//powmodn(4, y, r, P, R4);
			stopTimer(&timer4);
			dur4 = getTickCount(&timer4);
			min4 = (min4 < dur4) ? min4 : dur4;

			// bin5
			startTimer(&timer5);
			ShamirDecomposit_n(5, k, g, r, y, P);
			powmodn_Bin(5, r, y, P, R5);
			//powmod_dJSF(5, y, r, P, R5);
			//powmodn(5, y, r, P, R5);
			stopTimer(&timer5);
			dur5 = getTickCount(&timer5);
			min5 = (min5 < dur5) ? min5 : dur5;

			// lib1
			startTimer(&timer6);
			//ShamirDecomposit_n(4, k, g, r, y, P);
			//powmodn(4, y, r, P, R);
			powmod(g, k, P, R);
			stopTimer(&timer6);
			dur6 = getTickCount(&timer6);
			min6 = (min6 < dur6) ? min6 : dur6;
		}
		res.c[0] += !compare(R, R1);
		res.c[1] += !compare(R, R2);
		res.c[2] += !compare(R, R3);
		res.c[3] += !compare(R, R4);
		res.c[4] += !compare(R, R5);
		res.t[0] += min1;
		res.t[1] += min2;
		res.t[2] += min3;
		res.t[3] += min4;
		res.t[4] += min5;
		res.t[5] += min6;
	}
	res.t[0] /= TESTS;
	res.t[1] /= TESTS;
	res.t[2] /= TESTS;
	res.t[3] /= TESTS;
	res.t[4] /= TESTS;
	res.t[5] /= TESTS;
	res.p[1] = (res.t[1] / res.t[0]) * 100;
	res.p[2] = (res.t[2] / res.t[0]) * 100;
	res.p[3] = (res.t[3] / res.t[0]) * 100;
	res.p[4] = (res.t[4] / res.t[0]) * 100;
	res.p[5] = (res.t[5] / res.t[0]) * 100;

	mirkill(a); mirkill(b); mirkill(X); mirkill(Y);
	mirkill(r[0]); mirkill(r[1]); mirkill(r[2]); mirkill(r[3]); mirkill(r[4]);
	mirkill(y[0]); mirkill(y[1]); mirkill(y[2]); mirkill(y[3]); mirkill(y[4]);
	mirkill(g); mirkill(k);
	mirkill(R1); mirkill(R2); mirkill(R3); mirkill(R); mirkill(R4); mirkill(R5);
}

#define NUM_OF_P 5
void printcompares_bin(Result res[NUM_OF_P + 1], int *m)
{
	const int ilib = 5;
	res[NUM_OF_P].p[0] = 100;

	for (int i = 1; i <= ilib; i++) {
		res[NUM_OF_P].p[i] = res[0].p[i] + res[1].p[i]
			+ res[2].p[i] + res[3].p[i] + res[4].p[i];
		res[NUM_OF_P].p[i] /= NUM_OF_P;
	}
	for (int i = 0; i < NUM_OF_P; i++) {
		printf("%2d | %4d: %13.3f %13.3f %13.3f %13.3f %13.3f %13.3f\n",
			i, m[i], res[i].t[0], res[i].t[1], res[i].t[2], res[i].t[3], res[i].t[4], res[i].t[ilib]);
	}
	cout << endl;
	for (int i = 0; i < NUM_OF_P; i++) {
		printf("%2d | %4d: %12.1f%% %12.1f%% %12.1f%% %12.1f%% %12.1f%% %12.1f%%\n",
			i, m[i], res[i].p[0], res[i].p[1], res[i].p[2], res[i].p[3], res[i].p[4], res[i].p[ilib]);
	}
	printf("    avg  : %12.1f%% %12.1f%% %12.1f%% %12.1f%% %12.1f%% %12.1f%%\n",
		res[NUM_OF_P].p[0], res[NUM_OF_P].p[1], res[NUM_OF_P].p[2], res[NUM_OF_P].p[3], res[NUM_OF_P].p[4], res[NUM_OF_P].p[ilib]);
	cout << endl;
	for (int i = 0; i < NUM_OF_P; i++) {
		printf("%2d | %4d: %13d %13d %13d %13d %13d %13d\n",
			i, m[i], res[i].c[0], res[i].c[1], res[i].c[2], res[i].c[3], res[i].c[4], res[i].c[ilib]);
	}
}