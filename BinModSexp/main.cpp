#include <iostream>
#include <ctime>
#include <iomanip>

extern "C" {
#include "miracl.h"
	FILE _iob[] = { *stdin, *stdout, *stderr };
	extern "C" FILE * __cdecl __iob_func(void) { return _iob; }
}
using namespace std;

#include "old_tests.h"


int main()
{
	srand(time(NULL));
	miracl *M = mirsys(100, 0);
	M->IOBASE = 10;
	csprng Rng;
	InitStrongRNG(&Rng);
	CONSTANTS_F2m f2m[5] = { { 163, 7, 6, 3 }, { 233, 74, 0, 0 },
						{ 283, 12, 7, 5 }, { 409, 87, 0, 0 }, { 571, 10, 5, 2 } };
	big P, X;
	for (int i = 0; i < 5; i++) {
		getGF(P, f2m[i]);
		//testMul3Bin(P, Rng);
		testMul3Bin_2(P, Rng);
	}

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
		testMul3Bin(P, Rng);
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