#ifndef _JSF_H
#define _JSF_H

#include "Field.h"
#include "shr_decomp.h"
#include "Bin.h"

/*=========================================== JSF.cpp ===========================================*/
DWORD GenJSF(big R, big S, char *JSFr, char *JSFs);

inline void prePowModJSF(big X, big Y, big P, big *lst);
void powmod_JSF(big X, big a, big Y, big b, big P, big Z);

/*______________________don't use these functions____________________________*/
inline void subGenJSF(big Var1, big Var2, char &JSFi, bool &d, big RS);
DWORD GenJSF2(big R, big S, char *JSFr, char *JSFs);

inline void prePowModJSF(big X, big X1, big Y, big Y1, big P, big *lst);
void powmod2_JSF(big X, big X1, big a, big Y, big Y1, big b, big P, big Z);
//void powmod_ShrJSF(big X, big k, big P, big Z);
//void powmod_JSF(big *lst, big a, big b, big P, big Z);
/*___________________________________________________________________________*/

void test_GenJSF(big P, csprng &Rng);
void compare_GenJSFs(big P, csprng &Rng);
void testJSF(big P, csprng &Rng);
void compare_JSFs(big P, csprng &Rng, Result &res);
/*===============================================================================================*/

/*========================================= d-JSF.cpp ===========================================*/
DWORD GendJSF(int d, big *r, char **dJSF);
void prePowMod_dJSF(int d, int len, big *y, big P, big *plist);
void powmod_dJSF(int d, big *y, big *r, big P, big &R);
void test_correctness_GendJSF(big P, csprng &Rng);
void test_HammingWeight_dJSF(big P, csprng &Rng);
void test_powmoddJSF(big P, csprng &Rng);
/*===============================================================================================*/

/*======================================= test_JSFs.cpp =========================================*/
void compare_prepowmodJSFs(big P, csprng &Rng, Result &res);
void printcompares_JSFs(Result res[], int *m);
/*===============================================================================================*/

#endif // !_JSF_H

