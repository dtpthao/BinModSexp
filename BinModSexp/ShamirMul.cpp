#include "ShamirMul.h"

void ShamirDecomposit(big k, big a, big b, big X, big Y, big P)
{
	if (k->len == 0) return;
	DWORD len, i = 31;
	big tmp2l = mirvar(1);
	while (!(k->w[k->len - 1] & (1 << i))) i--; //
	len = (k->len << 4) - ((31 - i) >> 1) - !(i & 1);
	sftbit(tmp2l, len, tmp2l);
	copy(k, a);
	divide(a, tmp2l, b);
	copy(X, Y);
	for (i = 0; i < len; i++) {
		multiply(Y, Y, Y);
		divide(Y, P, P);
	}
	mirkill(tmp2l);
}

/*
 * Bit length of r1, r2 depend on bit length of k
 */
// R = g^k = y1^r1 * y2^r2 * y3^r3
void ShamirDecomposit3(big k, big r1, big r2, big r3, big y1, big y2, big y3, big P)
{
	if (k->len == 0) return;
	DWORD l, i = 31, l1, l2, l3;
	big sft1 = mirvar(1),
		sft2 = mirvar(1);

	while (!(k->w[k->len - 1] & (1 << i))) i--;
	l = (k->len << 5) - (31 - i);  //lenght of "k"
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

void ShamirDecomposit3_2(big k, big g, big *r, big *y, big P)
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

void ShamirDecomposit_n(int n, big k, big g, big *r, big *y, big P)
{
	if (k->len == 0) return;
	DWORD l, i = 31;
	big sft = mirvar(1);
	for (int ii = 0; ii < n; ii++) {
		r[i] = mirvar(1);
		y[i] = mirvar(1);
	}

	while (!(k->w[k->len - 1] & (1 << i))) i--;
	l = ((k->len << 5) - (31 - i))/n;  //bit lenght of "k"/n

	sftbit(sft, l, sft);
	copy(k, r[0]);
	copy(g, y[0]);
	for (int i = 1; i < n; i++) {
		divide(r[i - 1], sft, r[i]);
		copy(y[i - 1], y[i]);
		for (int j = 0; j < l; j++) {
			multiply(y[i], y[i], y[i]);
			divide(y[i], P, P);
		}
	}
	mirkill(sft);
}