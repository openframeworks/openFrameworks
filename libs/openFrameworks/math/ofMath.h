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
///
/// If the input is already a power of 2, it will return itself.  
/// Example:
///
///    ofNextPow2(50); // returns 64
///    ofNextPow2(64); // returns 64
///    ofNextPow(401)); // returns 512
///
/// \param value.
/// \returns value^2.
int ofNextPow2(int a);

/// \brief Randomly seeds the random number generator.
///
/// This seeds the random number generator with an acceptably random value, generated from clock time and the PID.
void ofSeedRandom();

/// \brief Seeds the random number generator with a value for consistent randomness.
///
/// \param int The value with which to seed the generator.
void ofSeedRandom(int val);

/// \brief Returns a random floating point number between 0 and max.
///
/// \param float The maximum value of the random number.
float ofRandom(float max); 

/// \brief returns a random number between two values.
///
/// Example:
///
///    // Return a random floating point number between -30 and 20.
///    ofRandom(-30, 20);
///
/// \param val0 the minimum value of the random number.
/// \param val1 The maximum value of the random number.
/// \returns A random floating point number between val0 and val1.
float ofRandom(float val0, float val1);

/// \returns A random floating point number between -1 and 1.
float ofRandomf();

/// \Returns a random floating point number between 0 and 1.
float ofRandomuf();

/// \brief Given a value and a range, remap the value to be within 0 and 1.
///
/// Often, you'll need to work with percentages or other methods that expect a
/// value between 0 and 1.  This function will take a minimum and maximum, and
/// then finds where within that range a value sits.  If the value is outside
/// the range, it will be mapped to 0 or 1.
///
/// \param value The number to be normalized.
/// \param min The floor of the range.
/// \param max The ceiling of the range.
/// \returns A float between 0 and 1.
float ofNormalize(float value, float min, float max);

/// \brief Given a value and an input range, remap the value to be within an output range.
/// 
/// ofMap remaps the value passed in "value", calculating it's linear distance
/// between inputMin and inputMax, and remapping it based on that percentage to
/// outputMin and outputMax.  You can choose to clamp the results, which will
/// constrain the results between outputMin and outputMax.  Results are not
/// clamped by default.
///
/// Example:
///
///     float x, newx;
///     x = 5;
///     // 0 < x < 10
///     newx = ofMap(x, 0, 10, 21, 22); // newx = 21.5 a value between 21 and 22
///
/// \param value The number to be mapped.
/// \param inputMin The floor of the input range.
/// \param inputMax The ceiling of the input range.
/// \param outputMin The floor of the output range.
/// \param outputMax The ceiling of the output range.
/// \param clamp true iff the value should be clamped between outputMin and outputMax.
/// \returns a float, mapped between outputMin and outputMax.
float ofMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false);

/// \brief Clamp a value between min and max.
/// 
/// Restricts a value to be within a specified range defined by values min and
/// max.  If the value is min <= value <= max, returns value.  If the value is
/// greater than max, return max; if the value is less than min, return min.
///        
///     float val, newval;
///     val = 10;
///     newval = ofClamp(val,30,40); // newval = 30
///     newval = ofClamp(val,0,5); // newval = 5
///     newval = ofClamp(val,0,20); // newval = 10
///
/// \param value The number to be clamped.
/// \param min The floor of the range.
/// \param max The ceiling of the range.
/// \returns A float between min and max.
float ofClamp(float value, float min, float max);

/// \brief Linearly interpolate a value be within a range.
/// 
/// Calculates a number between two numbers (start,stop) at a specific increment
/// (amt).  If we want the new number to be between start and stop numbers, amt
/// needs to be a number between 0 and 1.  ofLerp() does not clamp the values.
///
///     float init = 1;
///     float end = 2;
///     float increment = 0.2;
///     float result = ofLerp(init, end, increment); // result = 1.2
///     // Values outside 0...1 work as well.
///     increment = 2;
///     result = ofLerp(init, end, increment); // result = 3
///
/// \param start The floor of the range.
/// \param stop The ceiling of the range.
/// \param amt The position within the range to return.
/// \returns A float between start and stop.
float ofLerp(float start, float stop, float amt);

/// \brief Calculates the distance between two points.
///
/// Uses <http://en.wikipedia.org/wiki/Pythagorean_theorem>
///
/// \param x1 X position of first point.
/// \param y1 Y position of first point.
/// \param x2 X position of second point.
/// \param y2 Y position of second point.
/// \returns float Distance between points.
float ofDist(float x1, float y1, float x2, float y2);

/// \brief Calculates the distance between two points, without taking the square root of the result
///
/// Same as ofMath::ofDist() but doesn't take the sqrt() of the result, 
/// which is a faster operation if you need to calculate and compare multiple
/// distances.
///
/// \param x1 X position of first point.
/// \param y1 Y position of first point.
/// \param x2 X position of second point.
/// \param y2 Y position of second point.
/// \returns distance^2 between two points.
float ofDistSquared(float x1, float y1, float x2, float y2);

/// \brief Returns the sign of a number.
/// \returns int -1 if n is negative, 1 if n is positive, and 0 is n == 0;
int ofSign(float n);

/// \brief Determines if a number is inside of a given range.
/// \param t The value to test.
/// \param min The floor of the range.
/// \param max The ceiling of the range.
/// \returns true iff the number t is the range of [min - max].
bool ofInRange(float t, float min, float max);

/// \brief Convert radians to degrees.
/// \param radians An angle in radians.
/// \returns the angle in degrees.
float ofRadToDeg(float radians);

/// \brief Convert degrees to radians.
///
/// Example:
///
///     float result = ofDegToRad(90); // result will be PI/2
///
/// \param degrees An angle in degrees.
/// \returns the angle in radians.
float ofDegToRad(float degrees);

/// \brief Linearly interpolate a value between two angles in degrees.
/// 
/// Calculates a number between two numbers (start, stop) at a specific
/// increment (amt). This does constrain the result into a single rotation,
/// but does not clamp the values
///
/// \param currentAngle The floor of the range in degrees.
/// \param targetAngle The ceiling of the range in degrees.
/// \param pct An amount between 0.0..1.0 within the range to return.
/// \returns An angle in degrees between currentAngle and targetAngle.
float ofLerpDegrees(float currentAngle, float targetAngle, float pct);

/// \brief Linearly interpolate a value between two angles in radians.
/// 
/// Calculates a number between two numbers (start, stop) at a specific
/// increment (amt).  This does constrain the result into a single rotation, but
/// does not clamp the values
///
/// \param currentAngle The floor of the range in radians.
/// \param targetAngle The ceiling of the range in radians.
/// \param pct An amount between 0.0..1.0 within the range to return.
/// \returns An angle in radians between currentAngle and targetAngle.
float ofLerpRadians(float currentAngle, float targetAngle, float pct);

/// \brief Calculates the difference between two angles in degrees.
///
/// This will calculate the actual difference, taking into account multiple
/// revolutions. For example:
///
///     ofAngleDifferenceDegrees(0,90); // returns 90;
///     ofAngleDifferenceDegrees(0,450); // also returns 90;
///
/// \param currentAngle The current angle in degrees.
/// \param targetAngle the angle to be compared to in degrees.
/// \returns The difference between two angles in degrees.
float ofAngleDifferenceDegrees(float currentAngle, float targetAngle);

/// \brief Calculates the difference between two angles in radians.
///
/// This will calculate the actual difference, taking into account multiple
/// revolutions. For example:
///
///     ofAngleDifferenceRadians(0,PI); // returns -PI;
///     ofAngleDifferenceRadians(0,3*PI); // also returns -PI;
///
/// \param currentAngle The current angle in radians.
/// \param targetAngle the angle to be compared to in radians.
/// \returns The difference between two angles in radians.
float ofAngleDifferenceRadians(float currentAngle, float targetAngle);

/// \brief Find a value within a given range, wrapping the value if it overflows.
///
/// If a value is between from and to, return that value.
/// If a value is NOT within that range, wrap it.
///
/// For example:
/// 
///     ofWrap(5,0,10); // returns 5;
///     ofWrap(15,0,10); // also returns 5;
///     ofWrap(-5,0,10); // also returns 5;
///
/// \param value The value to map.
/// \param from The floor of the range.
/// \returns to The ceiling of the range.
float ofWrap(float value, float from, float to);

// \brief Convenience function for ofMath::ofWrap(), constrained between -PI...PI
float ofWrapRadians(float angle, float from = -PI, float to=+PI);

// \brief Convenience function for ofMath::ofWrap(), constrained between -180...180
float ofWrapDegrees(float angle, float from = -180, float to=+180);

/// \returns a random number between 0 and the width of the window.
float ofRandomWidth();

/// \returns a random number between 0 and the height of the window. 
float ofRandomHeight();

/// \brief Calculates a one dimensional Perlin noise value between 0.0...1.0.
float ofNoise(float x);

/// \brief Calculates a two dimensional Perlin noise value between 0.0...1.0.
float ofNoise(float x, float y);

/// \brief Calculates a three dimensional Perlin noise value between 0.0...1.0.
float ofNoise(float x, float y, float z);

/// \brief Calculates a four dimensional Perlin noise value between 0.0...1.0.
float ofNoise(float x, float y, float z, float w);

/// \brief Calculates a one dimensional Perlin noise value between -1.0...1.0.
float ofSignedNoise(float x);

/// \brief Calculates a two dimensional Perlin noise value between -1.0...1.0.
float ofSignedNoise(float x, float y);

/// \brief Calculates a three dimensional Perlin noise value between -1.0...1.0.
float ofSignedNoise(float x, float y, float z);

/// \brief Calculates a four dimensional Perlin noise value between -1.0...1.0.
float ofSignedNoise(float x, float y, float z, float w);

/// \brief Determine if an (x,y) coordinate is within the polygon defined by a vector of ofPoints.
/// \param x The x dimension of the coordinate.
/// \param y The y dimension of the coordinate.
/// \param poly a vector of ofPoints defining a polygon.
/// \returns True if the point defined by the coordinates is enclosed, false otherwise.
bool ofInsidePoly(float x, float y, const vector<ofPoint> & poly);

/// \brief Determine if an ofPoint is within the polygon defined by a vector of ofPoints.
/// \param p A point to check.
/// \param poly A vector of ofPoints defining a polygon.
/// \returns True if the ofPoint is enclosed, false otherwise.
bool ofInsidePoly(const ofPoint & p, const vector<ofPoint> & poly);

/// \brief Determine the intersection between two lines.
/// \param line1Start Starting point for first line.
/// \param line1End End point for first line.
/// \param line2Start Starting point for second line.
/// \param line2End End point for second line.
/// \param intersection ofPoint reference in which to store the computed intersection point.
/// \returns True if the lines intersect.
bool ofLineSegmentIntersection(ofPoint line1Start, ofPoint line1End, ofPoint line2Start, ofPoint line2End, ofPoint & intersection);

/// \brief Given the four points that determine a bezier curve, return an interpolated point on the curve.
/// \param a The beginning point of the curve.
/// \param b The first control point.
/// \param c The second control point.
/// \param d The end point of the curve.
/// \param t an offset along the curve, normalized between 0 and 1.
/// \returns A ofPoint on the curve.
ofPoint ofBezierPoint( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t);

/// \brief Given the four points that determine a Catmull Rom curve, return an interpolated point on the curve.
/// \param a The first control point.
/// \param b The beginning point of the curve.
/// \param c The end point of the curve.
/// \param d The second control point.
/// \param t an offset along the curve, normalized between 0 and 1.
/// \returns A ofPoint on the curve.
ofPoint ofCurvePoint( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t);

/// Given the four points that determine a bezier curve and an offset along the curve, return an tangent vector to a point on the curve.
/// Currently this is not a normalized point, and will need to be normalized.
/// \param a The beginning point of the curve.
/// \param b The first control point.
/// \param c The second control point.
/// \param d The end point of the curve.
/// \param t an offset along the curve, normalized between 0 and 1.
/// \returns A ofPoint on the curve.
ofPoint ofBezierTangent( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t);

/// \brief Return a tangent point for an offset along a Catmull Rom curve.
/// \param a The first control point.
/// \param b The beginning point of the curve.
/// \param c The end point of the curve.
/// \param d The second control point.
/// \param t an offset along the curve, normalized between 0 and 1.
/// \returns A ofPoint on the curve.
ofPoint ofCurveTangent( ofPoint a, ofPoint b, ofPoint c, ofPoint d, float t);

template<typename Type>
Type ofInterpolateCosine(Type y1, Type y2, float pct);
template<typename Type>
Type ofInterpolateCubic(Type y0, Type y1, Type y2, Type y3, float pct);
template<typename Type>
Type ofInterpolateCatmullRom(Type y0, Type y1, Type y2, Type y3, float pct);
template<typename Type>
Type ofInterpolateHermite(Type y0, Type y1, Type y2, Type y3, float pct);
template<typename Type>
Type ofInterpolateHermite(Type y0, Type y1, Type y2, Type y3, float pct, float tension, float bias);





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
