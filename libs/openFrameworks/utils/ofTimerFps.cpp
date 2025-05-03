#include "ofTimerFps.h"

using namespace std::chrono;
using namespace std::chrono_literals;

void ofTimerFps::reset() {
   wakeTime = steady_clock::now();
}

ofTimerFps::ofTimerFps() {
	reset();
	interval = duration_cast<microseconds>(1s) / currentFPS;
};

void ofTimerFps::setFps(int fps) {
//	interval = std::ratio<1s, fps>;
	if (fps <= 0) {
		fps = 60; // fallback
	}
	currentFPS = fps;

	interval = duration_cast<microseconds>(1s) / currentFPS;
}

void ofTimerFps::waitNext() {
   // Lazy wakeup
   std::this_thread::sleep_until(wakeTime - 8ms); //4ms

   // Processor Coffee
   while(steady_clock::now() < (wakeTime)) { // 0.05ms 0.5us // - 0.5us  - 1ns
//	   std::this_thread::yield();
	   std::this_thread::sleep_for(5us);
   }

   lastWakeTime = wakeTime;
   wakeTime += interval;
}
