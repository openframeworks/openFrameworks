#include "ofTimer.h"

void ofGetMonotonicTime(unsigned long long & seconds, unsigned long long & nanoseconds);

ofTimer::ofTimer()
:nanosPerPeriod(0)
#ifdef TARGET_WINDOWS
,hTimer(CreateWaitableTimer(NULL, TRUE, NULL))
#endif
{

}

void ofTimer::reset(){
#if (defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI))
	clock_gettime(CLOCK_MONOTONIC,&nextWakeTime);
#elif defined(TARGET_WIN32)
	GetSystemTimeAsFileTime((LPFILETIME)&nextWakeTime);
#else
	ofGetMonotonicTime(prevSecs,prevNanos);
#endif
	calculateNextPeriod();
}

void ofTimer::setPeriodicEvent(unsigned long long nanoseconds){
	nanosPerPeriod = nanoseconds;
	reset();
}

void ofTimer::waitNext(){
#if (defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI))
	timespec remainder = {0,0};
	clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&nextWakeTime,&remainder);
#elif defined(TARGET_WIN32)
	WaitForSingleObject(hTimer, INFINITE);
#else
	unsigned long long secsNow, nanosNow;
	ofGetMonotonicTime(secsNow, nanosNow);
	unsigned long long diffNanos = (secsNow - prevSecs)*1000000000 + ((long long)(nanosNow - prevNanos));
	prevSecs = secsNow;
	prevNanos = nanosNow;
	if(diffNanos < nanosPerPeriod){
		unsigned long long waitNanos = nanosPerPeriod/1000 - diffNanos;
		nanosleep(waitNanos);
		// Theoretical value to compensate for the extra time that it might sleep
		prevNanos += waitNanos;
		if(prevNanos>1000000000){
			prevNanos-=1000000000;
			prevSecs+=1;
		}
#endif
	calculateNextPeriod();
}

void ofTimer::calculateNextPeriod(){
#if (defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI))
	nextWakeTime.tv_nsec += nanosPerPeriod;
	if(nextWakeTime.tv_nsec>1000000000){
		nextWakeTime.tv_nsec-=1000000000;
		nextWakeTime.tv_sec+=1;
	}
#elif defined(TARGET_WIN32)
	nextWakeTime.QuadPart += nanosPerPeriod/100;
	SetWaitableTimer(hTimer, &nextWakeTime, 0, NULL, NULL, 0);
#endif
}
