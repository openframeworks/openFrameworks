#include "ofMath.h"
#include "ofAppRunner.h"

#ifndef TARGET_WIN32
	#include <sys/time.h>
#endif



//--------------------------------------------------
int ofNextPow2(int a){
	// from nehe.gamedev.net lesson 43
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}

//--------------------------------------------------
void ofSeedRandom() {

	// good info here:
	// http://stackoverflow.com/questions/322938/recommended-way-to-initialize-srand

	#ifdef TARGET_WIN32
		srand(GetTickCount());
	#else
		// use XOR'd second, microsecond precision AND pid as seed
		struct timeval tv;
		gettimeofday(&tv, 0);
		long int n = (tv.tv_sec ^ tv.tv_usec) ^ getpid();
		srand(n);
	#endif
}

//--------------------------------------------------
void ofSeedRandom(int val) {
	srand((long) val);
}

//--------------------------------------------------
float ofRandom(float x, float y) {

	float high = 0;
	float low = 0;
	float randNum = 0;
	// if there is no range, return the value
	if (x == y) return x; 			// float == ?, wise? epsilon?
	high = MAX(x,y);
	low = MIN(x,y);
	randNum = low + ((high-low) * rand()/(RAND_MAX + 1.0));
	return randNum;
}

//--------------------------------------------------
float ofRandomf() {
	float randNum = 0;
	randNum = (rand()/(RAND_MAX + 1.0)) * 2.0 - 1.0;
	return randNum;
}

//--------------------------------------------------
float ofRandomuf() {
	float randNum = 0;
	randNum = rand()/(RAND_MAX + 1.0);
	return randNum;
}

//---- new to 006
//from the forums http://www.openframeworks.cc/forum/viewtopic.php?t=1413

//--------------------------------------------------
float ofNormalize(float value, float min, float max){
   return ofClamp( (value - min) / (max - min), 0, 1);
}

//check for division by zero???
//--------------------------------------------------
float ofMap(float value, float inputMin, float inputMax, float outputMin, float outputMax) {
   return ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
}

//--------------------------------------------------
float ofDist(float x1, float y1, float x2, float y2) {
   return sqrt(double((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

//--------------------------------------------------
float ofDistSquared(float x1, float y1, float x2, float y2) {
   return ( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) );
}

//--------------------------------------------------
float ofClamp(float value, float min, float max) {
   return value < min ? min : value > max ? max : value;
}

// return sign of the number
//--------------------------------------------------
int ofSign(float n) {
  if( n > 0 ) return 1;
  else if(n < 0) return -1;
  else return 0;
}

//--------------------------------------------------
bool ofInRange(float t, float min, float max) {
   return t>=min && t<=max;
}

//--------------------------------------------------
float ofRadToDeg(float radians) {
   return radians * RAD_TO_DEG;
}

//--------------------------------------------------
float ofDegToRad(float degrees) {
    return degrees * DEG_TO_RAD;
}

//--------------------------------------------------
float ofLerp(float start, float stop, float amt) {
    return start + (stop-start) * amt;
}

//--------------------------------------------------
float ofRandomWidth() {
   return ofRandom(0, ofGetWidth());
}

//--------------------------------------------------
float ofRandomHeight() {
   return ofRandom(0, ofGetHeight());
}
