/*
 *  ofQuaternion.h
 *  
 *  Created by Aaron Meyers on 6/22/09 -- modified by Arturo Castro and Zach Lieberman
 *  based on code from OSG - 
 *  see OSG license for more details: 
 *  http://www.openscenegraph.org/projects/osg/wiki/Legal
 * 
 */

#pragma once
#include "ofConstants.h"
#include "ofVec3f.h"
#include "ofVec4f.h"
#include <cmath>

#if (_MSC_VER)       
		// make microsoft visual studio complain less about double / float conversion.
		#pragma warning(disable : 4244)
#endif


class ofMatrix4x4;

class ofQuaternion {
public:
	float _v[4];

	inline ofQuaternion() {
		_v[0] = 0.0f;
		_v[1] = 0.0f;
		_v[2] = 0.0f;
		_v[3] = 1.0f;
	}
	inline ofQuaternion( float x, float y, float z, float w ) {
		_v[0] = x;
		_v[1] = y;
		_v[2] = z;
		_v[3] = w;
	}

	inline ofQuaternion( const ofVec4f& v ) {
		_v[0] = v.x;
		_v[1] = v.y;
		_v[2] = v.z;
		_v[3] = v.w;
	}

	inline ofQuaternion( float angle, const ofVec3f& axis) {
		makeRotate(angle, axis);
	}

	inline ofQuaternion( float angle1, const ofVec3f& axis1,
	                float angle2, const ofVec3f& axis2,
	                float angle3, const ofVec3f& axis3) {
		makeRotate(angle1, axis1, angle2, axis2, angle3, axis3);
	}

	inline ofQuaternion& operator = (const ofQuaternion& v) {
		_v[0] = v._v[0];
		_v[1] = v._v[1];
		_v[2] = v._v[2];
		_v[3] = v._v[3];
		return *this;
	}

	inline bool operator == (const ofQuaternion& v) const {
		return _v[0] == v._v[0] && _v[1] == v._v[1] && _v[2] == v._v[2] && _v[3] == v._v[3];
	}

	inline bool operator != (const ofQuaternion& v) const {
		return _v[0] != v._v[0] || _v[1] != v._v[1] || _v[2] != v._v[2] || _v[3] != v._v[3];
	}

	inline bool operator <  (const ofQuaternion& v) const {
		if (_v[0] < v._v[0]) return true;
		else if (_v[0] > v._v[0]) return false;
		else if (_v[1] < v._v[1]) return true;
		else if (_v[1] > v._v[1]) return false;
		else if (_v[2] < v._v[2]) return true;
		else if (_v[2] > v._v[2]) return false;
		else return (_v[3] < v._v[3]);
	}

	friend ostream& operator<<(ostream& os, const ofQuaternion& v);
	friend istream& operator>>(istream& is, ofQuaternion& v);
	

	/* ----------------------------------
	 Methods to access data members
	 ---------------------------------- */

	inline ofVec4f asVec4() const {
		return ofVec4f(_v[0], _v[1], _v[2], _v[3]);
	}

	inline ofVec3f asVec3() const {
		return ofVec3f(_v[0], _v[1], _v[2]);
	}

	inline void set(float x, float y, float z, float w) {
		_v[0] = x;
		_v[1] = y;
		_v[2] = z;
		_v[3] = w;
	}

	inline void set(const ofVec4f& v) {
		_v[0] = v.x;
		_v[1] = v.y;
		_v[2] = v.z;
		_v[3] = v.w;
	}

	void set(const ofMatrix4x4& matrix);

	void get(ofMatrix4x4& matrix) const;


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
	inline const ofQuaternion operator * (float rhs) const {
		return ofQuaternion(_v[0]*rhs, _v[1]*rhs, _v[2]*rhs, _v[3]*rhs);
	}

	/// Unary multiply by scalar
	inline ofQuaternion& operator *= (float rhs) {
		_v[0] *= rhs;
		_v[1] *= rhs;
		_v[2] *= rhs;
		_v[3] *= rhs;
		return *this;        // enable nesting
	}

	/// Binary multiply
	inline const ofQuaternion operator*(const ofQuaternion& rhs) const {
		return ofQuaternion( rhs._v[3]*_v[0] + rhs._v[0]*_v[3] + rhs._v[1]*_v[2] - rhs._v[2]*_v[1],
		                rhs._v[3]*_v[1] - rhs._v[0]*_v[2] + rhs._v[1]*_v[3] + rhs._v[2]*_v[0],
		                rhs._v[3]*_v[2] + rhs._v[0]*_v[1] - rhs._v[1]*_v[0] + rhs._v[2]*_v[3],
		                rhs._v[3]*_v[3] - rhs._v[0]*_v[0] - rhs._v[1]*_v[1] - rhs._v[2]*_v[2] );
	}

	/// Unary multiply
	inline ofQuaternion& operator*=(const ofQuaternion& rhs) {
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
	inline ofQuaternion operator / (float rhs) const {
		float div = 1.0 / rhs;
		return ofQuaternion(_v[0]*div, _v[1]*div, _v[2]*div, _v[3]*div);
	}

	/// Unary divide by scalar
	inline ofQuaternion& operator /= (float rhs) {
		float div = 1.0 / rhs;
		_v[0] *= div;
		_v[1] *= div;
		_v[2] *= div;
		_v[3] *= div;
		return *this;
	}

	/// Binary divide
	inline const ofQuaternion operator/(const ofQuaternion& denom) const {
		return ( (*this) * denom.inverse() );
	}

	/// Unary divide
	inline ofQuaternion& operator/=(const ofQuaternion& denom) {
		(*this) = (*this) * denom.inverse();
		return (*this);            // enable nesting
	}

	/// Binary addition
	inline const ofQuaternion operator + (const ofQuaternion& rhs) const {
		return ofQuaternion(_v[0] + rhs._v[0], _v[1] + rhs._v[1],
		               _v[2] + rhs._v[2], _v[3] + rhs._v[3]);
	}

	/// Unary addition
	inline ofQuaternion& operator += (const ofQuaternion& rhs) {
		_v[0] += rhs._v[0];
		_v[1] += rhs._v[1];
		_v[2] += rhs._v[2];
		_v[3] += rhs._v[3];
		return *this;            // enable nesting
	}

	/// Binary subtraction
	inline const ofQuaternion operator - (const ofQuaternion& rhs) const {
		return ofQuaternion(_v[0] - rhs._v[0], _v[1] - rhs._v[1],
		               _v[2] - rhs._v[2], _v[3] - rhs._v[3] );
	}

	/// Unary subtraction
	inline ofQuaternion& operator -= (const ofQuaternion& rhs) {
		_v[0] -= rhs._v[0];
		_v[1] -= rhs._v[1];
		_v[2] -= rhs._v[2];
		_v[3] -= rhs._v[3];
		return *this;            // enable nesting
	}

	/** Negation operator - returns the negative of the quaternion.
	 Basically just calls operator - () on the Vec4 */
	inline const ofQuaternion operator - () const {
		return ofQuaternion (-_v[0], -_v[1], -_v[2], -_v[3]);
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
	inline ofQuaternion conj () const {
		return ofQuaternion( -_v[0], -_v[1], -_v[2], _v[3] );
	}

	/// Multiplicative inverse method: q^(-1) = q^*/(q.q^*)
	inline const ofQuaternion inverse () const {
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
	void makeRotate( float  angle, const ofVec3f& vec );

	void makeRotate( float  angle1, const ofVec3f& axis1,
	                 float  angle2, const ofVec3f& axis2,
	                 float  angle3, const ofVec3f& axis3);

	/** Make a rotation Quat which will rotate vec1 to vec2.
	 Generally take a dot product to get the angle between these
	 and then use a cross product to get the rotation axis
	 Watch out for the two special cases when the vectors
	 are co-incident or opposite in direction.*/
	void makeRotate( const ofVec3f& vec1, const ofVec3f& vec2 );

	void makeRotate_original( const ofVec3f& vec1, const ofVec3f& vec2 );

	/** Return the angle and vector components represented by the quaternion.*/
	void getRotate ( float & angle, float & x, float & y, float & z ) const;

	/** Return the angle and vector represented by the quaternion.*/
	void getRotate ( float & angle, ofVec3f& vec ) const;

	/** Spherical Linear Interpolation.
	 As t goes from 0 to 1, the Quat object goes from "from" to "to". */
	void slerp   ( float  t, const ofQuaternion& from, const ofQuaternion& to);

	/** Rotate a vector by this quaternion.*/
	ofVec3f operator* (const ofVec3f& v) const {
		// nVidia SDK implementation
		ofVec3f uv, uuv;
		ofVec3f qvec(_v[0], _v[1], _v[2]);
		uv = qvec.getCrossed(v);	//uv = qvec ^ v;
		uuv = qvec.getCrossed(uv);	//uuv = qvec ^ uv;
		uv *= ( 2.0f * _v[3] );
		uuv *= 2.0f;
		return v + uv + uuv;
	}
};



inline ostream& operator<<(ostream& os, const ofQuaternion& v) {
	os << v._v[0] << ", " << v._v[1] << ", " << v._v[2] << ", " << v._v[3];
	return os;
}

inline istream& operator>>(istream& is, ofQuaternion& v) {
	is >> v._v[0];
	is.ignore(2);
	is >> v._v[1];
	is.ignore(2);
	is >> v._v[2];
	is.ignore(2);
	is >> v._v[3];
}
