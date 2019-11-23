#include "JSF.h"

// Generate d-dimensional Simple JSF Form
// d < 32
DWORD GendJSF(int d, big *k, int **dJSF)
{
	int i, j, a0, a1, tmp;
	int A[2];
	DWORD lenJSF;
	big *x = new big[d];

	i = j = lenJSF = a0 = A[0] = 0;
	bool loop = false;
	for (; i < d; i++) {
		x[i] = mirvar(0);
		copy(k[i], x[i]);
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
			if (tmp & 1) dJSF[i][lenJSF] = -dJSF[i][lenJSF];
			tmp >>= 1;
			i++;
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

void test_GendJSF(big P, csprng &Rng)
{
	const int d = 4;
	big *x = new big[d];
	big x2 = mirvar(0);
	DWORD lendJSF = 5;
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

	for (int i = 0; i < d; i++) {
		cotnum(x[i], stdout); fflush(stdout);
		x2 = mirvar(0);
		cout << "JSF[" << i << "]: ";
		for (int j = lendJSF - 1; j >= 0; j--) {
			sftbit(x2, 1, x2);
			incr(x2, dJSF[i][j], x2);
			printf("%2d", dJSF[i][j]);
		}
		cout << endl;
		cotnum(x2, stdout);
		mirkill(x[i]);
		mirkill(x2);
	}
	mirkill(k);
}