#include "ofFpsCounter.h"

#define NANOS_PER_SEC 1000000000
#define NANOS_TO_SEC .000000001

void ofGetMonotonicTime(unsigned long long & seconds, unsigned long long & nanoseconds);

ofFpsCounter::ofFpsCounter()
:nFrameCount(0)
,secsThen(0)
,nanosThen(0)
,fps(0)
,nFramesForFPS(0)
,secsOneSec(0)
,nanosOneSec(0)
,lastFrameTime(0){
	ofGetMonotonicTime(secsThen,nanosThen);
	secsOneSec = secsThen;
	nanosOneSec = nanosThen;
}



ofFpsCounter::ofFpsCounter(double targetFPS)
:nFrameCount(0)
,secsThen(0)
,nanosThen(0)
,fps(targetFPS)
,nFramesForFPS(0)
,secsOneSec(0)
,nanosOneSec(0)
,lastFrameTime(0){
	ofGetMonotonicTime(secsThen,nanosThen);
	secsOneSec = secsThen;
	nanosOneSec = nanosThen;
}

void ofFpsCounter::newFrame(){
	unsigned long long secsNow, nanosNow;
	ofGetMonotonicTime(secsNow,nanosNow);
	unsigned long long oneSecDiff = (secsNow-secsOneSec)*NANOS_PER_SEC + (long long)nanosNow-nanosOneSec;

	if( oneSecDiff  >= NANOS_PER_SEC ){
		fps = nFramesForFPS/(oneSecDiff*NANOS_TO_SEC);
		secsOneSec  = secsNow;
		nanosOneSec  = nanosNow;
		nFramesForFPS = 0;
	}else{
		double deltaTime = ((double)oneSecDiff)*NANOS_TO_SEC;
		if( deltaTime > 0.0 ){
			fps = fps*0.99 + (nFramesForFPS/deltaTime)*0.01;
		}
	}

	lastFrameTime = (secsNow-secsThen)*NANOS_PER_SEC + (long long)nanosNow-nanosThen;
	secsThen = secsNow;
	nanosThen = nanosNow;

	nFramesForFPS++;
	nFrameCount++;
}

double ofFpsCounter::getFps() const{
	return fps;
}

unsigned int ofFpsCounter::getNumFrames() const{
	return nFrameCount;
}

unsigned long long ofFpsCounter::getLastFrameNanos() const{
	return lastFrameTime;
}

double ofFpsCounter::getLastFrameSecs() const{
	return lastFrameTime*NANOS_TO_SEC;
}
