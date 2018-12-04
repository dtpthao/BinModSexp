#ifndef _JSF_H
#define _JSF_H

#include "Field.h"
#include "ShamirMul.h"
#include "Bin.h"

inline void subGenJSF(big Var1, big Var2, char &JSFi, bool &d, big RS);	//in use
DWORD GenJSF(big R, big S, char *JSFr, char *JSFs);						//in use


inline void prePowModJSF(big X, big Y, big P, big *lst);						//in use
void powmod2_JSF(big X, big a, big Y, big b, big P, big Z);	//in use
void powmod_ShrJSF(big X, big k, big P, big Z);
void powmod_JSF(big *lst, big X, big a, big Y, big b, big P, big Z);

void testJSF(big P, csprng &Rng);

#endif // !_JSF_H

