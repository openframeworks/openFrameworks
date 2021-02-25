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
#if defined(TARGET_WIN32)
	GetSystemTimeAsFileTime((LPFILETIME)&nextWakeTime);
#else
	nextWakeTime = ofGetCurrentTime();
#endif
	calculateNextPeriod();
}

void ofTimer::setPeriodicEvent(uint64_t nanoseconds){
	nanosPerPeriod = std::chrono::nanoseconds(nanoseconds);
	reset();
}

void ofTimer::waitNext(){
#if (defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI_LEGACY))
	timespec remainder = {0,0};
	timespec wakeTime = nextWakeTime.getAsTimespec();
	clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&wakeTime,&remainder);
#elif defined(TARGET_WIN32)
	WaitForSingleObject(hTimer, INFINITE);
#else
	auto now = ofGetCurrentTime();
	if(now < nextWakeTime){
		auto waitNanos = nextWakeTime - now;
		timespec waittime = (ofTime() + waitNanos).getAsTimespec();
		timespec remainder = {0,0};
		nanosleep(&waittime, &remainder);
	}
#endif
	calculateNextPeriod();
}


void ofTimer::calculateNextPeriod(){
#if defined(TARGET_WIN32)
	nextWakeTime.QuadPart += nanosPerPeriod.count()/100;
    LARGE_INTEGER now;
    GetSystemTimeAsFileTime((LPFILETIME)&now);
	if(nextWakeTime.QuadPart<now.QuadPart){
	    reset();
	}else{
	    SetWaitableTimer(hTimer, &nextWakeTime, 0, nullptr, nullptr, 0);
	}
#else
	nextWakeTime += nanosPerPeriod;
	auto now = ofGetCurrentTime();
    if(nextWakeTime<now){
        reset();
    }
#endif
}
