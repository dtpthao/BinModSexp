#include "ShamirMul.h"

void ShamirDecomposit(big k, big a, big b, big X, big Y, big P)
{
	if (k->len == 0) return;
	DWORD len, i = 31;
	big tmp2l = mirvar(1);
	while (!(k->w[k->len - 1] & (1 << i))) i--; 
	len = (k->len << 4) - ((31 - i) >> 1) - !(i & 1); // half bit length of k
	sftbit(tmp2l, len, tmp2l);
	copy(k, a);
	divide(a, tmp2l, b); // bit length of a <= bit length of b
	copy(X, Y);
	for (i = 0; i < len; i++) {
		multiply(Y, Y, Y);
		divide(Y, P, P);
	}
	mirkill(tmp2l);
}

// R = g^k = y0^r0 * y1^r1 * y2^r2
// Bit length of r[i] (i = [0, 1]) is greater than bit length of r2 by 1 
// ==> seems a stupid idea. I keep it here to remind me not do it again.
void ShamirDecomposit3(big k, big r1, big r2, big r3, big y1, big y2, big y3, big P)
{
	if (k->len == 0) return;
	DWORD l, i = 31, l1, l2, l3;
	big sft1 = mirvar(1),
		sft2 = mirvar(1);

	while (!(k->w[k->len - 1] & (1 << i))) i--;
	l = (k->len << 5) - (31 - i);  // bit lenght of k
	l1 = l2 = l3 = l / 3;
	if ((l % 3) == 2) l1 = ++l2;
	if ((l % 3)== 1) l1++;

	sftbit(sft1, l1, sft1);
	sftbit(sft2, l2, sft2);

	copy(k, r1);
	divide(r1, sft1, r2);
	divide(r2, sft2, r3);

	copy(y1, y2);	
	for (i = 0; i < l1; i++) {
		multiply(y2, y2, y2);
		//cout << "y2: "; cotnum(y2, stdout);
		divide(y2, P, P);
		//cout << "y2: "; cotnum(y2, stdout);
	}
	copy(y2, y3);
	for (i = 0; i < l2; i++) {
		multiply(y3, y3, y3);
		divide(y3, P, P);
	}
	mirkill(sft1); mirkill(sft2);
}

// R = g^k = y0^r0 * y1^r1 * y2^r2
// Bit length of r2 is greater than bit length of r0, r1 by bitlen of k mod 3
// This is currently effective.
void ShamirDecomposit3(big k, big g, big *r, big *y, big P)
{
	if (k->len == 0) return;
	DWORD l, i = 31, l1, l2, l3;
	big sft = mirvar(1);
	for (int ii = 0; ii < 3; ii++) {
		r[ii] = mirvar(1);
		y[ii] = mirvar(1);
	}

	while (!(k->w[k->len - 1] & (1 << i))) i--;
	l = (k->len << 5) - (31 - i);  //lenght of "k"
	l1 = l2 = l3 = l / 3;
	l3 += l % 3;

	sftbit(sft, l1, sft);
	copy(k, r[0]);
	divide(r[0], sft, r[1]);
	divide(r[1], sft, r[2]);

	copy(g, y[0]);
	copy(y[0], y[1]);
	for (i = 0; i < l1; i++) {
		multiply(y[1], y[1], y[1]);
		//cout << "y1: "; cotnum(y1, stdout);
		divide(y[1], P, P);
		//cout << "y1: "; cotnum(y1, stdout);
	}
	copy(y[1], y[2]);
	for (i = 0; i < l2; i++) {
		multiply(y[2], y[2], y[2]);
		divide(y[2], P, P);
	}
	mirkill(sft);
}

// k = r0|r1|...|r(n-1)
void ShamirDecomposit_nk(int n, big k, big *r)
{
	if (k->len == 0) return;
	DWORD l, i = 31;
	big sft = mirvar(1);
	for (int ii = 0; ii < n; ii++) r[i] = mirvar(1);

	while (!(k->w[k->len - 1] & (1 << i))) i--;
	l = ((k->len << 5) - (31 - i)) / n;  //bit lenght of "k"/n

	sftbit(sft, l, sft);
	copy(k, r[0]);
	for (int i = 1; i < n; i++) {
		divide(r[i - 1], sft, r[i]);
	}
	mirkill(sft);
}

// y[i] = g^(2^(i*len/n)), i = [0, ..., (n-1)]
void ShamirDecomposit_ng(int len, int n, big g, big *y, big P)
{
	DWORD l, i = 31;
	for (int ii = 0; ii < n; ii++) y[i] = mirvar(1);

	l = len / n;  //bit lenght of "k"/n
	copy(g, y[0]);
	for (int i = 1; i < n; i++) {
		copy(y[i - 1], y[i]);
		for (int j = 0; j < l; j++) {
			multiply(y[i], y[i], y[i]);
			divide(y[i], P, P);
		}
	}
}

// Input : n, k, g
// Output: *r, *y
// k = r0|r1|...|r(n-1), 
// y[i] = g^(2^(i*L/n)), i = [0, ..., (n-1)], L - length bit of k
void ShamirDecomposit_n(int n, big k, big g, big *r, big *y, big P)
{
	if (k->len == 0) return;
	DWORD l, i;
	big sft = mirvar(1);
	for (i = 0; i < n; i++) {
		r[i] = mirvar(1);
		y[i] = mirvar(1);
	}
	i = 31;
	while (!(k->w[k->len - 1] & (1 << i))) i--;
	l = ((k->len << 5) - (31 - i))/n;  //bit lenght of "k" / n

	sftbit(sft, l, sft);
	copy(k, r[0]);
	copy(g, y[0]);
	for (i = 1; i < n; i++) {
		divide(r[i - 1], sft, r[i]);
		copy(y[i - 1], y[i]);
		for (int j = 0; j < l; j++) {
			multiply(y[i], y[i], y[i]);
			divide(y[i], P, P);
		}
	}
	mirkill(sft);
}