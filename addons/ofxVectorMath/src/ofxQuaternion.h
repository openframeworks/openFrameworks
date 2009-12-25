/*
 *  ofxQuaternion.h
 *  
 *  Created by Aaron Meyers on 6/22/09 -- modified by Arturo Castro and Zach Lieberman
 *  based on code from OSG - 
 *  see OSG license for more details: 
 *  http://www.openscenegraph.org/projects/osg/wiki/Legal
 * 
 */

#ifndef _OFX_QUATERNION
#define _OFX_QUATERNION

#include "ofxVec3f.h"
#include "ofxVec4f.h"
#include <cmath>

#if (_MSC_VER)       
		// make microsoft visual studio complain less about double / float conversion.
		#pragma warning(disable : 4244)
#endif


class ofxMatrix4x4;

class ofxQuaternion {
public:
	float _v[4];

	inline ofxQuaternion() {
		_v[0] = 0.0f;
		_v[1] = 0.0f;
		_v[2] = 0.0f;
		_v[3] = 1.0f;
	}
	inline ofxQuaternion( float x, float y, float z, float w ) {
		_v[0] = x;
		_v[1] = y;
		_v[2] = z;
		_v[3] = w;
	}

	inline ofxQuaternion( const ofxVec4f& v ) {
		_v[0] = v.x;
		_v[1] = v.y;
		_v[2] = v.z;
		_v[3] = v.w;
	}

	inline ofxQuaternion( float angle, const ofxVec3f& axis) {
		makeRotate(angle, axis);
	}

	inline ofxQuaternion( float angle1, const ofxVec3f& axis1,
	                float angle2, const ofxVec3f& axis2,
	                float angle3, const ofxVec3f& axis3) {
		makeRotate(angle1, axis1, angle2, axis2, angle3, axis3);
	}

	inline ofxQuaternion& operator = (const ofxQuaternion& v) {
		_v[0] = v._v[0];
		_v[1] = v._v[1];
		_v[2] = v._v[2];
		_v[3] = v._v[3];
		return *this;
	}

	inline bool operator == (const ofxQuaternion& v) const {
		return _v[0] == v._v[0] && _v[1] == v._v[1] && _v[2] == v._v[2] && _v[3] == v._v[3];
	}

	inline bool operator != (const ofxQuaternion& v) const {
		return _v[0] != v._v[0] || _v[1] != v._v[1] || _v[2] != v._v[2] || _v[3] != v._v[3];
	}

	inline bool operator <  (const ofxQuaternion& v) const {
		if (_v[0] < v._v[0]) return true;
		else if (_v[0] > v._v[0]) return false;
		else if (_v[1] < v._v[1]) return true;
		else if (_v[1] > v._v[1]) return false;
		else if (_v[2] < v._v[2]) return true;
		else if (_v[2] > v._v[2]) return false;
		else return (_v[3] < v._v[3]);
	}


	/* ----------------------------------
	 Methods to access data members
	 ---------------------------------- */

	inline ofxVec4f asVec4() const {
		return ofxVec4f(_v[0], _v[1], _v[2], _v[3]);
	}

	inline ofxVec3f asVec3() const {
		return ofxVec3f(_v[0], _v[1], _v[2]);
	}

	inline void set(float x, float y, float z, float w) {
		_v[0] = x;
		_v[1] = y;
		_v[2] = z;
		_v[3] = w;
	}

	inline void set(const ofxVec4f& v) {
		_v[0] = v.x;
		_v[1] = v.y;
		_v[2] = v.z;
		_v[3] = v.w;
	}

	void set(const ofxMatrix4x4& matrix);

	void get(ofxMatrix4x4& matrix) const;


	inline float & operator [] (int i) {
		return _v[i];
	}
	inline float   operator [] (int i) const {
		return _v[i];
	}

	inline float & x() {
		return _v[0];
	}
	inline float & y() {
		return _v[1];
	}
	inline float & z() {
		return _v[2];
	}
	inline float & w() {
		return _v[3];
	}

	inline float x() const {
		return _v[0];
	}
	inline float y() const {
		return _v[1];
	}
	inline float z() const {
		return _v[2];
	}
	inline float w() const {
		return _v[3];
	}

	/** return true if the Quat represents a zero rotation, and therefore can be ignored in computations.*/
	bool zeroRotation() const {
		return _v[0] == 0.0 && _v[1] == 0.0 && _v[2] == 0.0 && _v[3] == 1.0;
	}



	/* -------------------------------------------------------------
	 BASIC ARITHMETIC METHODS
	 Implemented in terms of Vec4s.  Some Vec4 operators, e.g.
	 operator* are not appropriate for quaternions (as
	 mathematical objects) so they are implemented differently.
	 Also define methods for conjugate and the multiplicative inverse.
	 ------------------------------------------------------------- */
	/// Multiply by scalar
	inline const ofxQuaternion operator * (float rhs) const {
		return ofxQuaternion(_v[0]*rhs, _v[1]*rhs, _v[2]*rhs, _v[3]*rhs);
	}

	/// Unary multiply by scalar
	inline ofxQuaternion& operator *= (float rhs) {
		_v[0] *= rhs;
		_v[1] *= rhs;
		_v[2] *= rhs;
		_v[3] *= rhs;
		return *this;        // enable nesting
	}

	/// Binary multiply
	inline const ofxQuaternion operator*(const ofxQuaternion& rhs) const {
		return ofxQuaternion( rhs._v[3]*_v[0] + rhs._v[0]*_v[3] + rhs._v[1]*_v[2] - rhs._v[2]*_v[1],
		                rhs._v[3]*_v[1] - rhs._v[0]*_v[2] + rhs._v[1]*_v[3] + rhs._v[2]*_v[0],
		                rhs._v[3]*_v[2] + rhs._v[0]*_v[1] - rhs._v[1]*_v[0] + rhs._v[2]*_v[3],
		                rhs._v[3]*_v[3] - rhs._v[0]*_v[0] - rhs._v[1]*_v[1] - rhs._v[2]*_v[2] );
	}

	/// Unary multiply
	inline ofxQuaternion& operator*=(const ofxQuaternion& rhs) {
		float x = rhs._v[3] * _v[0] + rhs._v[0] * _v[3] + rhs._v[1] * _v[2] - rhs._v[2] * _v[1];
		float y = rhs._v[3] * _v[1] - rhs._v[0] * _v[2] + rhs._v[1] * _v[3] + rhs._v[2] * _v[0];
		float z = rhs._v[3] * _v[2] + rhs._v[0] * _v[1] - rhs._v[1] * _v[0] + rhs._v[2] * _v[3];
		_v[3]   = rhs._v[3] * _v[3] - rhs._v[0] * _v[0] - rhs._v[1] * _v[1] - rhs._v[2] * _v[2];

		_v[2] = z;
		_v[1] = y;
		_v[0] = x;

		return (*this);            // enable nesting
	}

	/// Divide by scalar
	inline ofxQuaternion operator / (float rhs) const {
		float div = 1.0 / rhs;
		return ofxQuaternion(_v[0]*div, _v[1]*div, _v[2]*div, _v[3]*div);
	}

	/// Unary divide by scalar
	inline ofxQuaternion& operator /= (float rhs) {
		float div = 1.0 / rhs;
		_v[0] *= div;
		_v[1] *= div;
		_v[2] *= div;
		_v[3] *= div;
		return *this;
	}

	/// Binary divide
	inline const ofxQuaternion operator/(const ofxQuaternion& denom) const {
		return ( (*this) * denom.inverse() );
	}

	/// Unary divide
	inline ofxQuaternion& operator/=(const ofxQuaternion& denom) {
		(*this) = (*this) * denom.inverse();
		return (*this);            // enable nesting
	}

	/// Binary addition
	inline const ofxQuaternion operator + (const ofxQuaternion& rhs) const {
		return ofxQuaternion(_v[0] + rhs._v[0], _v[1] + rhs._v[1],
		               _v[2] + rhs._v[2], _v[3] + rhs._v[3]);
	}

	/// Unary addition
	inline ofxQuaternion& operator += (const ofxQuaternion& rhs) {
		_v[0] += rhs._v[0];
		_v[1] += rhs._v[1];
		_v[2] += rhs._v[2];
		_v[3] += rhs._v[3];
		return *this;            // enable nesting
	}

	/// Binary subtraction
	inline const ofxQuaternion operator - (const ofxQuaternion& rhs) const {
		return ofxQuaternion(_v[0] - rhs._v[0], _v[1] - rhs._v[1],
		               _v[2] - rhs._v[2], _v[3] - rhs._v[3] );
	}

	/// Unary subtraction
	inline ofxQuaternion& operator -= (const ofxQuaternion& rhs) {
		_v[0] -= rhs._v[0];
		_v[1] -= rhs._v[1];
		_v[2] -= rhs._v[2];
		_v[3] -= rhs._v[3];
		return *this;            // enable nesting
	}

	/** Negation operator - returns the negative of the quaternion.
	 Basically just calls operator - () on the Vec4 */
	inline const ofxQuaternion operator - () const {
		return ofxQuaternion (-_v[0], -_v[1], -_v[2], -_v[3]);
	}

	/// Length of the quaternion = sqrt( vec . vec )
	float length() const {
		return sqrt( _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2] + _v[3]*_v[3]);
	}

	/// Length of the quaternion = vec . vec
	float length2() const {
		return _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2] + _v[3]*_v[3];
	}

	/// Conjugate
	inline ofxQuaternion conj () const {
		return ofxQuaternion( -_v[0], -_v[1], -_v[2], _v[3] );
	}

	/// Multiplicative inverse method: q^(-1) = q^*/(q.q^*)
	inline const ofxQuaternion inverse () const {
		return conj() / length2();
	}

	/* --------------------------------------------------------
	 METHODS RELATED TO ROTATIONS
	 Set a quaternion which will perform a rotation of an
	 angle around the axis given by the vector (x,y,z).
	 Should be written to also accept an angle and a Vec3?

	 Define Spherical Linear interpolation method also

	 Not inlined - see the Quat.cpp file for implementation
	 -------------------------------------------------------- */
	void makeRotate( float  angle, float  x, float  y, float  z );
	void makeRotate( float  angle, const ofxVec3f& vec );

	void makeRotate( float  angle1, const ofxVec3f& axis1,
	                 float  angle2, const ofxVec3f& axis2,
	                 float  angle3, const ofxVec3f& axis3);

	/** Make a rotation Quat which will rotate vec1 to vec2.
	 Generally take a dot product to get the angle between these
	 and then use a cross product to get the rotation axis
	 Watch out for the two special cases when the vectors
	 are co-incident or opposite in direction.*/
	void makeRotate( const ofxVec3f& vec1, const ofxVec3f& vec2 );

	void makeRotate_original( const ofxVec3f& vec1, const ofxVec3f& vec2 );

	/** Return the angle and vector components represented by the quaternion.*/
	void getRotate ( float & angle, float & x, float & y, float & z ) const;

	/** Return the angle and vector represented by the quaternion.*/
	void getRotate ( float & angle, ofxVec3f& vec ) const;

	/** Spherical Linear Interpolation.
	 As t goes from 0 to 1, the Quat object goes from "from" to "to". */
	void slerp   ( float  t, const ofxQuaternion& from, const ofxQuaternion& to);

	/** Rotate a vector by this quaternion.*/
	ofxVec3f operator* (const ofxVec3f& v) const {
		// nVidia SDK implementation
		ofxVec3f uv, uuv;
		ofxVec3f qvec(_v[0], _v[1], _v[2]);
		uv = qvec.getCrossed(v);	//uv = qvec ^ v;
		uuv = qvec.getCrossed(uv);	//uuv = qvec ^ uv;
		uv *= ( 2.0f * _v[3] );
		uuv *= 2.0f;
		return v + uv + uuv;
	}
};

#endif
