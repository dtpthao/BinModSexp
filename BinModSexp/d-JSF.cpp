#include "JSF.h"

// Generate d-dimensional Simple JSF Form
// d < 32
DWORD GendJSF(int d, big *r, char **dJSF)
{
	int i, j, a0, a1, tmp;
	int A[2];
	DWORD lenJSF;

	i = j = lenJSF = a0 = A[0] = 0;
	bool loop = false;
	for (; i < d; i++) {
		copy(r[i], gl_bigs[i]);
		if (gl_bigs[i]->w[0] & 1) A[a0] += 1 << i;
		loop |= !(gl_bigs[i]->len == 1 && gl_bigs[i]->w[0] == 0 || gl_bigs[i]->len == 0);
	}

	while (loop) {
		a1 = a0 ^ 1;
		A[a1] = 0;
		for (i = 0; i < d; i++) {
			dJSF[i][lenJSF] = gl_bigs[i]->w[0] & 1;
			if (gl_bigs[i]->w[0] & 2) A[a1] += 1 << i;
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
			sftbit(gl_bigs[i], -1, gl_bigs[i]);
			if (dJSF[i][lenJSF] == -1) {
				if (gl_bigs[i]->len == 0) gl_bigs[i]->len = 1;
				if (gl_bigs[i]->w[0] ^ 0xffffffff) gl_bigs[i]->w[0]++;
				else incr(gl_bigs[i], 1, gl_bigs[i]);
			}
			loop |= !(gl_bigs[i]->len == 1 && gl_bigs[i]->w[0] == 0 || gl_bigs[i]->len == 0);
		}
		lenJSF++;
		a0 = a1;
	}

	return lenJSF;
}

void prePowMod_dJSF(int d, int len, big *y, big P, big *plist)
{
	int i, j, k, i0 = len >> 1, upi, downi;

	for (i = 0, j = 1; i < d; i++, j *= 3) {
		upi = i0 + j;
		downi = i0 - j;
		copy(y[i], plist[downi]);								// y[i]
		copy(y[i], plist[upi]);
		xgcd(plist[upi], P, plist[upi], plist[upi], plist[upi]);// 1/y[i]
		//printf("indexes: %d\t%d\n", upi, downi);
		for (k = 1; k <= (j >> 1); k++) {
			mulmod(plist[upi], plist[i0 + k], P, plist[upi + k]);
			mulmod(plist[upi], plist[i0 - k], P, plist[upi - k]);
			mulmod(plist[downi], plist[i0 + k], P, plist[downi + k]);
			mulmod(plist[downi], plist[i0 - k], P, plist[downi - k]);
			//printf("indexes: %d\t%d\t%d\t%d\n", upi + k, upi - k, downi + k, downi - k);
		}
	}
}

// R = y1^r1 * y2^r2 * ... * yn^rd mod P;
void powmod_dJSF(int d, big *y, big *r, big P, big &R)
{
	int tmp = 1, I0, idx = 0, i, j;
	char **dJSF = new char*[d]; 
	for (i = 0; i < d; i++) dJSF[i] = new char[1600];
	DWORD lendJSF;
	for (i = 0; i < d; i++) tmp *= 3;
	
	I0 = tmp >> 1;
	// gl_bigs is used inside GendJSF, hence calling GendJSF before prePowMod_dJSF
	lendJSF = GendJSF(d, r, dJSF);
	prePowMod_dJSF(d, tmp, y, P, gl_bigs);
	R->len = 1; R->w[0] = 1;
	for (i = lendJSF - 1; i >= 0; i--) {
		idx = I0;
		for (j = 0, tmp = 1; j < d; j++, tmp *= 3) {
			idx -= tmp * dJSF[j][i];
		}
		mulmod(R, R, P, R);
		if (idx != I0) mulmod(R, gl_bigs[idx], P, R);
	}
	
	for (i = 0; i < d; i++) delete[] dJSF[i];
	delete[] dJSF;
}

void test_correctness_GendJSF(big P, csprng &Rng)
{
	const int d = 3;
	big *x = new big[d];
	big x2 = mirvar(0);
	DWORD lendJSF;

	char **dJSF = new char*[d];
	for (int i = 0; i < d; i++) {
		x[i] = mirvar(0);
		dJSF[i] = new char[200];
	}
	big k = mirvar(0x1ED627);

	strong_bigrand(&Rng, P, k);
	//strong_bigdig(&Rng, 9, 16, k);
	//char sk[10] = "F4C9F1076";
	//cinstr(k, sk);
	//cout << "k : "; cotnum(k, stdout);
	ShamirDecomposit_nk(d, k, x);
	lendJSF = GendJSF(d, x, dJSF);
	cout << lendJSF << endl;
	
	for (int i = 0; i < d; i++) {
		x2 = mirvar(0);
		cout << "JSF[" << i << "]: ";
		for (int j = lendJSF - 1; j >= 0; j--) {
			sftbit(x2, 1, x2);
			incr(x2, dJSF[i][j], x2);
			printf("%2d", dJSF[i][j]);
		}
		cout << endl;
		cotnum(x[i], stdout);
		cotnum(x2, stdout);
		mirkill(x2);
	}
	

	for (int i = 0; i < d; i++) {
		mirkill(x[i]);
		delete[] dJSF[i];
	}
	delete[] dJSF;
	mirkill(k);
}

void test_HammingWeight_dJSF(big P, csprng &Rng)
{
	big *x = new big[6];
		char **dJSF = new char*[6];
	for (int i = 0; i < 6; i++) {
		x[i] = mirvar(0);
		dJSF[i] = new char[1600];
	}
	big k = mirvar(0);
	DWORD lendJSF;
	int count = 0;
	bool b = 0;
	double weight = 0;
	const int TIMES = 20000;
	printf(" d | Testing times | Average hamming weight\n");
	for (int d = 1; d < 6; d++) {
		weight = 0;
		for (int i = 0; i < TIMES; i++) {
			strong_bigrand(&Rng, P, k);
			ShamirDecomposit_nk(d, k, x);
			lendJSF = GendJSF(d, x, dJSF);
			count = 0;
			for (int j = lendJSF - 1; j >= 0; j--) {
				b = 0;
				for (int ii = 0; ii < d; ii++) {
					b |= dJSF[ii][j];
				}
				if (!b) count++;
			}
			weight += (double)count / lendJSF;
		}
		weight /= TIMES;
		printf(" %d |     %5d     |      %f\n", d, TIMES, (1 - weight));
	}

	for (int i = 0; i < 6; i++) {
		mirkill(x[i]);
		delete[] dJSF[i];
	}
	delete[] dJSF;
	mirkill(k);
}

void test_powmoddJSF(big P, csprng &Rng)
{
	const int d = 3;
	big *r = new big[d];
	big *y = new big[d];
	big k = mirvar(0), g = mirvar(0);
	big Z = mirvar(0), Z1 = mirvar(0), Z2 = mirvar(0);
	DWORD lendJSF;
	char **dJSF = new char*[d];
	for (int i = 0; i < d; i++) {
		r[i] = mirvar(0);
		y[i] = mirvar(0);
		dJSF[i] = new char[300];
	}

	strong_bigrand(&Rng, P, g);
	int count = 0;
	//strong_bigdig(&Rng, 2, 16, g);
	//nxprime(g, P);
	//r[0] = mirvar(0x3);
	//r[1] = mirvar(0x2);
	//r[2] = mirvar(0x5);
	//y[0] = mirvar(0x4);
	//y[1] = mirvar(0x5);
	//y[2] = mirvar(0x2);
	//P = mirvar(0xB);
//	char sP[300] = "BFF9F8B8706AAA8FC1AE08E4A6C2B85D026748CCFCB092162BCF1FF6169417375AEDADFC015\
//90DEF9F474505A7C46DC24EDDBCD1CF3CD3AC8D6625774CE48C6E6963A020D0857A9F6976994C37A754F00267B82705\
//D81061CD281BB708712888623B6D2C45E6AFC6DDB84C51641D1FEFF6E598225E893F22577156E9E5C7235B";
//	char sg[260] = "39AAB800A5913F33EE72659ABC7FB7D9059E58838D5BACA1AC4EF7A26D5F02D974C3E007991\
//CAC954C55E7710259DA28691CDD4684387938918BAE3221760FA0B10745D4ACF6A4D2705849B639CA8A9633CB4E80EC\
//BCF490A2BCA2AA182E9398F15D4BA57985D1FECBDCEE9433B48F426D0988625DD71A7CC43C2C803082F940";
//	char sk[260] = "7C446723D62968564DBD4C0C8F457A3C210A694FBD25C306B6E3892833BD26A348041FEEC75\
//342D962A5C60A93B37F03EDCDA7D9612D53C44CD16469F2883D41B00A2AF49F4C9AD7BD82D01F6DD7D4B14AE0189341\
//875407AD80F18F957F62AB553C211126E0B31500856B129E49B6E90DC490881204F6304AC61755D2905501";
//	cinstr(P, sP);
//	cinstr(g, sg);
//	cinstr(k, sk);
	for (int i = 0; i < 5000; i++) {
		//strong_bigdig(&Rng, 2, 16, k);
		strong_bigrand(&Rng, P, k);
		ShamirDecomposit_n(d, k, g, r, y, P);
		powmod_dJSF(d, y, r, P, Z);
		powmodn(d, y, r, P, Z1);
		count += !compare(Z, Z1);
		//cout << "P : "; cotnum(P, stdout);
		//cout << "g : "; cotnum(g, stdout);
		//cout << "k : "; cotnum(k, stdout);
		//for (int j = 0; j < d; j++) { cout << "ri: "; cotnum(r[j], stdout); }
		//for (int j = 0; j < d; j++) { cout << "yi: "; cotnum(y[j], stdout); }
		//cout << "Z : "; cotnum(Z, stdout);
		//cout << "Z1: "; cotnum(Z1, stdout);
		//if (compare(Z, Z1)) {
		//	cout << "k: "; cotnum(k, stdout);
		//	cout << "a: "; cotnum(a, stdout);
		//	cout << "b: "; cotnum(b, stdout);
		//	cout << "X: "; cotnum(X, stdout);
		//	cout << "Y: "; cotnum(Y, stdout);
		//	cout << "P: "; cotnum(P, stdout);
		//	cout << "Z : "; cotnum(Z, stdout);
		//	cout << "Z1: "; cotnum(Z1, stdout);
		//	powmod2_Bin(X, a, Y, b, P, Z2);
		//	cout << "Z2: "; cotnum(Z2, stdout);
		//	break;
		//}
	}

	for (int i = 0; i < d; i++) {
		mirkill(r[i]);
		mirkill(y[i]);
	}
	printf("%d\n", count);
	mirkill(k);mirkill(g);
	mirkill(Z); mirkill(Z1); mirkill(Z2);
}


