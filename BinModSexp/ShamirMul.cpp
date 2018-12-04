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
		//power(Y, 2, P, Y);
		multiply(Y, Y, Y);
		divide(Y, P, P);
	}
	mirkill(tmp2l);
}

