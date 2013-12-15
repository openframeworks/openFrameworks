#include "ofMath.h"
#include "ofUtils.h"
#include "ofAppRunner.h"
#include "float.h"

#ifndef TARGET_WIN32
	#include <sys/time.h>
#endif

#include "ofNoise.h"
#include "ofPolyline.h"

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
float ofRandom(float max) {
	return max * rand() / (RAND_MAX + 1.0f);
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
float ofMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) {

	if (fabs(inputMin - inputMax) < FLT_EPSILON){
		ofLogWarning("ofMath") << "ofMap(): avoiding possible divide by zero, check inputMin and inputMax: " << inputMin << " " << inputMax;
		return outputMin;
	} else {
		float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
	
		if( clamp ){
			if(outputMax < outputMin){
				if( outVal < outputMax )outVal = outputMax;
				else if( outVal > outputMin )outVal = outputMin;
			}else{
				if( outVal > outputMax )outVal = outputMax;
				else if( outVal < outputMin )outVal = outputMin;
			}
		}
		return outVal;
	}

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

float ofWrap(float value, float from, float to){
	// algorithm from http://stackoverflow.com/a/5852628/599884
	if(from > to){
		swap(from, to);
	}
	float cycle = to - from;
	if(cycle == 0){
		return to;
	}
	return value - cycle * floor((value - from) / cycle);
}

//--------------------------------------------------
float ofWrapRadians(float angle, float from, float to){
	return ofWrap(angle, from, to);
}

float ofWrapDegrees(float angle, float from, float to){
	return ofWrap(angle, from, to);
}

//--------------------------------------------------
float ofLerpDegrees(float currentAngle, float targetAngle, float pct) {
    return currentAngle + ofAngleDifferenceDegrees(currentAngle,targetAngle) * pct;
}

//--------------------------------------------------
float ofLerpRadians(float currentAngle, float targetAngle, float pct) {
	return currentAngle + ofAngleDifferenceRadians(currentAngle,targetAngle) * pct;
}

//--------------------------------------------------
float ofRandomWidth() {
	return ofRandom(0, ofGetWidth());
}

//--------------------------------------------------
float ofRandomHeight() {
	return ofRandom(0, ofGetHeight());
}

//--------------------------------------------------
float ofNoise(float x){
	return _slang_library_noise1(x)*0.5f + 0.5f;
}

//--------------------------------------------------
float ofNoise(float x, float y){
	return _slang_library_noise2(x,y)*0.5f + 0.5f;
}

//--------------------------------------------------
float ofNoise(float x, float y, float z){
	return _slang_library_noise3(x,y,z)*0.5f + 0.5f;
}

//--------------------------------------------------
float ofNoise(float x, float y, float z, float w){
	return _slang_library_noise4(x,y,z,w)*0.5f + 0.5f;
}

//--------------------------------------------------
float ofSignedNoise(float x){
	return _slang_library_noise1(x);
}

//--------------------------------------------------
float ofSignedNoise(float x, float y){
	return _slang_library_noise2(x,y);
}

//--------------------------------------------------
float ofSignedNoise(float x, float y, float z){
	return _slang_library_noise3(x,y,z);
}

//--------------------------------------------------
float ofSignedNoise(float x, float y, float z, float w){
	return _slang_library_noise4(x,y,z,w);
}

//--------------------------------------------------
bool ofInsidePoly(float x, float y, const vector<ofPoint> & polygon){
    return ofPolyline::inside(x,y, ofPolyline(polygon));
}

//--------------------------------------------------
bool ofInsidePoly(const ofPoint & p, const vector<ofPoint> & poly){
    return ofPolyline::inside(p.x,p.y, ofPolyline(poly));
}

//--------------------------------------------------
bool ofLineSegmentIntersection(ofPoint line1Start, ofPoint line1End, ofPoint line2Start, ofPoint line2End, ofPoint & intersection){
	ofPoint diffLA, diffLB;
	float compareA, compareB;
	diffLA = line1End - line1Start;
	diffLB = line2End - line2Start;
	compareA = diffLA.x*line1Start.y - diffLA.y*line1Start.x;
	compareB = diffLB.x*line2Start.y - diffLB.y*line2Start.x;
	if (
		(
			( ( diffLA.x*line2Start.y - diffLA.y*line2Start.x ) < compareA ) ^
			( ( diffLA.x*line2End.y - diffLA.y*line2End.x ) < compareA )
		)
		&&
		(
			( ( diffLB.x*line1Start.y - diffLB.y*line1Start.x ) < compareB ) ^
			( ( diffLB.x*line1End.y - diffLB.y*line1End.x) < compareB )
		)
	)
	{
		float lDetDivInv = 1 / ((diffLA.x*diffLB.y) - (diffLA.y*diffLB.x));
		intersection.x =  -((diffLA.x*compareB) - (compareA*diffLB.x)) * lDetDivInv ;
		intersection.y =  -((diffLA.y*compareB) - (compareA*diffLB.y)) * lDetDivInv ;

		return true;
	}

	return false;
}

//--------------------------------------------------
ofPoint ofBezierPoint( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t){
    float tp = 1.0 - t;
    return a*tp*tp*tp + b*3*t*tp*tp + c*3*t*t*tp + d*t*t*t;
}

//--------------------------------------------------
ofPoint ofCurvePoint( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t){
    ofPoint pt;
    float t2 = t * t;
    float t3 = t2 * t;
    pt.x = 0.5f * ( ( 2.0f * b.x ) +
                   ( -a.x + c.x ) * t +
                   ( 2.0f * a.x - 5.0f * b.x + 4 * c.x - d.x ) * t2 +
                   ( -a.x + 3.0f * b.x - 3.0f * c.x + d.x ) * t3 );
    pt.y = 0.5f * ( ( 2.0f * b.y ) +
                   ( -a.y + c.y ) * t +
                   ( 2.0f * a.y - 5.0f * b.y + 4 * c.y - d.y ) * t2 +
                   ( -a.y + 3.0f * b.y - 3.0f * c.y + d.y ) * t3 );
    return pt;
}

//--------------------------------------------------
ofPoint ofBezierTangent( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t){
    return (d-a-c*3+b*3)*(t*t)*3 + (a+c-b*2)*t*6 - a*3+b*3;
}

//--------------------------------------------------
ofPoint ofCurveTangent( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t){
    ofPoint v0 = ( c - a )*0.5;
    ofPoint v1 = ( d - b )*0.5;
    return ( b*2 -c*2 + v0 + v1)*(3*t*t) + ( c*3 - b*3 - v1 - v0*2 )*( 2*t) + v0;

}

//--------------------------------------------------
float ofAngleDifferenceDegrees(float currentAngle, float targetAngle) {
	return ofWrapDegrees(targetAngle - currentAngle);
}

//--------------------------------------------------
float ofAngleDifferenceRadians(float currentAngle, float targetAngle) {
	return  ofWrapRadians(targetAngle - currentAngle);
}


