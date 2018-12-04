#ifndef _COUNT_TIME_H
#define _COUNT_TIME_H

#include <Windows.h>
#include <time.h>
#include "Field.h"
#include "ShamirMul.h"

typedef epoint* pepoint;

typedef struct {
	LARGE_INTEGER start;
	LARGE_INTEGER stop;
} stopWatch;

inline void startTimer(stopWatch *timer);
inline void stopTimer(stopWatch *timer);
inline double LIToSecs(LARGE_INTEGER *L);
inline double getElapsedTime(stopWatch *timer);	//get time in secs/msecs
inline LONGLONG getTickCount(stopWatch *timer);	//get time in tick

void duration1(big X, big k, big P, big &Z,
	void(*func) (big, big, big, big&), double &t);

void duration1(big X, big k, big P, big &Z,
	void(*func) (big, big, big, big), double &t);

void durationJSF(big X, big k, big P, big &Z, double &t);

void duration2(big X, big k, big P, big &Z,
	void(*func) (big, big, big, big, big, big&), double &t);

void duration2(big X, big k, big P, big &Z,
	void(*func) (big, big, big, big, big, big), double &t);

#endif
