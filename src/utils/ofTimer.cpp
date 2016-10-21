#include "ofTimer.h"

#define NANOS_PER_SEC 1000000000ll

void ofGetMonotonicTime(uint64_t & seconds, uint64_t & nanoseconds);

ofTimer::ofTimer()
:nanosPerPeriod(0)
#ifdef TARGET_WIN32
,hTimer(CreateWaitableTimer(nullptr, TRUE, nullptr))
#endif
{

}

void ofTimer::reset(){
#if (defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI))
	clock_gettime(CLOCK_MONOTONIC,&nextWakeTime);
#elif defined(TARGET_WIN32)
	GetSystemTimeAsFileTime((LPFILETIME)&nextWakeTime);
#else
	ofGetMonotonicTime(nextWakeTime.tv_sec,nextWakeTime.tv_nsec);
#endif
	calculateNextPeriod();
}

void ofTimer::setPeriodicEvent(uint64_t nanoseconds){
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
	sec_ns now;
	ofGetMonotonicTime(now.tv_sec, now.tv_nsec);
	int64_t waitNanos = ((int64_t)(nextWakeTime.tv_sec - now.tv_sec))*1000000000 + ((int64_t)(nextWakeTime.tv_nsec - now.tv_nsec));
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

#if (defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI))
static bool operator<(const timespec & time1, const timespec & time2){
    return time1.tv_sec < time2.tv_sec || (time1.tv_sec == time2.tv_sec && time1.tv_nsec < time2.tv_nsec);
}
#endif


void ofTimer::calculateNextPeriod(){
#if defined(TARGET_WIN32)
	nextWakeTime.QuadPart += nanosPerPeriod/100;
    LARGE_INTEGER now;
    GetSystemTimeAsFileTime((LPFILETIME)&now);
	if(nextWakeTime.QuadPart<now.QuadPart){
	    reset();
	}else{
	    SetWaitableTimer(hTimer, &nextWakeTime, 0, nullptr, nullptr, 0);
	}
#else
    nextWakeTime.tv_nsec += nanosPerPeriod;
    if(nextWakeTime.tv_nsec>NANOS_PER_SEC){
        uint64_t secs = nextWakeTime.tv_nsec / NANOS_PER_SEC;
        nextWakeTime.tv_nsec-=NANOS_PER_SEC*secs;
        nextWakeTime.tv_sec+=secs;
    }
#if (defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI))
    timespec now;
    clock_gettime(CLOCK_MONOTONIC,&now);
#else
    sec_ns now;
    ofGetMonotonicTime(now.tv_sec,now.tv_nsec);
#endif
    if(nextWakeTime<now){
        reset();
    }
#endif
}
