
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

/// \brief Calculates the next larger power of 2.
/// \param int value.
/// \returns int value^2.
/// If the input is already a power of 2, it will return itself.  
/// Example:
///
///    ofNextPow2(50); // returns 64
///    ofNextPow2(64); // returns 64
///    ofNextPow(401)); // returns 512
int 		ofNextPow2 ( int a );

/// \brief Randomly seeds the random number generator.
/// This seeds the random number generator with an acceptably random value, generated from clock time and the PID.
void 		ofSeedRandom();

/// \brief Seeds the random number generator with a value for consistent randomness.
/// \param int The value with which to seed the generator.
void 		ofSeedRandom(int val);

/// \brief Returns a random floating point number between 0 and max.
/// \param float The maximum value of the random number.
float 		ofRandom(float max); 

/// \returns A random floating point number between val0 and val1.
/// \param float val0 the minimum value of the random number.
/// \param float val1 The maximum value of the random number.
/// Example:
///
///    ofRandom(-30,20); // will return a random floating point number between -30 and 20.
float 		ofRandom(float val0, float val1);		// random (x - y)

/// \returns A random floating point number between -1 and 1.
float 		ofRandomf();							// random (-1 - 1)

/// \Returns a random floating point number between 0 and 1.
float 		ofRandomuf();							// random (0 - 1) 

/// \brief Given a value and a range, remap the value to be within 0 and 1.
/// \param float value The number to be normalized.
/// \param float min The floor of the range.
/// \param float max The ceiling of the range.
/// Often, you'll need to work with percentages or other methods that expect a value between 0 and 1.
/// This function will take a minimum and maximum, and then finds where within that range a value sits.
/// If the value is outside the range, it will be mapped to 0 or 1.
/// \returns A float between 0 and 1.
float		ofNormalize(float value, float min, float max);

/// \brief Given a value and an input range, remap the value to be within an output range.
/// \param float value The number to be mapped.
/// \param float inputMin The floor of the input range.
/// \param float inputMax The ceiling of the input range.
/// \param float outputMin The floor of the output range.
/// \param float outputMax The ceiling of the output range.
/// \param bool clamp Should the value be clamped between outputMin and outputMax or allowed to extend beyond?
/// 
/// ofMap remaps the value passed in "value", calculating it's linear distance between inputMin and inputMax, 
/// and remapping it based on that percentage to outputMin and outputMax.
/// You can choose to clamp the results, which will constrain the results between outputMin and outputMax.  
/// Results are not clamped by default.
///
/// Example:
///     float x, newx;
///     x=5;
//      //0 < x < 10
//      newx = ofMap(x, 0, 10, 21, 22) //newx = 21.5 a value between 21 and 22
/// \returns a float, mapped between outputMin and outputMax.
float		ofMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false);

/// \brief Clamp a value between min and max.
/// \param float value The number to be clamped.
/// \param float min The floor of the range.
/// \param float max The ceiling of the range.
/// 
/// Restricts a value to be within a specified range defined by values min and max.   
/// If the value is min <= value <= max, returns value.
/// If the value is greater than max, return max; if the value is less than min, return min.
///        
///     float val, newval;
///     val=10;
///     newval=ofClamp(val,30,40); //newval = 30
///     newval=ofClamp(val,0,5); //newval = 5
///     newval=ofClamp(val,0,20); //newval = 10
///
/// \returns A float between min and max.
float		ofClamp(float value, float min, float max);

/// \brief Linearly interpolate a value be within a range.
/// \param float start The floor of the range.
/// \param float stop The ceiling of the range.
/// \param float amt The position within the range to return.
/// 
/// Calculates a number between two numbers (start,stop) at a specific increment (amt).
/// If we want the new number to be between start and stop numbers, amt needs to be a number between 0 and 1. 
/// ofLerp() does not clamp the values.
///
////     float init,end,increment,result;
///      increment=0.2;
///      init = 1;
///      end =2;
///      result=ofLerp(init, end, increment); //result = 1.2
///      // Values outside 0...1 work as well.
///      increment=2;
///      result=ofLerp(init, end, increment); //result = 3
///
/// \returns A float between start and stop.
float		ofLerp(float start, float stop, float amt);

/// \brief Calculates the distance between two points.
/// \param float x1 X position of first point.
/// \param float y1 Y position of first point.
/// \param float x2 X position of second point.
/// \param float y2 Y position of second point.
/// Uses <http://en.wikipedia.org/wiki/Pythagorean_theorem>
///
/// \returns float Distance between points.
float		ofDist(float x1, float y1, float x2, float y2);

/// \brief Calculates the distance between two points, without taking the square root of the result
/// \param float x1 X position of first point.
/// \param float y1 Y position of first point.
/// \param float x2 X position of second point.
/// \param float y2 Y position of second point.
/// Same as ofMath::ofDist() but doesn't take the sqrt() of the result, 
/// which is a faster operation if you need to calculate and compare multiple distances. 
/// \returns distance^2 between two points.
float		ofDistSquared(float x1, float y1, float x2, float y2);

/// \brief Returns the sign of a number.
/// \returns int -1 if n is negative, 1 if n is positive, and 0 is n == 0;
int			ofSign(float n);

/// \brief Returns true if the number t is the range of [min - max], false if it's not.
/// \param float t  The value to determine
/// \param float min The floor of the range.
/// \param float max The ceiling of the range.
/// \returns true if min<=t<=max.
bool		ofInRange(float t, float min, float max);

/// \brief Convert radians to degrees.
/// \param float radians  An angle in radians.
/// \returns float the angle in degrees.
float		ofRadToDeg(float radians);

/// \brief Convert degrees to radiant.
/// \param float degrees  An angle in degrees.
/// \returns float the angle in radians.
/// For example:
///     float result;
///     result = ofDegToRad(90); // result will be PI/2 
float		ofDegToRad(float degrees);


``
float 		ofLerpDegrees(float currentAngle, float targetAngle, float pct);
float 		ofLerpRadians(float currentAngle, float targetAngle, float pct);
float 		ofAngleDifferenceDegrees(float currentAngle, float targetAngle);
float 		ofAngleDifferenceRadians(float currentAngle, float targetAngle);
float		ofWrap(float value, float from, float to);
float		ofWrapRadians(float angle, float from = -PI, float to=+PI);
float		ofWrapDegrees(float angle, float from = -180, float to=+180);

/// \returns a random number between 0 and the width of the screen. 
float		ofRandomWidth();

/// \returns a random number between 0 and the height of the screen. 
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
