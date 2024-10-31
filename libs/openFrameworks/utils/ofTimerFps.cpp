#include "ofTimerFps.h"

using namespace std::chrono;
using namespace std::chrono_literals;

void ofTimerFps::reset() {
   wakeTime = steady_clock::now();
}

ofTimerFps::ofTimerFps(){
	reset();
};

void ofTimerFps::setFps(int fps) {
//	interval = std::ratio<1s, fps>;
   interval = duration_cast<microseconds>(1s) / fps;
}

void ofTimerFps::waitNext() {
   // Lazy wakeup
   std::this_thread::sleep_until(wakeTime - 36ms); //4ms

   // Processor Coffee
   while(steady_clock::now() < (wakeTime)) { // 0.05ms 0.5us // - 0.5us  - 1ns
	   std::this_thread::yield();
   }

   lastWakeTime = wakeTime;
   wakeTime += interval;
}
