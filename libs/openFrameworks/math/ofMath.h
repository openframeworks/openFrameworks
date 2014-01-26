
// notes:
// -----------------------------------------------------------
// for fast things look here: http://musicdsp.org/archive.php?classid=5#115
// -----------------------------------------------------------
// the random () calls are based on misconceptions described here:
// http://www.azillionmonkeys.com/qed/random.html
// (Bad advice from C.L.C. FAQ)
// we should correct this -- 
// -----------------------------------------------------------

#pragma once


#include "ofPoint.h"
#include "ofConstants.h"

int 		ofNextPow2 ( int a );
void 		ofSeedRandom();
void 		ofSeedRandom(int val);
float 		ofRandom(float max); // random (0 - max)
float 		ofRandom(float val0, float val1);		// random (x - y)
float 		ofRandomf();							// random (-1 - 1)
float 		ofRandomuf();							// random (0 - 1) 

float		ofNormalize(float value, float min, float max);
float		ofMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false);
float		ofClamp(float value, float min, float max);
float		ofLerp(float start, float stop, float amt);
float		ofDist(float x1, float y1, float x2, float y2);
float		ofDistSquared(float x1, float y1, float x2, float y2);
int			ofSign(float n);
bool		ofInRange(float t, float min, float max);

float		ofRadToDeg(float radians);
float		ofDegToRad(float degrees);
float 		ofLerpDegrees(float currentAngle, float targetAngle, float pct);
float 		ofLerpRadians(float currentAngle, float targetAngle, float pct);
float 		ofAngleDifferenceDegrees(float currentAngle, float targetAngle);
float 		ofAngleDifferenceRadians(float currentAngle, float targetAngle);
float		ofWrap(float value, float from, float to);
float		ofWrapRadians(float angle, float from = -PI, float to=+PI);
float		ofWrapDegrees(float angle, float from = -180, float to=+180);

float		ofRandomWidth();
float		ofRandomHeight();

			//returns noise in 0.0 to 1.0 range
float		ofNoise(float x);
float		ofNoise(float x, float y);
float		ofNoise(float x, float y, float z);
float		ofNoise(float x, float y, float z, float w);

			//returns noise in -1.0 to 1.0 range
float		ofSignedNoise(float x);
float		ofSignedNoise(float x, float y);
float		ofSignedNoise(float x, float y, float z);
float		ofSignedNoise(float x, float y, float z, float w);

bool        ofInsidePoly(float x, float y, const vector<ofPoint> & poly);
bool        ofInsidePoly(const ofPoint & p, const vector<ofPoint> & poly);

bool 		ofLineSegmentIntersection(ofPoint line1Start, ofPoint line1End, ofPoint line2Start, ofPoint line2End, ofPoint & intersection);

ofPoint 	ofBezierPoint( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t);
ofPoint 	ofCurvePoint( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t);
ofPoint 	ofBezierTangent( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t);
ofPoint 	ofCurveTangent( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t);

template<typename Type>
Type		ofInterpolateCosine(Type y1, Type y2, float pct);
template<typename Type>
Type		ofInterpolateCubic(Type y0, Type y1, Type y2, Type y3, float pct);
template<typename Type>
Type		ofInterpolateCatmullRom(Type y0, Type y1, Type y2, Type y3, float pct);
template<typename Type>
Type		ofInterpolateHermite(Type y0, Type y1, Type y2, Type y3, float pct);
template<typename Type>
Type		ofInterpolateHermite(Type y0, Type y1, Type y2, Type y3, float pct, float tension, float bias);





// from http://paulbourke.net/miscellaneous/interpolation/
//--------------------------------------------------
template<typename Type>
Type ofInterpolateCosine(Type y1, Type y2, float pct){
	float pct2;

	pct2 = (1-cos(pct*PI))/2;
	return(y1*(1-pct2)+y2*pct2);
}

// from http://paulbourke.net/miscellaneous/interpolation/
//--------------------------------------------------
template<typename Type>
Type ofInterpolateCubic(Type y0, Type y1, Type y2, Type y3, float pct){
	Type a0,a1,a2,a3;
	float pct2;

	pct2 = pct*pct;
	a0 = y3 - y2 - y0 + y1;
	a1 = y0 - y1 - a0;
	a2 = y2 - y0;
	a3 = y1;

	return(a0*pct*pct2+a1*pct2+a2*pct+a3);
}

// from http://paulbourke.net/miscellaneous/interpolation/
//--------------------------------------------------
template<typename Type>
Type ofInterpolateCatmullRom(Type y0, Type y1, Type y2, Type y3, float pct){
	Type a0,a1,a2,a3;
	float pct2 = pct*pct;
	a0 = -0.5*y0 + 1.5*y1 - 1.5*y2 + 0.5*y3;
	a1 = y0 - 2.5*y1 + 2*y2 - 0.5*y3;
	a2 = -0.5*y0 + 0.5*y2;
	a3 = y1;
	return(a0*pct*pct2 + a1*pct2 + a2*pct +a3);
}

// from http://musicdsp.org/showArchiveComment.php?ArchiveID=93
// laurent de soras
//--------------------------------------------------
template<typename Type>
inline Type ofInterpolateHermite(Type y0, Type y1, Type y2, Type y3, float pct){
	const Type c = (y2 - y0) * 0.5f;
	const Type v = y1 - y2;
	const Type w = c + v;
	const Type a = w + v + (y3 - y1) * 0.5f;
	const Type b_neg = w + a;

	return ((((a * pct) - b_neg) * pct + c) * pct + y1);
}

// from http://paulbourke.net/miscellaneous/interpolation/
//--------------------------------------------------
template<typename Type>
Type ofInterpolateHermite(Type y0, Type y1, Type y2, Type y3, float pct, float tension, float bias){
	float pct2,pct3;
	Type m0,m1;
	Type a0,a1,a2,a3;

	pct2 = pct * pct;
	pct3 = pct2 * pct;
	m0  = (y1-y0)*(1+bias)*(1-tension)/2;
	m0 += (y2-y1)*(1-bias)*(1-tension)/2;
	m1  = (y2-y1)*(1+bias)*(1-tension)/2;
	m1 += (y3-y2)*(1-bias)*(1-tension)/2;
	a0 =  Type(2*pct3 - 3*pct2 + 1);
	a1 =  Type(pct3 - 2*pct2 + pct);
	a2 =  Type(pct3 -   pct2);
	a3 =  Type(-2*pct3 + 3*pct2);

   return(a0*y1+a1*m0+a2*m1+a3*y2);
}
