#pragma once

#include <chrono>
using namespace std::chrono;
using namespace std::chrono_literals;


struct grapher {
	string name {""};
	ofColor color = ofColor(255, 0, 0);
	int nVals = 300;
	int cursor = 0;
	std::vector<float> vals;
	ofRectangle rect;
	bool isSetup = false;
	int divider = 3;
	
	void setRect(ofRectangle r) {
		rect = r;
		nVals = rect.width/divider;
		vals.reserve(nVals);
		isSetup = true;
	}
	
	void add(float v) {
		if (isSetup) {
			if (vals.size() < nVals) {
				vals.emplace_back(v);
			} else {
				vals[cursor] = v;
				cursor = (cursor+1)%(nVals-1);
			}
		}
	}
	
	void reset() {
		cursor = 0;
		vals.clear();
	}
	
	void draw() {
		if (isSetup) {
			ofSetColor(color);
			ofNoFill();
			ofDrawRectangle(rect);
			
			float min = *std::min_element (vals.begin(), vals.end()); //          ^
			float max = *std::max_element (vals.begin(), vals.end()); //
			float minmax = (std::abs(min) + std::abs(max))/2.0f;
			
			int x = 0;
			for (auto & v : vals) {
				float y = ofMap(v, -minmax, minmax, 0, rect.height);
				ofDrawLine(rect.x + x, rect.y + rect.height*.5, rect.x + x, rect.y + y );
				x += divider;
			}
			
			float y = ofMap(vals[cursor], -minmax, minmax, 0, rect.height);

			ofSetColor(255);
			ofDrawLine(rect.x, rect.y + y, rect.x+rect.width, rect.y + y);

			string s {
				name + "\n" +
				"min:"+ofToString(min) + "\n" +
				"max:"+ofToString(max)
			};
			ofSetColor(255);
			ofDrawBitmapString(s, rect.x + 30, rect.y + 50);
		}
	}
};

struct fpsCounter {
public:
	int nAverages = 40;
	using space = std::chrono::duration<long double, std::nano>;
	time_point<steady_clock> lastTick;
	steady_clock::duration onesec = 1s;
	vector <space> intervals;
	space interval;
	space average;
	bool firstTick = true;
	int cursor = 0;

	void tick() {
		if (firstTick) {
			firstTick = false;
			lastTick = steady_clock::now();
			return;
		}

		interval = steady_clock::now() - lastTick;
		lastTick = steady_clock::now();
		if (intervals.size() < nAverages) {
			intervals.emplace_back(interval);
		} else {
			intervals[cursor] = interval;
			cursor = (cursor+1)%nAverages;
		}
	}
	
	float get() {
//		average = std::reduce(intervals.begin(), intervals.end())/intervals.size();
//		return onesec / average;
		average = std::reduce(intervals.begin(), intervals.end());
		return intervals.size() * onesec / average;
	}
};



#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

using std::cout;
using std::endl;

//
//class ofTimerFps {
//public:
//	ofTimerFps(){
//		reset();
//	};
//
//	using space = std::chrono::duration<long long, std::nano>;
//	space interval;
//	time_point<steady_clock> wakeTime;
//	time_point<steady_clock> lastWakeTime;
//
//	void setFps(int fps) {
//		interval = duration_cast<microseconds>(1s) / fps;
//	}
//
//	void reset() {
//		wakeTime = steady_clock::now();
//	}
//	
//	void waitNext(){
//		std::this_thread::sleep_until(wakeTime);
//		lastWakeTime = wakeTime;
//		wakeTime += interval;
//	}
//};

struct fpsToggler {
public:
	bool isNew = false;
	ofTimerFps t;
	int fps = 30;
	
	void updateFps() {
		if (isNew) {
			ofSetVerticalSync(0);
			ofSetFrameRate(0);
			t.setFps(fps);
			t.reset();
		} else {
			ofSetFrameRate(fps);
		}
	}
	void setup() {
		updateFps();
	}

	void setFps(int f) {
		fps = f;
		updateFps();
	}

	void toggle() {
		isNew ^= 1;
		updateFps();
	}
	
	void update() {
		if (isNew) {
			t.waitNext();
		}
	}
} ;




