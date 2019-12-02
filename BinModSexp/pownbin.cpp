#include "Bin.h"

// This is complicated to explain in condensed words
// for example with n = 3:
//  0   1   2    3     4    5      6        7
// [1, y1, y2, y1*y2, y3, y1*y3, y2*y3, y1*y2*y3]
void prepowmodn_Bin(int n, big *y, big *pre_list, big P)
{
	int loop = 1 << n, idx;
	for (int i = 0; i < loop; i++) pre_list[i] = mirvar(1);
	for (int i = 0; i < n; i++) {
		idx = 1 << i;
		copy(y[i], pre_list[idx]);
		//printf("pre_list[%d]: ", idx); cotnum(pre_list[idx], stdout);
		for (int j = 1; j < idx; j++) {
			mulmod(pre_list[j], pre_list[idx], P, pre_list[idx + j]);
			//printf("pre_list[%d]: ", idx + j); cotnum(pre_list[idx + j], stdout);
		}
	}
}

// R = y1^r1 * y2^r2 * ... * yn^rn mod P;
void powmodn_Bin(int n, big *r, big *y, big P, big &R)
{
	int i, j = 0, tmp;
	DWORD last32,
		*w = new DWORD[n];
	big *pre_lst = new big[1 << n];
	prepowmodn_Bin(n, y, pre_lst, P);

	i = r[n - 1]->len - 1;
	last32 = r[n - 1]->w[i];
	while (last32 >> j && j != 32) j++;

	R->len = 1; R->w[0] = 1;
	for (--j; i >= 0; i--, j = 31) {
		tmp = 0;
		for (int ii = 0; ii < n; ii++) w[ii] = r[ii]->w[i];
		while (j) {
			for (int ii = 0; ii < n; ii++) {
				tmp += ((w[ii] >> j) & 1) << ii;
			}
			//printf("index: %d\n", tmp);
			mulmod(R, R, P, R);
			//cout << "R^2: "; cotnum(R, stdout);
			if (tmp) mulmod(R, pre_lst[tmp], P, R);
			//cout << "R: "; cotnum(R, stdout);
			j--;
			tmp = 0;
		}
		for (int ii = 0; ii < n; ii++) {
			tmp += (w[ii] & 1) << ii;
		}
		//printf("tmp: %d\n", tmp);
		mulmod(R, R, P, R);
		//cout << "R^2: "; cotnum(R, stdout);
		if (tmp) mulmod(R, pre_lst[tmp], P, R);
		//cout << "R: "; cotnum(R, stdout);
		//cout << endl;
	}
	tmp = 1 << n;
	for (i = 0; i < tmp; i++) mirkill(pre_lst[i]);
}

// This is complicated to explain in condensed words
// for example with n = 3:
//  0   1   2    3     4    5      6        7
// [1, y1, y2, y1*y2, y3, y1*y3, y2*y3, y1*y2*y3]
void prepowmodn_Bin_gl(int n, big *y, big *pre_list, big P)
{
	int idx;
	for (int i = 0; i < n; i++) {
		idx = 1 << i;
		copy(y[i], pre_list[idx]);
		//printf("pre_list[%d]: ", idx); cotnum(pre_list[idx], stdout);
		for (int j = 1; j < idx; j++) {
			mulmod(pre_list[j], pre_list[idx], P, pre_list[idx + j]);
			//printf("pre_list[%d]: ", idx + j); cotnum(pre_list[idx + j], stdout);
		}
	}
}

// R = y1^r1 * y2^r2 * ... * yn^rn mod P;
void powmodn_Bin_gl(int n, big *r, big *y, big P, big &R)
{
	int i, j = 0, tmp;
	DWORD last32,
		*w = new DWORD[n];
	prepowmodn_Bin(n, y, gl_bigs, P);

	i = r[n - 1]->len - 1;
	last32 = r[n - 1]->w[i];
	while (last32 >> j && j != 32) j++;

	R->len = 1; R->w[0] = 1;
	for (--j; i >= 0; i--, j = 31) {
		tmp = 0;
		for (int ii = 0; ii < n; ii++) w[ii] = r[ii]->w[i];
		while (j) {
			for (int ii = 0; ii < n; ii++) {
				tmp += ((w[ii] >> j) & 1) << ii;
			}
			//printf("index: %d\n", tmp);
			mulmod(R, R, P, R);
			//cout << "R^2: "; cotnum(R, stdout);
			if (tmp) mulmod(R, gl_bigs[tmp], P, R);
			//cout << "R: "; cotnum(R, stdout);
			j--;
			tmp = 0;
		}
		for (int ii = 0; ii < n; ii++) {
			tmp += (w[ii] & 1) << ii;
		}
		//printf("tmp: %d\n", tmp);
		mulmod(R, R, P, R);
		//cout << "R^2: "; cotnum(R, stdout);
		if (tmp) mulmod(R, gl_bigs[tmp], P, R);
		//cout << "R: "; cotnum(R, stdout);
		//cout << endl;
	}
	tmp = 1 << n;
}

#define TESTn 5000
#define N 5
void testnBin(big P, csprng &Rng)
{
	big k = mirvar(0);
	big	R = mirvar(0), R1 = mirvar(0), R2 = mirvar(0);
	big r[N], y[N];
	for (int i = 0; i < N; i++) {
		r[i] = mirvar(1);
		y[i] = mirvar(1);
	}
	strong_bigrand(&Rng, P, y[0]);
	cout << "P: "; cotnum(P, stdout);
	//k = mirvar(0x69D); y[0] = mirvar(3);
	//nxprime(k, P);
	//char sk[51] = "7214642831191146504103114534140063683838032640121";
	//cinstr(k, sk);
	//char g[51] = "2843960000033430704672362122080271010298877412814";
	//cinstr(y[0], g);

	int count = 0;
	for (int i = 0; i < TESTn; i++) {
		strong_bigrand(&Rng, P, k);
		ShamirDecomposit_n(N, k, y[0], r, y, P);
		cout << "k: "; cotnum(k, stdout);
		cout << "r0: "; cotnum(r[0], stdout);
		cout << "r1: "; cotnum(r[1], stdout);
		cout << "r2: "; cotnum(r[2], stdout);
		cout << "y0: "; cotnum(y[0], stdout);
		cout << "y1: "; cotnum(y[1], stdout);
		cout << "y2: "; cotnum(y[2], stdout);

		powmodn_Bin(N, r, y, P, R1);
		powmodn(N, y, r, P, R2);
		powmod(y[0], k, P, R);
		cout << "R2: "; cotnum(R2, stdout);
		cout << "R1: "; cotnum(R1, stdout);
		cout << "R : "; cotnum(R, stdout);
		count += !compare(R, R1);
	}
	printf("correct: %d\n\n", count);
	for (int i = 0; i < N; i++) {
		mirkill(r[i]);
		mirkill(y[i]);
	}
	mirkill(R); mirkill(R1);mirkill(R2); mirkill(k);
}

#define REPEATn 10
void compare_binns(big P, csprng &Rng, Result &res)
{
	stopWatch timer1, timer2, timer3, timer4;
	LONGLONG dur1, min1 = LONG_MAX,
		dur2, min2 = LONG_MAX,
		dur3, min3 = LONG_MAX,
		dur4, min4 = LONG_MAX;

	big	g = mirvar(1), k = mirvar(1),
		R = mirvar(1), R1 = mirvar(1),
		R2 = mirvar(1), R3 = mirvar(1);
	big y[N], r[N];
	for (int i = 0; i < N; i++) {
		y[i] = mirvar(1);
		r[i] = mirvar(1);
	}

	strong_bigrand(&Rng, P, g);

	int count1 = 0, count2 = 0;
	for (int i = 0; i < TESTn; i++) {
		strong_bigrand(&Rng, P, k);
		ShamirDecomposit_n(3, k, g, r, y, P);

		for (int i = 0; i < REPEATn; i++) {
			startTimer(&timer1);
			powmod3_Bin(r, y, P, R1);
			stopTimer(&timer1);
			dur1 = getTickCount(&timer1);
			min1 = (min1 < dur1) ? min1 : dur1;

			startTimer(&timer2);
			powmodn_Bin_gl(3, r, y, P, R2);
			stopTimer(&timer2);
			dur2 = getTickCount(&timer2);
			min2 = (min2 < dur2) ? min2 : dur2;

			startTimer(&timer3);
			powmodn_Bin(3, r, y, P, R3);
			stopTimer(&timer3);
			dur3 = getTickCount(&timer3);
			min3 = (min3 < dur3) ? min3 : dur3;

			startTimer(&timer4);
			powmodn(3, y, r, P, R);
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
	res.t[0] /= TESTn;
	res.t[1] /= TESTn;
	res.t[2] /= TESTn;
	res.t[3] /= TESTn;
	res.p[1] = (res.t[1] / res.t[0]) * 100;
	res.p[2] = (res.t[2] / res.t[0]) * 100;
	res.p[3] = (res.t[3] / res.t[0]) * 100;

	for (int i = 0; i < N; i++) {
		mirkill(y[i]);
		mirkill(r[i]);
	}
	mirkill(g); mirkill(k);
	mirkill(R1); mirkill(R2); mirkill(R3); mirkill(R);
}