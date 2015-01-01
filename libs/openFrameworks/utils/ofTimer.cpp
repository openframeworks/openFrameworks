#include "ofTimer.h"

#define NANOS_PER_SEC 1000000000ll

void ofGetMonotonicTime(unsigned long long & seconds, unsigned long long & nanoseconds);

ofTimer::ofTimer()
:nanosPerPeriod(0)
#ifdef TARGET_WIN32
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
	ofGetMonotonicTime(nextWakeTimeSecs,nextWakeTimeNanos);
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
	long long waitNanos = ((long long)(nextWakeTimeSecs - secsNow))*1000000000 + ((long long)(nextWakeTimeNanos - nanosNow));
	if(waitNanos > 0){
		timespec waittime;
		timespec remainder;
		waittime.tv_sec = waitNanos / NANOS_PER_SEC;
		waittime.tv_nsec = waitNanos % NANOS_PER_SEC;
		nanosleep(&waittime,&remainder);
	}
#endif
	calculateNextPeriod();
}

void ofTimer::calculateNextPeriod(){
#if (defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI))
	nextWakeTime.tv_nsec += nanosPerPeriod;
	if(nextWakeTime.tv_nsec>NANOS_PER_SEC){
		uint64_t secs = nextWakeTime.tv_nsec / NANOS_PER_SEC;
		nextWakeTime.tv_nsec-=NANOS_PER_SEC*secs;
		nextWakeTime.tv_sec+=secs;
	}
#elif defined(TARGET_WIN32)
	nextWakeTime.QuadPart += nanosPerPeriod/100;
	SetWaitableTimer(hTimer, &nextWakeTime, 0, NULL, NULL, 0);
#else
	nextWakeTimeNanos += nanosPerPeriod;
	if(nextWakeTimeNanos>NANOS_PER_SEC){
		uint64_t secs = nextWakeTimeNanos / NANOS_PER_SEC;
		nextWakeTimeNanos-=NANOS_PER_SEC*secs;
		nextWakeTimeSecs+=secs;
	}
#endif
}
