#include "JSF.h"

#define TESTJSFs 5000
#define REPEATJSFs 10
void compare_prepowmodJSFs(big P, csprng &Rng, Result &res)
{
	stopWatch timer1, timer2, timer3, timer4, timer5, timer6;
	LONGLONG dur1, min1 = LONG_MAX,
		dur2, min2 = LONG_MAX,
		dur3, min3 = LONG_MAX,
		dur5, min5 = LONG_MAX,
		dur6, min6 = LONG_MAX,
		dur4, min4 = LONG_MAX;
	big a = mirvar(1), b = mirvar(1),
		X = mirvar(1), Y = mirvar(1), R = mirvar(1);
	big	g = mirvar(1), k = mirvar(1);
	big r[5], y[5];
	r[0] = mirvar(0); r[1] = mirvar(0); r[2] = mirvar(0); r[3] = mirvar(0); r[4] = mirvar(0); r[5] = mirvar(0);
	y[0] = mirvar(0); y[1] = mirvar(0); y[2] = mirvar(0); y[3] = mirvar(0); y[4] = mirvar(0); y[5] = mirvar(0);

	strong_bigrand(&Rng, P, g);
	copy(g, X);
	char **dJSF = new char*[5];
	for (int i = 0; i < 5; i++) {
		dJSF[i] = new char[1600];
	}

	int count1 = 0, count2 = 0;
	for (int i = 0; i < TESTJSFs; i++) {
		strong_bigrand(&Rng, P, k);

		for (int j = 0; j < REPEATJSFs; j++) {
			startTimer(&timer1);
			powmod_Bin(g, k, P, R);
			stopTimer(&timer1);
			dur1 = getTickCount(&timer1);
			min1 = (min1 < dur1) ? min1 : dur1;
		}

		ShamirDecomposit(k, a, b, X, Y, P);
		for (int j = 0; j < REPEATJSFs; j++) {
			startTimer(&timer2);
			//prePowModJSF(X, Y, P, gl_bigs);
			GenJSF(a, b, dJSF[0], dJSF[1]);
			stopTimer(&timer2);
			dur2 = getTickCount(&timer2);
			min2 = (min2 < dur2) ? min2 : dur2;
		}

		//ShamirDecomposit_n(3, k, g, r, y, P);
		ShamirDecomposit_nk(3, k, r);
		for (int j = 0; j < REPEATJSFs; j++) {
			startTimer(&timer3);
			//prePowMod_dJSF(3, 27, y, P, gl_bigs);
			GendJSF(3, r, dJSF);
			stopTimer(&timer3);
			dur3 = getTickCount(&timer3);
			min3 = (min3 < dur3) ? min3 : dur3;
		}

		//ShamirDecomposit_n(4, k, g, r, y, P);
		ShamirDecomposit_nk(4, k, r);
		for (int j = 0; j < REPEATJSFs; j++) {
			startTimer(&timer4);
			//prePowMod_dJSF(4, 81, y, P, gl_bigs);
			GendJSF(4, r, dJSF);
			stopTimer(&timer4);
			dur4 = getTickCount(&timer4);
			min4 = (min4 < dur4) ? min4 : dur4;
		}

		ShamirDecomposit_nk(5, k, r);
		//ShamirDecomposit_n(5, k, g, r, y, P);
		for (int j = 0; j < REPEATJSFs; j++) {
			startTimer(&timer5);
			//prePowMod_dJSF(5, 243, y, P, gl_bigs);
			GendJSF(5, r, dJSF);
			stopTimer(&timer5);
			dur5 = getTickCount(&timer5);
			min5 = (min5 < dur5) ? min5 : dur5;
		}
		
		copy(k, r[0]);
		for (int j = 0; j < REPEATJSFs; j++) {
			startTimer(&timer6);
			//prePowMod_dJSF(5, 243, y, P, gl_bigs);
			GendJSF(1, r, dJSF);
			stopTimer(&timer6);
			dur6 = getTickCount(&timer6);
			min6 = (min6 < dur6) ? min6 : dur6;
		}
		res.t[0] += min1;
		res.t[1] += min2;
		res.t[2] += min3;
		res.t[3] += min4;
		res.t[4] += min5;
		res.t[5] += min6;
	}
	res.t[0] /= TESTJSFs;
	res.t[1] /= TESTJSFs;
	res.t[2] /= TESTJSFs;
	res.t[3] /= TESTJSFs;
	res.t[4] /= TESTJSFs;
	res.t[5] /= TESTJSFs;
	res.p[1] = (res.t[1] / res.t[0]) * 100;
	res.p[2] = (res.t[2] / res.t[0]) * 100;
	res.p[3] = (res.t[3] / res.t[0]) * 100;
	res.p[4] = (res.t[4] / res.t[0]) * 100;
	res.p[5] = (res.t[5] / res.t[0]) * 100;

	mirkill(a); mirkill(b); mirkill(X); mirkill(Y);
	mirkill(r[0]); mirkill(r[1]); mirkill(r[2]); mirkill(r[3]); mirkill(r[4]);
	mirkill(y[0]); mirkill(y[1]); mirkill(y[2]); mirkill(y[3]); mirkill(y[4]);
	mirkill(g); mirkill(k);mirkill(R);
}

#define NUM_OF_P 5
void printcompares_JSFs(Result res[NUM_OF_P + 1], int *m)
{
	const int lib = 4;
	res[NUM_OF_P].p[0] = 100;

	for (int i = 0; i <= NUM_OF_P; i++) {
		res[i].p[0] = 100;
	}

	for (int i = 1; i <= lib; i++) {
		res[NUM_OF_P].p[i] = res[0].p[i] + res[1].p[i]
			+ res[2].p[i] + res[3].p[i] + res[4].p[i];
		res[NUM_OF_P].p[i] /= NUM_OF_P;
	}

	cout << setw(19) << "" << "Bin1         GenJSF       GenJSF3       GenJSF4       GenJSF5       GenJSF1\n";
	for (int i = 0; i < NUM_OF_P; i++) {
		printf("%2d | %4d: %13.3f %13.3f %13.3f %13.3f %13.3f %13.3f\n",
			i, m[i], res[i].t[0], res[i].t[1], res[i].t[2], res[i].t[3], res[i].t[4], res[i].t[lib]);
	}
	cout << endl;
	for (int i = 0; i < NUM_OF_P; i++) {
		printf("%2d | %4d: %12.1f%% %12.1f%% %12.1f%% %12.1f%% %12.1f%% %12.1f%%\n",
			i, m[i], res[i].p[0], res[i].p[1], res[i].p[2], res[i].p[3], res[i].p[4], res[i].p[lib]);
	}
	printf("    avg  : %12.1f%% %12.1f%% %12.1f%% %12.1f%% %12.1f%% %12.1f%%\n",
		res[NUM_OF_P].p[0], res[NUM_OF_P].p[1], res[NUM_OF_P].p[2], res[NUM_OF_P].p[3], res[NUM_OF_P].p[4], res[NUM_OF_P].p[lib]);
	cout << endl;
	cout << "Test times: " << TESTJSFs << endl;
}
