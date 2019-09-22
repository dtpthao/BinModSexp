#include "Duration.h"
#include <iostream>
#include "ShamirMul.h"
#include "JSF.h"

inline void startTimer(stopWatch *timer) {
	QueryPerformanceCounter(&timer->start);
}

inline void stopTimer(stopWatch *timer) {
	QueryPerformanceCounter(&timer->stop);
}

inline double LIToSecs(LARGE_INTEGER *L) {
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return ((double)L->QuadPart / (double)frequency.QuadPart);
}

inline double getElapsedTime(stopWatch *timer) {
	LARGE_INTEGER time;
	time.QuadPart = timer->stop.QuadPart - timer->start.QuadPart;
	return LIToSecs(&time);
}

inline LONGLONG getTickCount(stopWatch *timer) {
	return timer->stop.QuadPart - timer->start.QuadPart;
}


#define REPEAT 10
/*
 * R = kP
 */
void duration1(big X, big k, big P, big &Z, 
	void(*func) (big, big, big, big&), double &t)
{
	LONGLONG dur, min = LONG_MAX;
	stopWatch timer;

	for (int i = 0; i < REPEAT; i++) {
		startTimer(&timer);
		(*func)(X, k, P, Z);
		stopTimer(&timer);
		dur = getTickCount(&timer);
		min = (min < dur) ? min : dur;
	}
	t += min;
}

void duration1(big X, big k, big P, big &Z,
	void(*func) (big, big, big, big), double &t)
{
	LONGLONG dur, min = LONG_MAX;
	stopWatch timer;

	for (int i = 0; i < REPEAT; i++) {
		startTimer(&timer);
		(*func)(X, k, P, Z);
		stopTimer(&timer);
		dur = getTickCount(&timer);
		min = (min < dur) ? min : dur;
	}
	t += min;
}

void durationJSF(big X, big k, big P, big &Z, double &t)
{
	LONGLONG dur, min = LONG_MAX;
	big a = mirvar(1),
		b = mirvar(1),
		X1 = mirvar(1),
		Y = mirvar(1), Y1 = mirvar(1);
	big lst[9];
	stopWatch timer;
	for (int i = 0; i < 9; i++) lst[i] = mirvar(1);
	ShamirDecomposit(k, a, b, X, Y, P);
	copy(X, X1); copy(Y, Y1);
	xgcd(X1, P, X1, X1, X1);
	xgcd(Y1, P, Y1, Y1, Y1);
	//prePowModJSF(X, X1, Y, Y1, P, lst);
	for (int i = 0; i < REPEAT; i++) {
		startTimer(&timer);
		ShamirDecomposit(k, a, b, X, Y, P);
		prePowModJSF(X, X1, Y, Y1, P, lst);
		powmod_JSF(lst, a, b, P, Z);
		stopTimer(&timer);
		dur = getTickCount(&timer);
		min = (min < dur) ? min : dur;
	}
	t += min;
	for (int i = 0; i < 9; i++) mirkill(lst[i]);
	mirkill(a); mirkill(b);
	mirkill(X1); mirkill(Y1); mirkill(Y);
}

/*
 * R = kP = aP + bQ
 */
void duration2(big X, big k, big P, big &Z,
	void(*func) (big, big, big, big, big, big&), double &t)
{
	LONGLONG dur, min = LONG_MAX;
	big a = mirvar(1),
		b = mirvar(1),
		Y = mirvar(1);
	stopWatch timer;
	ShamirDecomposit(k, a, b, X, Y, P);
	for (int i = 0; i < REPEAT; i++) {
		startTimer(&timer);
		
		(*func) (X, a, Y, b, P, Z);
		stopTimer(&timer);
		dur = getTickCount(&timer);
		min = (min < dur) ? min : dur;
	}
	t += min;
	mirkill(a); mirkill(b); mirkill(Y);
}

void duration2(big X, big k, big P, big &Z,
	void(*func) (big, big, big, big, big, big), double &t)
{
	LONGLONG dur, min = LONG_MAX;
	big a = mirvar(1),
		b = mirvar(1),
		Y = mirvar(1);
	stopWatch timer;
	ShamirDecomposit(k, a, b, X, Y, P);
	for (int i = 0; i < REPEAT; i++) {
		startTimer(&timer);
		
		(*func) (X, a, Y, b, P, Z);
		stopTimer(&timer);

		dur = getTickCount(&timer);
		min = (min < dur) ? min : dur;
	}
	t += min;
	mirkill(a); mirkill(b); mirkill(Y);
}

void duration3(big g, big k, big P, big &R)
{

}