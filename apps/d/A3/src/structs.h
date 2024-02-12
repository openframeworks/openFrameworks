

#include <chrono>
using namespace std::chrono;
using namespace std::chrono_literals;

struct fpsCounter {
public:
	int nAverages = 20;
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
} ;



#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

using std::cout;
using std::endl;

class ofTimerFpsX {
public:
	ofTimerFpsX(){
		reset();
	}
	ofTimerFpsX(int f) {
		reset();
		setFps(f);
	}
	using space = std::chrono::duration<long long, std::nano>; //micro
	time_point<steady_clock> wakeTime;
	time_point<steady_clock> lastWakeTime;
	space interval;
	space drift;

	void setFps(int fps) {
		interval = duration_cast<microseconds>(1s) / fps;
		wakeTime = steady_clock::now() + interval;
		lastWakeTime = steady_clock::now();
	}

	void reset() {
		wakeTime = steady_clock::now();
		lastWakeTime = steady_clock::now();
	}
	
	void waitNext(){
		bool debug = false;

		if (debug) {
			std::cout << "interval:" << duration_cast<microseconds>(wakeTime - lastWakeTime).count() << std::endl;
	
			auto el = duration_cast<microseconds>(steady_clock::now() - lastWakeTime).count();
			std::cout << "ellapsed:" << el  << std::endl;
	
			auto ws = duration_cast<microseconds>(wakeTime - steady_clock::now()).count();
			std::cout << "will sleep:" << ws << std::endl;
			cout << "soma" << (el + ws) << endl;
		}
		if (wakeTime > steady_clock::now()) {
			std::this_thread::sleep_until(wakeTime);
		}
		drift = steady_clock::now() - wakeTime;
		auto driftNs = duration_cast<microseconds>(drift).count();
		

		if (debug) {
			std::cout << "drift: " << driftNs << std::endl;
			std::cout << "----" << std::endl;
		}
		
		lastWakeTime = wakeTime;
		wakeTime += interval;
		if (drift < interval) {
			wakeTime -= (drift/1000);
		}
	}
};


