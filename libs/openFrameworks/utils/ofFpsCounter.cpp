#include "ofFpsCounter.h"

ofFpsCounter::ofFpsCounter()
:nFrameCount(0)
,then(steady_clock::now())
,fps(0)
,lastFrameTime(0)
,filteredTime(0)
,filterAlpha(0.9){}



ofFpsCounter::ofFpsCounter(double targetFPS)
:nFrameCount(0)
,then(steady_clock::now())
,fps(targetFPS)
,lastFrameTime(0)
,filteredTime(0)
,filterAlpha(0.9){}

void ofFpsCounter::newFrame(){
	now = steady_clock::now();

//	auto now = ofGetCurrentTime();
	update(now);
	timestamps.push(now);

	lastFrameTime = now - then;
//	auto filtered =
//	filteredTime * filterAlpha + lastFrameTime * (1-filterAlpha);
//	filteredTime = filteredTime * filterAlpha + lastFrameTime * (1-filterAlpha);
	then = now;
	nFrameCount++;
}

void ofFpsCounter::update(){
	now = steady_clock::now();
	update(now);
}

void ofFpsCounter::update(time_point<steady_clock> now){
	while(!timestamps.empty() && timestamps.front() + 2s < now){
		timestamps.pop();
	}

	space diff;
	if(!timestamps.empty() && timestamps.front() + 0.5s < now){
		diff = now - timestamps.front();
	}
	if(diff > 0.0s){
		fps = (double)timestamps.size() / std::chrono::duration<double>(diff).count();
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
	return lastFrameTime.count();
}

double ofFpsCounter::getLastFrameSecs() const{
	return std::chrono::duration<double>(lastFrameTime).count();
}

uint64_t ofFpsCounter::getLastFrameFilteredNanos() const{
	return lastFrameTime.count();
}

double ofFpsCounter::getLastFrameFilteredSecs() const{
	return std::chrono::duration<double>(filteredTime).count();
}

void ofFpsCounter::setFilterAlpha(float alpha){
	filterAlpha = alpha;
}
