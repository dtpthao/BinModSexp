#ifndef _JSF_H
#define _JSF_H

#include "Field.h"
#include "shr_decomp.h"
#include "Bin.h"

inline void subGenJSF(big Var1, big Var2, char &JSFi, bool &d, big RS);	//in use
DWORD GenJSF(big R, big S, char *JSFr, char *JSFs);						//in use

inline void prePowModJSF(big X, big X1, big Y, big Y1, big P, big *lst);	//in use
void powmod2_JSF(big X, big X1, big a, big Y, big Y1, big b, big P, big Z);	//in use
void powmod_ShrJSF(big X, big k, big P, big Z);
void powmod_JSF(big *lst, big a, big b, big P, big Z);

void testJSF(big P, csprng &Rng);

/* d-JSF.cpp */
void test_GendJSF(big P, csprng &Rng);
void test_powmoddJSF(big P, csprng &Rng);

#endif // !_JSF_H

