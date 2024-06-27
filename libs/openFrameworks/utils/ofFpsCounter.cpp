#include "ofFpsCounter.h"
using namespace std::chrono;

ofFpsCounter::ofFpsCounter() {}

ofFpsCounter::ofFpsCounter(double targetFPS) : fps(targetFPS) {}

void ofFpsCounter::newFrame(){
	now = steady_clock::now();
	update(now);
	timestamps.push(now);
	lastFrameTime = now - then;

	// std::lerp from c++20 on
	filteredTime = filteredTime * filterAlpha + getLastFrameSecs() * (1-filterAlpha);
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
	return duration_cast<nanoseconds>(lastFrameTime).count();
}

double ofFpsCounter::getLastFrameSecs() const{
	return duration_cast<seconds>(lastFrameTime).count();
}

uint64_t ofFpsCounter::getLastFrameFilteredNanos() const{
	return duration_cast<nanoseconds>(lastFrameTime).count();
}

double ofFpsCounter::getLastFrameFilteredSecs() const{
	return filteredTime;
}

void ofFpsCounter::setFilterAlpha(float alpha){
	filterAlpha = alpha;
}
