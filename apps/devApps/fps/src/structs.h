#pragma once

#include <chrono>
using namespace std::chrono;
using namespace std::chrono_literals;

class ofTimerFpsX {
public:
	ofTimerFpsX(){
		reset();
	};

	using space = std::chrono::duration<long long, std::nano>;
	space interval;
	time_point<steady_clock> wakeTime;
	time_point<steady_clock> lastWakeTime;

	void setFps(int fps) {
		interval = duration_cast<microseconds>(1s) / fps;
	}

	void reset() {
		wakeTime = steady_clock::now();
	}
	
	void waitNext() {
		// Lazy wakeup
		std::this_thread::sleep_until(wakeTime - 4ms);
		
		// Processor Coffee
		while(steady_clock::now() < (wakeTime)) { // 0.05ms 0.5us // - 0.5us  - 1ns
			std::this_thread::yield();
		}
	
		lastWakeTime = wakeTime;
		wakeTime += interval;
	}
};






struct grapher {
	string name {""};
	ofColor color = ofColor(255, 0, 0);
	int nVals = 300;
	int cursor = 0;
	std::vector<float> vals;
	ofRectangle rect;
	bool isSetup = false;
	int divider = 10;
	
	void setRect(ofRectangle r) {
		vals.reserve(nVals);
		rect = r;
		nVals = rect.width/divider;
		isSetup = true;
	}
	
	void add(float v) {
		if (std::isnan(v)) {
			return;
		}
//		cout << name << " : " << v << " : " << vals.size() << endl;
		if (isSetup) {
			if (vals.size() < nVals) {
				vals.emplace_back(v);
			} else {
				vals[cursor] = v;
				cursor = (cursor+1)%(nVals);
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
			
			float min = *std::min_element (vals.begin(), vals.end()); //          ^
			float max = *std::max_element (vals.begin(), vals.end()); //
			float minmax = (std::abs(min) + std::abs(max))/2.0f;
//			float avg = 1.0f * std::accumulate( vals.begin(), vals.end(), 0 )/vals.size();
			float avg = 1.0f * std::reduce( vals.begin(), vals.end(), 0 ) / (float)vals.size();

			int x = 0;
			for (auto & v : vals) {
				float y = ofMap(v, -minmax, minmax, 0, rect.height);
				ofDrawLine(rect.x + x, rect.y + rect.height*.5, rect.x + x, rect.y + y );
				x += divider;
			}
			
			float y = ofMap(vals[cursor], -minmax, minmax, 0, rect.height);

			ofSetColor(255);
			ofDrawLine(rect.x, rect.y + y, rect.x+rect.width, rect.y + y);
			ofNoFill();
			ofDrawRectangle(rect);

			string s {
				name + "\n" +
				"min:"+ofToString(min) + "\n" +
				"max:"+ofToString(max) + "\n" +
				"avg:"+ofToString(avg) + "\n" +
				""
			};
			ofDrawBitmapString(s, rect.x + 30, rect.y + 50);
		}
	}
};



struct fpsCounter {
public:
	int nAverages = 60;
	using space = std::chrono::duration<long double, std::nano>;
	time_point<steady_clock> lastTick;
	steady_clock::duration onesec = 1s;
	vector <space> intervals;
	space interval;
	space average;
	bool firstTick = true;
	int cursor = 0;
	vector<float> results;
	int cur = 0;
	
	float filterAverages = 32.0f;
	
	fpsCounter() {
		for (int a=0; a<filterAverages; a++) {
			results.emplace_back(0.0f);
		}
	}
	
	void reset() {

		intervals.clear();
		cursor = 0;
	}

	void tick() {
		if (firstTick) {
			firstTick = false;
			lastTick = steady_clock::now();
			return;
		}

		interval = steady_clock::now() - lastTick;
		if (intervals.size() < nAverages) {
			intervals.emplace_back(interval);
		} else {
			intervals[cursor] = interval;
			cursor = (cursor+1)%nAverages;
		}
		lastTick = steady_clock::now();
	}
	
	float lastVal = 1;
	float valRatio = 1.0;
	
	float get() {
		if (intervals.size()) {
			average = std::reduce(intervals.begin(), intervals.end());
			//		cout << ofGetFrameNum() << " : " << intervals.size() << endl;
			float val = intervals.size() * onesec / average;
			results[cur] = val;
			cur = (cur+1)% (int)filterAverages;
			return std::reduce(results.begin(), results.end())/filterAverages;
//			lastVal *= (1.0f - valRatio);
//			lastVal += val * valRatio;
//			return lastVal;
		}
		return 1;
		//		float res = val * . + lastVal * .5;
//		lastVal = val;
//		return val;
//		return intervals.size() * onesec / average;
	}
};



#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

using std::cout;
using std::endl;



struct fpsToggler {
public:
	bool isNew = true;
	ofTimerFpsX t;
	int fps = 30;
	
	void updateFps() {
		if (isNew) {
			ofSetVerticalSync(0);
			ofSetFrameRate(0);
			t.setFps(fps);
			t.reset();
		} else {
			ofSetVerticalSync(0);
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






