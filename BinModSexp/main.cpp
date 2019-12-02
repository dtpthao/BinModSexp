#include <iostream>
#include <ctime>
#include <iomanip>

extern "C" {
#include "miracl.h"
	FILE _iob[] = { *stdin, *stdout, *stderr };
	extern "C" FILE * __cdecl __iob_func(void) { return _iob; }
}
using namespace std;

#include "new_tests.h"
//extern big gl_bigs[100];

int main()
{
	srand(time(NULL));
	miracl *M = mirsys(100, 0);
	M->IOBASE = 16;
	csprng Rng;
	InitStrongRNG(&Rng);
	int m[5] = { 256, 384, 512, 1024, 1536};
	big P = mirvar(1);
	char sP[51] = "11692013098647223345629478661730264157247460344008";
	cinstr(P, sP);

	Result res[6];
	for (int i = 0; i < 100; i++) gl_bigs[i] = mirvar(0);

	for (int i = 0; i < 5; i++) {
		bigdig(m[i], 2, P);
		nxprime(P, P);
		//cout << "m = " << m[i] << endl;
		//test2Bin(P, Rng);
		//test3Bin_2(P, Rng);
		//testnBin(P, Rng);
		//test_powmoddJSF(P, Rng);
		//compares(m[i], P, Rng, res[i]);
		compare_binns(P, Rng, res[i]);
		//compare_GenJSFs(P, Rng);
		//test_HammingWeight_dJSF(P, Rng);
		//cout << endl;
	}
	//cout << setw(19) << "" << "n=1           n=2           n=3           n=4           n=5        lib(n=4)\n";
	//cout << setw(19) << "" <<   "Bin1          Bin2      Bin2_gblvar         xxx           xxx          lib1\n";
	//printcompares(res, m);
	cout << setw(19) << "" <<   "Bin3       Binn3gl         Binn3          lib3\n";
	printBins(res, m);

	//test_GenJSF(P, Rng);
	//test_GendJSF(P, Rng);
	//bigdig(1024, 2, P);
	//nxprime(P, P);
	//test_powmoddJSF(P, Rng);

	for (int i = 0; i < 100; i++) mirkill(gl_bigs[i]);

	mirkill(P);
	mirexit();
	system("pause");
	return 0;
}

int old_main() 
{
	srand(time(NULL));
	miracl *M = mirsys(100, 0);
	M->IOBASE = 10;
	csprng Rng; 
	InitStrongRNG(&Rng);
	CONSTANTS_F2m f2m[5] = { { 163, 7, 6, 3 }, { 233, 74, 0, 0 },
						{ 283, 12, 7, 5 }, { 409, 87, 0, 0 }, { 571, 10, 5, 2 } };
	Result res[6];
	big P = mirvar(1), X = mirvar(1);
	for (int i = 0; i < 5; i++) {
		getGF(P, f2m[i]);
		//cout << "P: "; cotnum(P, stdout);
		//cout << "prime: " << nxprime(P, P) << endl;
		//cout << "P: "; cotnum(P, stdout);
		//strong_bigrand(&Rng, P, X);
		//testMulBin(P, Rng);
		//testMul2Bin(P, Rng);
		test3Bin(P, Rng);
		//Test(Rng, X, P, res[i]);
		//testJSF(P, Rng);
	}
	//cout << setw(19) << "" << "X^k        X^a + Y^b        JSF	lib\n";
	//printBinOption(res, f2m);

	mirkill(P); mirkill(X);
	mirexit();
	system("pause");
	return 0;
}