#include "ofMath.h"
#include <sys/time.h>



//--------------------------------------------------
int ofNextPow2(int a){
	// from nehe.gamedev.net lesson 43
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}

//--------------------------------------------------
void ofSeedRandom() {
	struct timeval tv;
	gettimeofday(&tv, 0);

	// use XOR'd second, microsecond precision AND pid as seed
	long int n = (tv.tv_sec ^ tv.tv_usec) ^ getpid();
	srand(n);

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
//--------------------------------------------------
