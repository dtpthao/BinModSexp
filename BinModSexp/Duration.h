#ifndef _COUNT_TIME_H
#define _COUNT_TIME_H

#include <Windows.h>
#include <time.h>
#include "Field.h"
#include "shr_decomp.h"

typedef epoint* pepoint;

struct Result {
	double t[7] = { 0 };
	double p[7] = { 0 };
	unsigned int c[7] = { 0 };
};

typedef struct {
	LARGE_INTEGER start;
	LARGE_INTEGER stop;
} stopWatch;

inline void startTimer(stopWatch *timer);
inline void stopTimer(stopWatch *timer);
inline double LIToSecs(LARGE_INTEGER *L);
inline double getElapsedTime(stopWatch *timer);	//get time in secs/msecs
inline LONGLONG getTickCount(stopWatch *timer);	//get time in tick

#endif