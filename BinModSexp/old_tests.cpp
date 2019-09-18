#include "old_tests.h"

void Test(csprng &Rng, big X, big P, Result &res)
{
	big Z = mirvar(0),
		Z1 = mirvar(0), Z2 = mirvar(0), Z3 = mirvar(0);
	big k = mirvar(1), Y = mirvar(1);

	res.t[0] = res.t[1] = res.t[2] = res.t[3] = 0;
	big a = mirvar(1), b = mirvar(1);
	for (int i = 0; i < TEST; i++) {
		strong_bigrand(&Rng, P, k);

		if (k->len == 0 || (k->len == 1 && k->w[0] == 1)) {
			i--;
			continue;
		}
		duration1(X, k, P, Z, powmod, res.t[LIB]);

		duration1(X, k, P, Z1, powmod_Bin, res.t[0]);
		//duration1(X, k, P, Z1, nrespowmod_Bin, res.t[0]);
		res.c[0] += !compare(Z1, Z);

		//duration2(X, k, P, Z2, powmod2_Bin, res.t[1]);
		duration1(X, k, P, Z2, powmod_ShrBin, res.t[1]);
		res.c[1] += !compare(Z2, Z);

		//duration2(X, k, P, Z1, powmod2_JSF, res.t[2]);
		//duration1(X, k, P, Z3, powmod_ShrJSF, res.t[2]);
		durationJSF(X, k, P, Z3, res.t[2]);
		res.c[2] += !compare(Z3, Z);
	}
	res.t[0] /= TEST;
	res.p[0] = 100;
	for (int i = 1; i <= LIB; i++) {
		res.t[i] /= TEST;
		res.p[i] = (res.t[i] / res.t[0]) * 100;
	}
	mirkill(Z); mirkill(Z1);
	mirkill(Z2); mirkill(Z3);
	mirkill(k); mirkill(Y);
	mirkill(a); mirkill(b);
}

#define NUM_OF_P 5
void printBinOption(Result res[NUM_OF_P + 1], CONSTANTS_F2m *f2m)
{
	res[NUM_OF_P].p[0] = 100;
	res[NUM_OF_P].p[1] /= NUM_OF_P;
	res[NUM_OF_P].p[2] /= NUM_OF_P;

	for (int i = 1; i <= LIB; i++) {
		res[NUM_OF_P].p[i] = res[0].p[i] + res[1].p[i]
			+ res[2].p[i] + res[3].p[i] + res[4].p[i];
		res[NUM_OF_P].p[i] /= NUM_OF_P;
	}
	for (int i = 0; i < NUM_OF_P; i++) {
		printf("%2d | %4d: %13.3f %13.3f %13.3f %13.3f\n",
			i, f2m[i].m, res[i].t[0], res[i].t[1], res[i].t[2], res[i].t[LIB]);
	}
	cout << endl;
	for (int i = 0; i < NUM_OF_P; i++) {
		printf("%2d | %4d: %12.1f%% %12.1f%% %12.1f%% %12.1f%%\n",
			i, f2m[i].m, res[i].p[0], res[i].p[1], res[i].p[2], res[i].p[LIB]);
	}
	printf("    avg  : %12.1f%% %12.1f%% %12.1f%% %12.1f%%\n",
		res[NUM_OF_P].p[0], res[NUM_OF_P].p[1], res[NUM_OF_P].p[2], res[NUM_OF_P].p[LIB]);
	cout << endl;
	for (int i = 0; i < NUM_OF_P; i++) {
		printf("%2d | %4d: %13d %13d %13d %13d\n",
			i, f2m[i].m, res[i].c[0], res[i].c[1], res[i].c[2], res[i].c[LIB]);
	}
}