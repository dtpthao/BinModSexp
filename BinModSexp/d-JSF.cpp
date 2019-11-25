#include "JSF.h"

// Generate d-dimensional Simple JSF Form
// d < 32
DWORD GendJSF(int d, big *r, int **dJSF)
{
	int i, j, a0, a1, tmp;
	int A[2];
	DWORD lenJSF;
	big *x = new big[d];

	i = j = lenJSF = a0 = A[0] = 0;
	bool loop = false;
	for (; i < d; i++) {
		x[i] = mirvar(0);
		copy(r[i], x[i]);
		if (x[i]->w[0] & 1) A[a0] += 1 << i;
		loop |= !(x[i]->len == 1 && x[i]->w[0] == 0 || x[i]->len == 0);
	}

	while (loop) {
		a1 = a0 ^ 1;
		A[a1] = 0;
		for (i = 0; i < d; i++) {
			dJSF[i][lenJSF] = x[i]->w[0] & 1;
			if (x[i]->w[0] & 2) A[a1] += 1 << i;
		}

		/* check if A[a1] is a subset of A[a0] */
		if ((A[a1] | A[a0]) == A[a0]) {
			tmp = A[a1];
			A[a1] = 0;
		}
		else { /* relative complement A[a0]\A[a1] */
			tmp = (A[a0] | A[a1]) ^ A[a1];
			A[a1] |= A[a0];
		}

		i = 0;
		while (tmp) {
			if (tmp & 1) dJSF[i][lenJSF] = -dJSF[i++][lenJSF];
			tmp >>= 1;
		}

		loop = false;
		for (i = 0; i < d; i++) {
			sftbit(x[i], -1, x[i]);
			if (dJSF[i][lenJSF] == -1) incr(x[i], 1, x[i]);
			loop |= !(x[i]->len == 1 && x[i]->w[0] == 0 || x[i]->len == 0);
		}
		lenJSF++;
		a0 = a1;
	}
	for (i = 0; i < d; i++) mirkill(x[i]);

	return lenJSF;
}

void prePowMod_dJSF(int d, int len, big *y, big P, big *plist)
{
	int i, j, k, i0 = len/2, upi, downi;
	int *idxs = new int[d];
	//for (i = 0, tmp = 1; i < d; i++, tmp *= 3) {
	//	idxs[i] = idx0 - tmp;
	//	idx = idx0 + tmp;
	//	copy(y[i], plist[idx0 - tmp]);
	//	copy(y[i], plist[idx]);
	//	xgcd(plist[idx], P, plist[idx], plist[idx], plist[idx]);	// 1/y[i] mod P
	//}
	//for (i = 1, j = 3; i < d; i++, j *= 3) {
	//	tmp = j/2;
	//	for (k = 1; k <= tmp; k++) {
	//		//copy(plist[idxs[i]], plist[idx0 - k]);
	//		mulmod(plist[idxs[i]], plist[idx0 + k], P, plist[idxs[i] + k]);
	//		mulmod(plist[idxs[i]], plist[idx0 - k], P, plist[idxs[i] - k]);
	//	}
	//}
	for (i = 0, j = 1; i < d; i++, j *= 3) {
		upi = i0 + j;
		downi = i0 - j;
		copy(y[i], plist[downi]);								// y[i]
		copy(y[i], plist[upi]);
		xgcd(plist[upi], P, plist[upi], plist[upi], plist[upi]);// 1/y[i]
		for (k = 1; k < (j >> 1); k++) {
			mulmod(plist[upi], plist[i0 + k], P, plist[upi + k]);
			mulmod(plist[upi], plist[i0 - k], P, plist[upi - k]);
			mulmod(plist[downi], plist[i0 + k], P, plist[downi + k]);
			mulmod(plist[downi], plist[i0 - k], P, plist[downi - k]);
		}
	}
}

// R = y1^r1 * y2^r2 * ... * yn^rd mod P;
void powmod_dJSF(int d, big *r, big *y, big P, big &R)
{
	int tmp = 1, idx0, idx = 0, i, j;
	int **dJSF = new int*[d]; 
	for (i = 0; i < d; i++) dJSF[i] = new int[300];
	DWORD lendJSF;
	for (i = 0; i < d; i++) tmp *= 3;
	big *plist = new big[tmp];
	
	idx0 = tmp / 2;
	prePowMod_dJSF(d, idx0, y, P, plist);
	lendJSF = GendJSF(d, r, dJSF);
	R->len = 1; R->w[0] = 0;
	for (i = lendJSF - 1; i >= 0; i--) {
		idx = idx0;
		for (j = 0, tmp = 1; j < d; j++, tmp *= 3) {
			idx -= tmp * dJSF[j][i];
		}
		mulmod(R, R, P, R);
		if (idx != idx0) mulmod(R, plist[idx], P, R);
	}
}

void test_GendJSF(big P, csprng &Rng)
{
	const int d = 3;
	big *x = new big[d];
	big x2 = mirvar(0);
	DWORD lendJSF;
	//x[0] = mirvar(8);
	//x[1] = mirvar(7);
	//x[2] = mirvar(5);
	//x[3] = mirvar(4);

	int **dJSF = new int*[d];
	for (int i = 0; i < d; i++) {
		x[i] = mirvar(0);
		dJSF[i] = new int[200];
	}
	big k = mirvar(0);
	strong_bigrand(&Rng, P, k);
	ShamirDecomposit_nk(d, k, x);
	lendJSF = GendJSF(d, x, dJSF);
	cout << lendJSF << endl;
	for (int i = 0; i < d; i++) {
		//cotnum(x[i], stdout);
		x2 = mirvar(0);
		cout << "JSF[" << i << "]: ";
		for (int j = lendJSF - 1; j >= 0; j--) {
			sftbit(x2, 1, x2);
			incr(x2, dJSF[i][j], x2);
			printf("%2d", dJSF[i][j]);
		}
		cout << endl;
		//cotnum(x2, stdout);
		mirkill(x[i]);
		mirkill(x2);
	}
	mirkill(k);
}

