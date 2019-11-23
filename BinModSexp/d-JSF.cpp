#include "JSF.h"

// Generate d-dimensional Simple JSF Form
// d < 32
DWORD GendJSF(int d, big *x, char **dJSF)
{
	int i, j, a0, a1, tmp;
	int A[2];
	DWORD lenJSF;

	i = j = lenJSF = a0 = 0;
	bool loop = false;
	for (; i < d; i++) {
		if (x[i]->w[0] & 1) A[a0] += 1 << i;
		loop |= !(x[i]->len == 1 && x[i]->w[0] == 0);
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
		for (int i = 0; i < d; i++) {
			sftbit(x[i], -1, x[i]);
			loop |= !(x[i]->len == 1 && x[i]->w[0] == 0);
		}
		lenJSF++;
		a0 = a1;
	}

	return lenJSF;
}