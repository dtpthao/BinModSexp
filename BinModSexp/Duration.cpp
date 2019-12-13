#include "Duration.h"

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