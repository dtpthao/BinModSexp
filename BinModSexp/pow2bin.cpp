#include "Bin.h"

void powmod2_Bin(big X, big a, big Y, big b, big P, big &Z)
{
	int i, j = 0, index;
	DWORD lastw, a1, b1;
	i = b->len - 1;
	lastw = b->w[i];
	while (lastw >> j && j != 32) j++;

	copy(X, gl_bigs[1]);
	copy(Y, gl_bigs[2]);
	mulmod(X, Y, P, gl_bigs[3]);
	Z->len = 1; Z->w[0] = 1;
	for (--j; i >= 0; i--, j = 31) {
		a1 = a->w[i];
		b1 = b->w[i];
		while (j) {
			index = (a1 >> j) & 1;
			index += ((b1 >> j) & 1) << 1;
			mulmod(Z, Z, P, Z);
			if (index) mulmod(Z, gl_bigs[index], P, Z);
			j--;
		}
		index = (a1 & 1) + ((b1 & 1) << 1);
		mulmod(Z, Z, P, Z);
		if (index) mulmod(Z, gl_bigs[index], P, Z);
	}
}

#define TEST2 10000
void test2Bin(big P, csprng &Rng)
{
	big X = mirvar(0), Y = mirvar(0), k = mirvar(0);
	big a = mirvar(0), b = mirvar(0),
		Z = mirvar(0), Z1 = mirvar(0), Z2 = mirvar(0);

	//k = mirvar(0x69D); X = mirvar(3);
	//nxprime(k, P);
	strong_bigrand(&Rng, P, X);
	cout << "P: "; cotnum(P, stdout);

	int count = 0;
	for (int i = 0; i < TEST2; i++) {
		strong_bigrand(&Rng, P, k);
		ShamirDecomposit(k, a, b, X, Y, P);
		powmod2_Bin(X, a, Y, b, P, Z1);
		powmod2(X, a, Y, b, P, Z);
		count += !compare(Z, Z1);
		if (compare(Z, Z1)) {
			powmod_Bin(X, k, P, Z2);
			cout << "k: "; cotnum(k, stdout);
			cout << "a: "; cotnum(a, stdout);
			cout << "b: "; cotnum(b, stdout);
			cout << "X: "; cotnum(X, stdout);
			cout << "Z : "; cotnum(Z, stdout);
			cout << "Z1: "; cotnum(Z1, stdout);
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
void compare_bin2s(big P, csprng &Rng, Result &res)
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
	big y[2], r[2];
	y[0] = mirvar(1); y[1] = mirvar(1);
	r[0] = mirvar(1); r[1] = mirvar(1);

	strong_bigrand(&Rng, P, g);
	//ShamirDecomposit_ng(len, 2, g, y, P);
	copy(g, X);
	//copy(y[1], Y);

	int count1 = 0, count2 = 0;
	for (int i = 0; i < TEST2; i++) {
		strong_bigrand(&Rng, P, k);
		/*ShamirDecomposit_nk(2, k, r);
		copy(r[0], a);
		copy(r[1], b);*/
		ShamirDecomposit(k, a, b, X, Y, P);

		for (int i = 0; i < REPEAT2; i++) {
			startTimer(&timer1);
			powmod_Bin(X, k, P, R1);           // R1 = X^k mod P
			stopTimer(&timer1);
			dur1 = getTickCount(&timer1);
			min1 = (min1 < dur1) ? min1 : dur1;

			startTimer(&timer2);
			powmod2_Bin(X, a, Y, b, P, R2);    // R2 = X^a * Y^b mod P
			stopTimer(&timer2);
			dur2 = getTickCount(&timer2);
			min2 = (min2 < dur2) ? min2 : dur2;

			startTimer(&timer3);
			//powmodn_Bin(2, r, y, P, R3);       // R3 = y1^r1 * y2^r2 mod P
			//powmod2_Bin_old(X, a, Y, b, P, R3);
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
	res.t[0] /= TEST2; 
	res.t[1] /= TEST2; 
	res.t[2] /= TEST2; 
	res.t[3] /= TEST2; 
	res.p[1] = (res.t[1] / res.t[0]) * 100;
	res.p[2] = (res.t[2] / res.t[0]) * 100;
	res.p[3] = (res.t[3] / res.t[0]) * 100;

	mirkill(a); mirkill(b); mirkill(X); mirkill(Y);
	mirkill(r[0]); mirkill(r[1]); mirkill(y[0]); mirkill(y[1]);
	mirkill(g); mirkill(k);
	mirkill(R1); mirkill(R2); mirkill(R3); mirkill(R);
}

#define NUM_OF_P 5
void printBins(Result res[NUM_OF_P + 1], int *m)
{
	const int lib = 3;
	res[NUM_OF_P].p[0] = 100;

	for (int i = 0; i <= NUM_OF_P; i++) {
		res[i].p[0] = 100;
	}

	for (int i = 1; i <= lib; i++) {
		res[NUM_OF_P].p[i] = res[0].p[i] + res[1].p[i]
			+ res[2].p[i] + res[3].p[i] + res[4].p[i];
		res[NUM_OF_P].p[i] /= NUM_OF_P;
	}
	for (int i = 0; i < NUM_OF_P; i++) {
		printf("%2d | %4d: %13.3f %13.3f %13.3f %13.3f\n",
			i, m[i], res[i].t[0], res[i].t[1], res[i].t[2], res[i].t[lib]);
	}
	cout << endl;
	for (int i = 0; i < NUM_OF_P; i++) {
		printf("%2d | %4d: %12.1f%% %12.1f%% %12.1f%% %12.1f%%\n",
			i, m[i], res[i].p[0], res[i].p[1], res[i].p[2], res[i].p[lib]);
	}
	printf("    avg  : %12.1f%% %12.1f%% %12.1f%% %12.1f%%\n",
		res[NUM_OF_P].p[0], res[NUM_OF_P].p[1], res[NUM_OF_P].p[2], res[NUM_OF_P].p[lib]);
	cout << endl;
	for (int i = 0; i < NUM_OF_P; i++) {
		printf("%2d | %4d: %13d %13d %13d %13d\n",
			i, m[i], res[i].c[0], res[i].c[1], res[i].c[2], res[i].c[lib]);
	}
}
