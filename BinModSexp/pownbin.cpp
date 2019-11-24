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

#define TESTn 1
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
		//cout << "k: "; cotnum(k, stdout);
		//cout << "r0: "; cotnum(r[0], stdout);
		//cout << "r1: "; cotnum(r[1], stdout);
		//cout << "r2: "; cotnum(r[2], stdout);
		//cout << "y0: "; cotnum(y[0], stdout);
		//cout << "y1: "; cotnum(y[1], stdout);
		//cout << "y2: "; cotnum(y[2], stdout);

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