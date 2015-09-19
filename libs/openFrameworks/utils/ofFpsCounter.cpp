#include "ofFpsCounter.h"

#define NANOS_PER_SEC 1000000000
#define NANOS_TO_SEC .000000001

void ofGetMonotonicTime(uint64_t & seconds, uint64_t & nanoseconds);

ofFpsCounter::ofFpsCounter()
:nFrameCount(0)
,secsThen(0)
,nanosThen(0)
,fps(0)
,lastFrameTime(0){
	ofGetMonotonicTime(secsThen,nanosThen);
}



ofFpsCounter::ofFpsCounter(double targetFPS)
:nFrameCount(0)
,secsThen(0)
,nanosThen(0)
,fps(targetFPS)
,lastFrameTime(0){
	ofGetMonotonicTime(secsThen,nanosThen);
}

void ofFpsCounter::newFrame(){
	uint64_t secsNow, nanosNow;
	ofGetMonotonicTime(secsNow,nanosNow);
	auto now = secsNow + nanosNow/double(NANOS_PER_SEC);
	update(now);
	timestamps.push(now);

	lastFrameTime = (secsNow-secsThen)*NANOS_PER_SEC + (long long)nanosNow-nanosThen;
	secsThen = secsNow;
	nanosThen = nanosNow;
	nFrameCount++;
}

void ofFpsCounter::update(){
	uint64_t secsNow, nanosNow;
	ofGetMonotonicTime(secsNow,nanosNow);
	auto now = secsNow + nanosNow/double(NANOS_PER_SEC);
	update(now);
}

void ofFpsCounter::update(double now){
	while(!timestamps.empty() && timestamps.front() + 2 < now){
		timestamps.pop();
	}

	auto diff = 0.0;
	if(!timestamps.empty() && timestamps.front() + 0.5 < now){
		diff = now - timestamps.front();
	}
	if(diff>0.0){
		fps = timestamps.size() / diff;
	}else{
		fps = timestamps.size();
	}
}

double ofFpsCounter::getFps() const{
	return fps;
}

uint64_t ofFpsCounter::getNumFrames() const{
	return nFrameCount;
}

uint64_t ofFpsCounter::getLastFrameNanos() const{
	return lastFrameTime;
}

double ofFpsCounter::getLastFrameSecs() const{
	return lastFrameTime*NANOS_TO_SEC;
}
