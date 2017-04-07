#include "ofFpsCounter.h"

ofFpsCounter::ofFpsCounter()
:nFrameCount(0)
,then(ofGetCurrentTime())
,fps(0)
,lastFrameTime(0)
,filteredTime(0)
,filterAlpha(0.9){}



ofFpsCounter::ofFpsCounter(double targetFPS)
:nFrameCount(0)
,then(ofGetCurrentTime())
,fps(targetFPS)
,lastFrameTime(0)
,filteredTime(0)
,filterAlpha(0.9){}

void ofFpsCounter::newFrame(){
	auto now = ofGetCurrentTime();
	update(now.getAsSeconds());
	timestamps.push(now.getAsSeconds());

	lastFrameTime = now - then;
	uint64_t filtered = filteredTime.count() * filterAlpha + lastFrameTime.count() * (1-filterAlpha);
	filteredTime = std::chrono::nanoseconds(filtered);
	then = now;
	nFrameCount++;
}

void ofFpsCounter::update(){
	auto now = ofGetCurrentTime();
	update(now.getAsSeconds());
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
