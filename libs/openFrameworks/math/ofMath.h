#pragma once

#include "ofConstants.h"
#include <cmath>
#include <glm/gtc/constants.hpp>
#include <glm/fwd.hpp>

/// \file
/// ofMath provides a collection of mathematical utilities and functions.
///
/// \warning Many ofRandom-style functions wrap `rand()` which is not reentrant
/// or thread safe.  To generate random numbers simultaneously in multiple
/// threads, consider using c++11 uniform_real_distribution.
///
/// \sa http://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution

/// \name Random Numbers
/// \{

/// \brief Get a random floating point number between 0 and max.
///
/// A random number in the range [0, max) will be returned.
///
/// Example:
///	~~~~~{.cpp}
/// // Return a random floating point number between 0 and 20.
/// float randomNumber = ofRandom(20);
/// ~~~~~
///
/// \warning ofRandom wraps C++'s `rand()` which is not reentrant or thread safe.
///
/// \param max The maximum value of the random number.
float ofRandom(float max); 

/// \brief Get a random number between two values.
///
/// A random number in the range [min, max) will be returned.
///
/// Example:
///	~~~~~{.cpp}
/// // Return a random floating point number between -30 and 20.
/// float randomNumber = ofRandom(-30, 20);
/// ~~~~~
///
/// \warning ofRandom wraps `rand()` which is not reentrant or thread safe.
///
/// \param val0 the minimum value of the random number.
/// \param val1 The maximum value of the random number.
/// \returns A random floating point number between val0 and val1.
float ofRandom(float val0, float val1);

/// \brief Get a random floating point number.
///
/// \warning ofRandom wraps `rand()` which is not reentrant or thread safe.
///
/// \returns A random floating point number between -1 and 1.
float ofRandomf();

/// \brief Get a random unsigned floating point number.
///
/// \warning ofRandom wraps `rand()` which is not reentrant or thread safe.
///
/// \returns A random floating point number between 0 and 1.
float ofRandomuf();


/// \brief Get a random floating point number between 0 and the screen width.
///
/// A random number in the range [0, ofGetWidth()) will be returned.
///
/// \warning ofRandom wraps `rand()` which is not reentrant or thread safe.
///
/// \returns a random number between 0 and ofGetWidth().
float ofRandomWidth();

/// \brief Get a random floating point number between 0 and the screen height.
///
/// A random number in the range [0, ofGetHeight()) will be returned.
///
/// \warning ofRandom wraps `rand()` which is not reentrant or thread safe.
///
/// \returns a random number between 0 and ofGetHeight().
float ofRandomHeight();

/// \brief Seed the random number generator.
///
/// This passes a seed value to the random engine;
/// see of::random::Engine for details
///
/// \param val The value with which to seed the generator.
void ofSetRandomSeed(unsigned long new_seed);

/// \brief Seeds the random number generator with a unique value.
///
/// This seeds the old-school srand-based random number generator with an acceptably random value, 
/// generated from clock time and the PID.
void ofSeedRandom();

/// \brief Seed the random number generator.
///
/// If the user would like to repeat the same random sequence, a known random
/// seed can be used to initialize the random number generator during app
/// setup.  This can be useful for debugging and testing.
///
/// \param val The value with which to seed the generator.
[[deprecated("use ofSetRandomSeed() or of::random::seed() instead")]] void ofSeedRandom(int val);

/// \}

/// \name Number Ranges
/// \{

/// \brief Given a value and an input range, map the value to be within 0 and 1.
///
/// Often, you'll need to work with percentages or other methods that expect a
/// value between 0 and 1.  This function will take a minimum and maximum and
/// then finds where within that range a value sits.  If the value is outside
/// the range, it will be mapped to 0 or 1.
///
/// \param value The number to be normalized.
/// \param min The lower bound of the range.
/// \param max The upper bound of the range.
/// \returns A float between 0 and 1.
float ofNormalize(float value, float min, float max);

/// \brief Given a value and an input range, map the value to an output range.
/// 
/// ofMap linearly maps the given value to a new value given an input and output
/// range.  Thus if value is 50% of the way between inputMin and inputMax, the
/// output value will be 50% of the way between outpuMin and outputMax. For
/// an input value _outside_ of the intputMin and inputMax range, negative
/// percentages and percentages greater than 100% will be used.  For example, if
/// the input value is 150 and the input range is 0 - 100 and the output range
/// 0 - 1000, the output value will be 1500 or 150% of the total range.  The
/// user can avoid mapping outside of the input range by clamping the output
/// value.  Clamping is disabled by default and ensures that the output value
/// always stays in the range [outputMin, outputMax).
///
/// Example:
/// ~~~~~{.cpp}
///		float x = 5;
///		float newx = 0;
///		// 0 <= x < 10
///		newx = ofMap(x, 0, 10, 21, 22); // newx = 21.5 a value [21, 22).
/// ~~~~~
///
/// \param value The number to be mapped.
/// \param inputMin The lower bound of the input range.
/// \param inputMax The upper bound of the input range.
/// \param outputMin The lower bound of the output range.
/// \param outputMax The upper bound of the output range.
/// \param clamp True if the value should be clamped to [outputMin, outputMax).
/// \note If the absolute difference between inputMin and inputMax is less than
///		  std::numeric_limits<float>::epsilon(), outputMin will be returned to prevent divide by zero
///		  errors.
/// \returns a mapped floating point number.
float ofMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false);

/// \brief Clamp a value between min and max.
/// 
/// Restricts a value to be within a specified range defined by values min and
/// max. If the value is min <= value <= max, returns value.  If the value is
/// greater than max, return max; if the value is less than min, return min.
/// Otherwise, return the value unchanged.
/// 
/// ~~~~{.cpp}
///		float val = 10;
///		float newVal = 0;
///		newval = ofClamp(val, 30, 40); // newval = 30
///		newval = ofClamp(val, 0, 5);   // newval = 5
///		newval = ofClamp(val, 0, 20);  // newval = 10
/// ~~~~
///
/// \param value The number to be clamped.
/// \param min The lower bound of the range.
/// \param max The upper bound of the range.
/// \returns a floating point number in the range [min, max].
float ofClamp(float value, float min, float max);

/// \brief Determine if a number is inside of a giv(float)(en range.
/// \param t The value to test.
/// \param min The lower bound of the range.
/// \param max The upper bound of the range.
/// \returns true if the number t is the range of [min, max].
bool ofInRange(float t, float min, float max);

/// \brief Linearly interpolate a value within a range.
/// 
/// Calculates a number between two numbers [start, stop] at a specific increment
/// (amt).  If we want the new number to be between start and stop numbers, amt
/// needs to be a number between 0 and 1, inclusive.  ofLerp() does not clamp
/// the values.
///
/// ~~~~{.cpp}
///		float init = 1;
///		float end = 2;
///		float increment = 0.2;
///		float result = ofLerp(init, end, increment); // result = 1.2
///		// Values outside 0...1 work as well.
///		increment = 2;
///		result = ofLerp(init, end, increment); // result = 3
/// ~~~~
///
/// \sa float ofClamp(float value, float min, float max)
/// \param start The lower bound of the range.
/// \param stop The upper bound of the range.
/// \param amt The normalized [0, 1] value within the range to return.
/// \returns A float between start and stop.
float ofLerp(float start, float stop, float amt);

/// \}

//---------------------
/// \name Distance
/// \{

/// \brief Calculates the 2D distance between two points.
///
/// Uses the [Pythagorean theorem](http://en.wikipedia.org/wiki/Pythagorean_theorem).
///
/// \param x1 X position of first point.
/// \param y1 Y position of first point.
/// \param x2 X position of second point.
/// \param y2 Y position of second point.
/// \returns float Distance between points.
float ofDist(float x1, float y1, float x2, float y2);
///
/// \brief Calculates the 3D distance between two points.
///
/// Uses the [Pythagorean theorem](http://en.wikipedia.org/wiki/Pythagorean_theorem).
///
/// \param x1 X position of first point.
/// \param y1 Y position of first point.
/// \param z1 Z position of first point.
/// \param x2 X position of second point.
/// \param y2 Y position of second point.
/// \param z2 Z position of second point.
/// \returns float Distance between points.
float ofDist(float x1, float y1, float z1, float x2, float y2, float z2);

/// \brief Calculates the squared 2D distance between two points.
///
/// Same as ofDist() but doesn't take the square root sqrt() of the result,
/// which is a faster operation if you need to calculate and compare multiple
/// distances.
///
/// \param x1 X position of first point.
/// \param y1 Y position of first point.
/// \param x2 X position of second point.
/// \param y2 Y position of second point.
/// \returns distance-squared between two points.
float ofDistSquared(float x1, float y1, float x2, float y2);

/// \brief Calculates the squared 3D distance between two points.
///
/// Same as ofDist() but doesn't take the square root sqrt() of the result,
/// which is a faster operation if you need to calculate and compare multiple
/// distances.
///
/// \param x1 X position of first point.
/// \param y1 Y position of first point.
/// \param z1 Z position of first point.
/// \param x2 X position of second point.
/// \param y2 Y position of second point.
/// \param z2 Z position of second point.
/// \returns distance-squared between two points.
float ofDistSquared(float x1, float y1, float z1, float x2, float y2, float z2);

/// \}


/// \name Angles
/// \{

/// \brief Convert radians to degrees.
///
/// Example:
/// ~~~~{.cpp}
///		float result = ofRadToDeg(PI/2); // The result will be 90.
/// ~~~~
///
/// \param radians An angle in radians.
/// \returns the angle in degrees.
float ofRadToDeg(float radians);

/// \brief Convert degrees to radians.
///
/// Example:
/// ~~~~{.cpp}
///		float result = ofDegToRad(90); // The result will be PI/2.
/// ~~~~
///
/// \param degrees An angle in degrees.
/// \returns the angle in radians.
float ofDegToRad(float degrees);

/// \brief Linearly interpolate a value between two angles in degrees.
/// 
/// Calculates a number between two numbers [start, stop) at a specific
/// increment (amt). This does constrain the result into a single rotation,
/// but does not clamp the values
///
/// \param currentAngle The lower bound of the range in degrees.
/// \param targetAngle The upper bound of the range in degrees.
/// \param pct An amount between [0.0, 1.0] within the range to return.
/// \returns An angle in degrees between currentAngle and targetAngle.
float ofLerpDegrees(float currentAngle, float targetAngle, float pct);

/// \brief Linearly interpolate a value between two angles in radians.
/// 
/// Calculates a number between two numbers (start, stop) at a specific
/// increment (amt).  This does constrain the result into a single rotation, but
/// does not clamp the values
///
/// \param currentAngle The lower bound of the range in radians.
/// \param targetAngle The upper bound of the range in radians.
/// \param pct An amount between [0.0, 1.0] within the range to return.
/// \returns An angle in radians between currentAngle and targetAngle.
float ofLerpRadians(float currentAngle, float targetAngle, float pct);

/// \brief Calculates the difference between two angles in degrees.
///
/// This will calculate the actual difference, taking into account multiple
/// revolutions. For example:
///
/// ~~~~{.cpp}
///		ofAngleDifferenceDegrees(0, 90); // Returns 90.
///		ofAngleDifferenceDegrees(0, 450); // Also returns 90.
/// ~~~~
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
/// ~~~~{.cpp}
///		ofAngleDifferenceRadians(0, PI); // Returns -PI.
///		ofAngleDifferenceRadians(0, 3*PI); // Also returns -PI.
/// ~~~~
///
/// \param currentAngle The current angle in radians.
/// \param targetAngle the angle to be compared to in radians.
/// \returns The difference between two angles in radians.
float ofAngleDifferenceRadians(float currentAngle, float targetAngle);

/// \}

/// \name Number wrapping
/// \{


/// \brief Find a value within a given range, wrapping the value if it overflows.
///
/// If a value is between from and to, return that value.
/// If a value is NOT within that range, wrap it.
///
/// Example:
/// ~~~~{.cpp}
///		ofWrap(5, 0, 10); // Returns 5.
///		ofWrap(15, 0, 10); // Also returns 5.
///		ofWrap(-5, 0, 10); // Also returns 5.
/// ~~~~
///
/// \param value The value to map.
/// \param from The lower bound of the range.
/// \returns to The upper bound of the range.
float ofWrap(float value, float from, float to);

// \brief Convenience function for ofWrap(), constrained between -PI...PI
float ofWrapRadians(float angle, float from = -glm::pi<float>(), float to=glm::pi<float>());

// \brief Convenience function for ofWrap(), constrained between -180...180
float ofWrapDegrees(float angle, float from = -180, float to=+180);

/// \}

/// \name Noise
/// \{


/// \brief Calculates a one dimensional Perlin noise value between 0.0...1.0.
float ofNoise(float x);

/// \brief Calculates a two dimensional Perlin noise value between 0.0...1.0.
float ofNoise(float x, float y);

/// \brief Calculates a two dimensional Perlin noise value between 0.0...1.0.
float ofNoise(const glm::vec2& p);

/// \brief Calculates a three dimensional Perlin noise value between 0.0...1.0.
float ofNoise(float x, float y, float z);

/// \brief Calculates a three dimensional Perlin noise value between 0.0...1.0.
float ofNoise(const glm::vec3& p);

/// \brief Calculates a four dimensional Perlin noise value between 0.0...1.0.
float ofNoise(float x, float y, float z, float w);

/// \brief Calculates a four dimensional Perlin noise value between 0.0...1.0.
float ofNoise(const glm::vec4& p);

/// \brief Calculates a one dimensional Perlin noise value between -1.0...1.0.
float ofSignedNoise(float x);

/// \brief Calculates a two dimensional Perlin noise value between -1.0...1.0.
float ofSignedNoise(float x, float y);

/// \brief Calculates a two dimensional Perlin noise value between -1.0...1.0.
float ofSignedNoise(const glm::vec2& p);

/// \brief Calculates a three dimensional Perlin noise value between -1.0...1.0.
float ofSignedNoise(float x, float y, float z);

/// \brief Calculates a three dimensional Perlin noise value between -1.0...1.0.
float ofSignedNoise(const glm::vec3& p);

/// \brief Calculates a four dimensional Perlin noise value between -1.0...1.0.
float ofSignedNoise(float x, float y, float z, float w);

/// \brief Calculates a four dimensional Perlin noise value between -1.0...1.0.
float ofSignedNoise(const glm::vec4 & p);

/// \}


/// \name Geometry
/// \{


/// \brief Determine the intersection between two lines.
/// \param line1Start Starting point for first line.
/// \param line1End End point for first line.
/// \param line2Start Starting point for second line.
/// \param line2End End point for second line.
/// \param intersection glm::vec3 reference in which to store the computed intersection point.
/// \returns True if the lines intersect.
template<class vectype>
bool ofLineSegmentIntersection(const vectype& line1Start, const vectype& line1End, const vectype& line2Start, const vectype& line2End, vectype& intersection){
	vectype diffLA, diffLB;
	float compareA, compareB;
	diffLA = line1End - line1Start;
	diffLB = line2End - line2Start;
	compareA = diffLA.x*line1Start.y - diffLA.y*line1Start.x;
	compareB = diffLB.x*line2Start.y - diffLB.y*line2Start.x;
	if (
		(
			( ( diffLA.x*line2Start.y - diffLA.y*line2Start.x ) <= compareA ) ^
			( ( diffLA.x*line2End.y - diffLA.y*line2End.x ) <= compareA )
		)
		&&
		(
			( ( diffLB.x*line1Start.y - diffLB.y*line1Start.x ) <= compareB ) ^
			( ( diffLB.x*line1End.y - diffLB.y*line1End.x) <= compareB )
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

/// \brief Given the four points that determine a bezier curve, return an interpolated point on the curve.
/// \param a The beginning point of the curve.
/// \param b The first control point.
/// \param c The second control point.
/// \param d The end point of the curve.
/// \param t an offset along the curve, normalized between 0 and 1.
/// \returns A glm::vec3 on the curve.
template<class vectype>
vectype ofBezierPoint(const vectype& a, const vectype& b, const vectype& c, const vectype& d, float t){
	float tp = 1.0f - t;
	return a*tp*tp*tp + b*3*t*tp*tp + c*3*t*t*tp + d*t*t*t;
}

/// \brief Given the four points that determine a Catmull Rom curve, return an interpolated point on the curve.
/// \param a The first control point.
/// \param b The beginning point of the curve.
/// \param c The end point of the curve.
/// \param d The second control point.
/// \param t an offset along the curve, normalized between 0 and 1.
/// \returns A glm::vec3 on the curve.
template <class vectype>
vectype ofCurvePoint(const vectype& a, const vectype& b, const vectype& c, const vectype& d, float t){
	vectype pt;
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

/// Given the four points that determine a bezier curve and an offset along the curve, return an tangent vector to a point on the curve.
/// Currently this is not a normalized point, and will need to be normalized.
/// \param a The beginning point of the curve.
/// \param b The first control point.
/// \param c The second control point.
/// \param d The end point of the curve.
/// \param t an offset along the curve, normalized between 0 and 1.
/// \returns A glm::vec3 on the curve.
template <class vectype>
vectype ofBezierTangent(const vectype& a, const vectype& b, const vectype& c, const vectype& d, float t){
	return (d-a-c*3+b*3)*(t*t)*3 + (a+c-b*2)*t*6 - a*3+b*3;
}

/// \brief Return a tangent point for an offset along a Catmull Rom curve.
/// \param a The first control point.
/// \param b The beginning point of the curve.
/// \param c The end point of the curve.
/// \param d The second control point.
/// \param t an offset along the curve, normalized between 0 and 1.
/// \returns A glm::vec3 on the curve.
template <class vectype>
vectype ofCurveTangent(const vectype& a, const vectype& b, const vectype& c, const vectype& d, float t){
	auto v0 = ( c - a )*0.5;
	auto v1 = ( d - b )*0.5;
	return ( b*2 -c*2 + v0 + v1)*(3*t*t) + ( c*3 - b*3 - v1 - v0*2 )*( 2*t) + v0;

}

template<typename Type>
Type ofInterpolateCosine(const Type& y1, const Type& y2, float pct);
template<typename Type>
Type ofInterpolateCubic(const Type& y0, const Type& y1, const Type& y2, const Type& y3, float pct);
template<typename Type>
Type ofInterpolateCatmullRom(const Type& y0, const Type& y1, const Type& y2, const Type& y3, float pct);
template<typename Type>
Type ofInterpolateHermite(const Type& y0, const Type& y1, const Type& y2, const Type& y3, float pct);
template<typename Type>
Type ofInterpolateHermite(const Type& y0, const Type& y1, const Type& y2, const Type& y3, float pct, float tension, float bias);

/// \}


/// \name Others
/// \{

/// \brief Calculates the next larger power of 2.
///
/// If the input is already a power of 2, it will return itself.  
///
/// Example:
/// ~~~~{.cpp}
/// ofNextPow2(50); // returns 64
/// ofNextPow2(64); // returns 64
/// ofNextPow2(401); // returns 512
/// ~~~~
///
/// \param a The starting point for finding the next power of 2.
/// \returns value^2.
int ofNextPow2(int a);

/// \brief Returns the sign of a number.
/// \returns int -1 if n is negative, 1 if n is positive, and 0 is n == 0;
int ofSign(float n);

/// \brief Compare two floating point types for equality.
///
/// From C++ FAQ:
///
/// Floating point arithmetic is different from real number arithmetic.
/// Never use `==` to compare two floating point numbers.
///
/// This solution is not completely symmetric, meaning it is possible for
/// `ofIsFloatEqual(x, y) != ofIsFloatEqual(y, x)`. From a practical
/// standpoint, this does not usually occur when the magnitudes of x and y are
/// significantly larger than epsilon, but your mileage may vary.
///
/// \sa https://isocpp.org/wiki/faq/newbie#floating-point-arith
/// \sa https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html
/// \tparam The floating point data type.
/// \param a The first floating point type variable to compare.
/// \param b The second floating point type variable to compare.
/// \returns True if `std::abs(x - y) <= std::numeric_limits<Type>::epsilon() * std::abs(x)`.
template<typename Type>
typename std::enable_if<std::is_floating_point<Type>::value, bool>::type ofIsFloatEqual(const Type& a, const Type& b)
{
	return std::abs(a - b) <= std::numeric_limits<Type>::epsilon() * std::abs(a);
}

/// \}




// from http://paulbourke.net/miscellaneous/interpolation/
//--------------------------------------------------
template<typename Type>
Type ofInterpolateCosine(const Type& y1, const Type& y2, float pct){
	float pct2;

	pct2 = (1-cos(pct*glm::pi<float>()))/2;
	return(y1*(1-pct2)+y2*pct2);
}

// from http://paulbourke.net/miscellaneous/interpolation/
//--------------------------------------------------
template<typename Type>
Type ofInterpolateCubic(const Type& y0, const Type& y1, const Type& y2, const Type& y3, float pct){
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
Type ofInterpolateCatmullRom(const Type& y0, const Type& y1, const Type& y2, const Type& y3, float pct){
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
inline Type ofInterpolateHermite(const Type& y0, const Type& y1, const Type& y2, const Type& y3, float pct){
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
Type ofInterpolateHermite(const Type& y0, const Type& y1, const Type& y2, const Type& y3, float pct, float tension, float bias){
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
