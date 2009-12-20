/*
 *  ofxMatrix4x4.h
 *  splineTest
 *
 *  Created by Aaron Meyers on 6/22/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFX_MATRIX_4X4
#define _OFX_MATRIX_4X4 1

#include "ofxVec3f.h"
#include "ofxVec4f.h"
#include "ofxQuat.h"
#include <cmath>


class ofxMatrix4x4;

class ofxMatrix4x4 {
public:
	float _mat[4][4];

	inline ofxMatrix4x4() {
		makeIdentity();
	}
	inline ofxMatrix4x4( const ofxMatrix4x4& mat) {
		set(mat.ptr());
	}
	inline explicit ofxMatrix4x4( float const * const ptr ) {
		set(ptr);
	}
	inline explicit ofxMatrix4x4( const ofxQuat& quat ) {
		makeRotate(quat);
	}
	ofxMatrix4x4(	float a00, float a01, float a02, float a03,
	              float a10, float a11, float a12, float a13,
	              float a20, float a21, float a22, float a23,
	              float a30, float a31, float a32, float a33);

	~ofxMatrix4x4() {}

//	int compare(const ofxMatrix4x4& m) const;
//
//	bool operator < (const ofxMatrix4x4& m) const { return compare(m)<0; }
//	bool operator == (const ofxMatrix4x4& m) const { return compare(m)==0; }
//	bool operator != (const ofxMatrix4x4& m) const { return compare(m)!=0; }

	inline float& operator()(int row, int col) {
		return _mat[row][col];
	}
	inline float operator()(int row, int col) const {
		return _mat[row][col];
	}

	inline bool valid() const {
		return !isNaN();
	}
	inline bool isNaN() const {
		return std::isnan(_mat[0][0]) || std::isnan(_mat[0][1]) || std::isnan(_mat[0][2]) || std::isnan(_mat[0][3]) ||
		       std::isnan(_mat[1][0]) || std::isnan(_mat[1][1]) || std::isnan(_mat[1][2]) || std::isnan(_mat[1][3]) ||
		       std::isnan(_mat[2][0]) || std::isnan(_mat[2][1]) || std::isnan(_mat[2][2]) || std::isnan(_mat[2][3]) ||
		       std::isnan(_mat[3][0]) || std::isnan(_mat[3][1]) || std::isnan(_mat[3][2]) || std::isnan(_mat[3][3]);
	}

	inline ofxMatrix4x4& operator = (const ofxMatrix4x4& rhs) {
		if ( &rhs == this ) return *this;
		set(rhs.ptr());
		return *this;
	}

	inline void set(const ofxMatrix4x4& rhs) {
		set(rhs.ptr());
	}

	inline void set(float const * const ptr) {
		float* local_ptr = (float*)_mat;
		for (int i = 0;i < 16;++i) local_ptr[i] = (float)ptr[i];
	}

	inline void set(double const * const ptr) {
		float* local_ptr = (float*)_mat;
		for (int i = 0;i < 16;++i) local_ptr[i] = (float)ptr[i];
	}

	void set(float a00, float a01, float a02, float a03,
	         float a10, float a11, float a12, float a13,
	         float a20, float a21, float a22, float a23,
	         float a30, float a31, float a32, float a33);

	float * ptr() {
		return (float*)_mat;
	}
	const float * ptr() const {
		return (const float *)_mat;
	}

	bool isIdentity() const {
		return _mat[0][0] == 1.0f && _mat[0][1] == 0.0f && _mat[0][2] == 0.0f &&  _mat[0][3] == 0.0f &&
		       _mat[1][0] == 0.0f && _mat[1][1] == 1.0f && _mat[1][2] == 0.0f &&  _mat[1][3] == 0.0f &&
		       _mat[2][0] == 0.0f && _mat[2][1] == 0.0f && _mat[2][2] == 1.0f &&  _mat[2][3] == 0.0f &&
		       _mat[3][0] == 0.0f && _mat[3][1] == 0.0f && _mat[3][2] == 0.0f &&  _mat[3][3] == 1.0f;
	}


	void makeIdentity();

	void makeScale( const ofxVec3f& );
	void makeScale( float, float, float );

	void makeTranslate( const ofxVec3f& );
	void makeTranslate( float, float, float );

	void makeRotate( const ofxVec3f& from, const ofxVec3f& to );
	void makeRotate( float angle, const ofxVec3f& axis );
	void makeRotate( float angle, float x, float y, float z );
	void makeRotate( const ofxQuat& );
	void makeRotate( float angle1, const ofxVec3f& axis1,
	                 float angle2, const ofxVec3f& axis2,
	                 float angle3, const ofxVec3f& axis3);


	/** decompose the matrix into translation, rotation, scale and scale orientation.*/
	void decompose( ofxVec3f& translation,
	                ofxQuat& rotation,
	                ofxVec3f& scale,
	                ofxQuat& so ) const;


	/** Set to an orthographic projection.
	 * See glOrtho for further details.
	 */
	void makeOrtho(double left,   double right,
	               double bottom, double top,
	               double zNear,  double zFar);

	/** Get the orthographic settings of the orthographic projection matrix.
	 * Note, if matrix is not an orthographic matrix then invalid values
	 * will be returned.
	 */
	bool getOrtho(double& left,   double& right,
	              double& bottom, double& top,
	              double& zNear,  double& zFar) const;

	/** Set to a 2D orthographic projection.
	 * See glOrtho2D for further details.
	 */
	inline void makeOrtho2D(double left,   double right,
	                        double bottom, double top) {
		makeOrtho(left, right, bottom, top, -1.0, 1.0);
	}


	/** Set to a perspective projection.
	 * See glFrustum for further details.
	 */
	void makeFrustum(double left,   double right,
	                 double bottom, double top,
	                 double zNear,  double zFar);

	/** Get the frustum settings of a perspective projection matrix.
	 * Note, if matrix is not a perspective matrix then invalid values
	 * will be returned.
	 */
	bool getFrustum(double& left,   double& right,
	                double& bottom, double& top,
	                double& zNear,  double& zFar) const;

	/** Set to a symmetrical perspective projection.
	 * See gluPerspective for further details.
	 * Aspect ratio is defined as width/height.
	 */
	void makePerspective(double fovy,  double aspectRatio,
	                     double zNear, double zFar);

	/** Get the frustum settings of a symmetric perspective projection
	 * matrix.
	 * Return false if matrix is not a perspective matrix,
	 * where parameter values are undefined.
	 * Note, if matrix is not a symmetric perspective matrix then the
	 * shear will be lost.
	 * Asymmetric matrices occur when stereo, power walls, caves and
	 * reality center display are used.
	 * In these configuration one should use the AsFrustum method instead.
	 */
	bool getPerspective(double& fovy,  double& aspectRatio,
	                    double& zNear, double& zFar) const;

	/** Set the position and orientation to be a view matrix,
	 * using the same convention as gluLookAt.
	 */
	void makeLookAt(const ofxVec3f& eye, const ofxVec3f& center, const ofxVec3f& up);

	/** Get to the position and orientation of a modelview matrix,
	 * using the same convention as gluLookAt.
	 */
	void getLookAt(ofxVec3f& eye, ofxVec3f& center, ofxVec3f& up,
	               float lookDistance = 1.0f) const;

	/** invert the matrix rhs, automatically select invert_4x3 or invert_4x4. */
	inline bool invert( const ofxMatrix4x4& rhs) {
		bool is_4x3 = (rhs._mat[0][3] == 0.0f && rhs._mat[1][3] == 0.0f &&  rhs._mat[2][3] == 0.0f && rhs._mat[3][3] == 1.0f);
		return is_4x3 ? invert_4x3(rhs) :  invert_4x4(rhs);
	}

	/** 4x3 matrix invert, not right hand column is assumed to be 0,0,0,1. */
	bool invert_4x3( const ofxMatrix4x4& rhs);

	/** full 4x4 matrix invert. */
	bool invert_4x4( const ofxMatrix4x4& rhs);

	/** ortho-normalize the 3x3 rotation & scale matrix */
	void orthoNormalize(const ofxMatrix4x4& rhs);

	//basic utility functions to create new matrices
	inline static ofxMatrix4x4 identity( void );
	inline static ofxMatrix4x4 scale( const ofxVec3f& sv);
	inline static ofxMatrix4x4 scale( float sx, float sy, float sz);
	inline static ofxMatrix4x4 translate( const ofxVec3f& dv);
	inline static ofxMatrix4x4 translate( float x, float y, float z);
	inline static ofxMatrix4x4 rotate( const ofxVec3f& from, const ofxVec3f& to);
	inline static ofxMatrix4x4 rotate( float angle, float x, float y, float z);
	inline static ofxMatrix4x4 rotate( float angle, const ofxVec3f& axis);
	inline static ofxMatrix4x4 rotate( float angle1, const ofxVec3f& axis1,
	                                   float angle2, const ofxVec3f& axis2,
	                                   float angle3, const ofxVec3f& axis3);
	inline static ofxMatrix4x4 rotate( const ofxQuat& quat);
	inline static ofxMatrix4x4 inverse( const ofxMatrix4x4& matrix);
	inline static ofxMatrix4x4 transpose( const ofxMatrix4x4& matrix);
	inline static ofxMatrix4x4 orthoNormal(const ofxMatrix4x4& matrix);

	/** Create an orthographic projection matrix.
	 * See glOrtho for further details.
	 */
	inline static ofxMatrix4x4 ortho(double left,   double right,
	                                 double bottom, double top,
	                                 double zNear,  double zFar);

	/** Create a 2D orthographic projection.
	 * See glOrtho for further details.
	 */
	inline static ofxMatrix4x4 ortho2D(double left,   double right,
	                                   double bottom, double top);

	/** Create a perspective projection.
	 * See glFrustum for further details.
	 */
	inline static ofxMatrix4x4 frustum(double left,   double right,
	                                   double bottom, double top,
	                                   double zNear,  double zFar);

	/** Create a symmetrical perspective projection.
	 * See gluPerspective for further details.
	 * Aspect ratio is defined as width/height.
	 */
	inline static ofxMatrix4x4 perspective(double fovy,  double aspectRatio,
	                                       double zNear, double zFar);

	/** Create the position and orientation as per a camera,
	 * using the same convention as gluLookAt.
	 */
	inline static ofxMatrix4x4 lookAt(const ofxVec3f& eye,
	                                  const ofxVec3f& center,
	                                  const ofxVec3f& up);

	inline ofxVec3f preMult( const ofxVec3f& v ) const;
	inline ofxVec3f postMult( const ofxVec3f& v ) const;
	inline ofxVec3f operator* ( const ofxVec3f& v ) const;
	inline ofxVec4f preMult( const ofxVec4f& v ) const;
	inline ofxVec4f postMult( const ofxVec4f& v ) const;
	inline ofxVec4f operator* ( const ofxVec4f& v ) const;

	void setRotate(const ofxQuat& q);
	/** Get the matrix rotation as a Quat. Note that this function
	 * assumes a non-scaled matrix and will return incorrect results
	 * for scaled matrixces. Consider decompose() instead.
	 */
	ofxQuat getRotate() const;


	void setTrans( float tx, float ty, float tz );
	void setTrans( const ofxVec3f& v );

	inline ofxVec3f getTrans() const {
		return ofxVec3f(_mat[3][0], _mat[3][1], _mat[3][2]);
	}

	inline ofxVec3f getScale() const {
		ofxVec3f x_vec(_mat[0][0], _mat[1][0], _mat[2][0]);
		ofxVec3f y_vec(_mat[0][1], _mat[1][1], _mat[2][1]);
		ofxVec3f z_vec(_mat[0][2], _mat[1][2], _mat[2][2]);
		return ofxVec3f(x_vec.length(), y_vec.length(), z_vec.length());
	}

	/** apply a 3x3 transform of v*M[0..2,0..2]. */
	inline static ofxVec3f transform3x3(const ofxVec3f& v, const ofxMatrix4x4& m);

	/** apply a 3x3 transform of M[0..2,0..2]*v. */
	inline static ofxVec3f transform3x3(const ofxMatrix4x4& m, const ofxVec3f& v);

	// basic Matrixf multiplication, our workhorse methods.
	void mult( const ofxMatrix4x4&, const ofxMatrix4x4& );
	void preMult( const ofxMatrix4x4& );
	void postMult( const ofxMatrix4x4& );

	/** Optimized version of preMult(translate(v)); */
	inline void preMultTranslate( const ofxVec3f& v );
	/** Optimized version of postMult(translate(v)); */
	inline void postMultTranslate( const ofxVec3f& v );

	/** Optimized version of preMult(scale(v)); */
	inline void preMultScale( const ofxVec3f& v );
	/** Optimized version of postMult(scale(v)); */
	inline void postMultScale( const ofxVec3f& v );

	/** Optimized version of preMult(rotate(q)); */
	inline void preMultRotate( const ofxQuat& q );
	/** Optimized version of postMult(rotate(q)); */
	inline void postMultRotate( const ofxQuat& q );

	// AARON METHODS
	inline void postMultTranslate(float x, float y, float z);
	inline void postMultRotate(float angle, float x, float y, float z);
	inline void postMultScale(float x, float y, float z);

	inline void operator *= ( const ofxMatrix4x4& other ) {
		if ( this == &other ) {
			ofxMatrix4x4 temp(other);
			postMult( temp );
		} else postMult( other );
	}

	inline ofxMatrix4x4 operator * ( const ofxMatrix4x4 &m ) const {
		ofxMatrix4x4 r;
		r.mult(*this, m);
		return  r;
	}
};



//static utility methods
inline ofxMatrix4x4 ofxMatrix4x4::identity(void) {
	ofxMatrix4x4 m;
	m.makeIdentity();
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::scale(float sx, float sy, float sz) {
	ofxMatrix4x4 m;
	m.makeScale(sx, sy, sz);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::scale(const ofxVec3f& v ) {
	return scale(v.x, v.y, v.z );
}

inline ofxMatrix4x4 ofxMatrix4x4::translate(float tx, float ty, float tz) {
	ofxMatrix4x4 m;
	m.makeTranslate(tx, ty, tz);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::translate(const ofxVec3f& v ) {
	return translate(v.x, v.y, v.z );
}

inline ofxMatrix4x4 ofxMatrix4x4::rotate( const ofxQuat& q ) {
	return ofxMatrix4x4(q);
}
inline ofxMatrix4x4 ofxMatrix4x4::rotate(float angle, float x, float y, float z ) {
	ofxMatrix4x4 m;
	m.makeRotate(angle, x, y, z);
	return m;
}
inline ofxMatrix4x4 ofxMatrix4x4::rotate(float angle, const ofxVec3f& axis ) {
	ofxMatrix4x4 m;
	m.makeRotate(angle, axis);
	return m;
}
inline ofxMatrix4x4 ofxMatrix4x4::rotate(	float angle1, const ofxVec3f& axis1,
    float angle2, const ofxVec3f& axis2,
    float angle3, const ofxVec3f& axis3) {
	ofxMatrix4x4 m;
	m.makeRotate(angle1, axis1, angle2, axis2, angle3, axis3);
	return m;
}
inline ofxMatrix4x4 ofxMatrix4x4::rotate(const ofxVec3f& from, const ofxVec3f& to ) {
	ofxMatrix4x4 m;
	m.makeRotate(from, to);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::inverse( const ofxMatrix4x4& matrix) {
	ofxMatrix4x4 m;
	m.invert(matrix);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::transpose( const ofxMatrix4x4& matrix) {
	ofxMatrix4x4 m(matrix._mat[0][0], matrix._mat[1][0], matrix._mat[2][0],
	               matrix._mat[3][0], matrix._mat[0][1], matrix._mat[1][1], matrix._mat[2][1],
	               matrix._mat[3][1], matrix._mat[0][2], matrix._mat[1][2], matrix._mat[2][2],
	               matrix._mat[3][2], matrix._mat[0][3], matrix._mat[1][3], matrix._mat[2][3],
	               matrix._mat[3][3]);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::orthoNormal(const ofxMatrix4x4& matrix) {
	ofxMatrix4x4 m;
	m.orthoNormalize(matrix);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::ortho(double left, double right,
                                        double bottom, double top,
                                        double zNear, double zFar) {
	ofxMatrix4x4 m;
	m.makeOrtho(left, right, bottom, top, zNear, zFar);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::ortho2D(double left, double right,
    double bottom, double top) {
	ofxMatrix4x4 m;
	m.makeOrtho2D(left, right, bottom, top);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::frustum(double left, double right,
    double bottom, double top,
    double zNear, double zFar) {
	ofxMatrix4x4 m;
	m.makeFrustum(left, right, bottom, top, zNear, zFar);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::perspective(double fovy, double aspectRatio,
    double zNear, double zFar) {
	ofxMatrix4x4 m;
	m.makePerspective(fovy, aspectRatio, zNear, zFar);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::lookAt(const ofxVec3f& eye, const ofxVec3f& center, const ofxVec3f& up) {
	ofxMatrix4x4 m;
	m.makeLookAt(eye, center, up);
	return m;
}

inline ofxVec3f ofxMatrix4x4::postMult( const ofxVec3f& v ) const {
	float d = 1.0f / (_mat[3][0] * v.x + _mat[3][1] * v.y + _mat[3][2] * v.z + _mat[3][3]) ;
	return ofxVec3f( (_mat[0][0]*v.x + _mat[0][1]*v.y + _mat[0][2]*v.z + _mat[0][3])*d,
	                 (_mat[1][0]*v.x + _mat[1][1]*v.y + _mat[1][2]*v.z + _mat[1][3])*d,
	                 (_mat[2][0]*v.x + _mat[2][1]*v.y + _mat[2][2]*v.z + _mat[2][3])*d) ;
}

inline ofxVec3f ofxMatrix4x4::preMult( const ofxVec3f& v ) const {
	float d = 1.0f / (_mat[0][3] * v.x + _mat[1][3] * v.y + _mat[2][3] * v.z + _mat[3][3]) ;
	return ofxVec3f( (_mat[0][0]*v.x + _mat[1][0]*v.y + _mat[2][0]*v.z + _mat[3][0])*d,
	                 (_mat[0][1]*v.x + _mat[1][1]*v.y + _mat[2][1]*v.z + _mat[3][1])*d,
	                 (_mat[0][2]*v.x + _mat[1][2]*v.y + _mat[2][2]*v.z + _mat[3][2])*d);
}

inline ofxVec4f ofxMatrix4x4::postMult( const ofxVec4f& v ) const {
	return ofxVec4f( (_mat[0][0]*v.x + _mat[0][1]*v.y + _mat[0][2]*v.z + _mat[0][3]*v.w),
	                 (_mat[1][0]*v.x + _mat[1][1]*v.y + _mat[1][2]*v.z + _mat[1][3]*v.w),
	                 (_mat[2][0]*v.x + _mat[2][1]*v.y + _mat[2][2]*v.z + _mat[2][3]*v.w),
	                 (_mat[3][0]*v.x + _mat[3][1]*v.y + _mat[3][2]*v.z + _mat[3][3]*v.w)) ;
}

inline ofxVec4f ofxMatrix4x4::preMult( const ofxVec4f& v ) const {
	return ofxVec4f( (_mat[0][0]*v.x + _mat[1][0]*v.y + _mat[2][0]*v.z + _mat[3][0]*v.w),
	                 (_mat[0][1]*v.x + _mat[1][1]*v.y + _mat[2][1]*v.z + _mat[3][1]*v.w),
	                 (_mat[0][2]*v.x + _mat[1][2]*v.y + _mat[2][2]*v.z + _mat[3][2]*v.w),
	                 (_mat[0][3]*v.x + _mat[1][3]*v.y + _mat[2][3]*v.z + _mat[3][3]*v.w));
}
inline ofxVec3f ofxMatrix4x4::transform3x3(const ofxVec3f& v, const ofxMatrix4x4& m) {
	return ofxVec3f( (m._mat[0][0]*v.x + m._mat[1][0]*v.y + m._mat[2][0]*v.z),
	                 (m._mat[0][1]*v.x + m._mat[1][1]*v.y + m._mat[2][1]*v.z),
	                 (m._mat[0][2]*v.x + m._mat[1][2]*v.y + m._mat[2][2]*v.z));
}

inline ofxVec3f ofxMatrix4x4::transform3x3(const ofxMatrix4x4& m, const ofxVec3f& v) {
	return ofxVec3f( (m._mat[0][0]*v.x + m._mat[0][1]*v.y + m._mat[0][2]*v.z),
	                 (m._mat[1][0]*v.x + m._mat[1][1]*v.y + m._mat[1][2]*v.z),
	                 (m._mat[2][0]*v.x + m._mat[2][1]*v.y + m._mat[2][2]*v.z) ) ;
}

inline void ofxMatrix4x4::preMultTranslate( const ofxVec3f& v ) {
	for (unsigned i = 0; i < 3; ++i) {
		float tmp = v.v[i];
		if (tmp == 0)
			continue;
		_mat[3][0] += tmp * _mat[i][0];
		_mat[3][1] += tmp * _mat[i][1];
		_mat[3][2] += tmp * _mat[i][2];
		_mat[3][3] += tmp * _mat[i][3];
	}
}

inline void ofxMatrix4x4::postMultTranslate( const ofxVec3f& v ) {
	for (unsigned i = 0; i < 3; ++i) {
		float tmp = v.v[i];
		if (tmp == 0)
			continue;
		_mat[0][i] += tmp * _mat[0][3];
		_mat[1][i] += tmp * _mat[1][3];
		_mat[2][i] += tmp * _mat[2][3];
		_mat[3][i] += tmp * _mat[3][3];
	}
}

// AARON METHOD
inline void ofxMatrix4x4::postMultTranslate( float x, float y, float z) {
	if (x != 0) {
		_mat[0][0] += x * _mat[0][3];
		_mat[1][0] += x * _mat[1][3];
		_mat[2][0] += x * _mat[2][3];
		_mat[3][0] += x * _mat[3][3];
	}
	if (y != 0) {
		_mat[0][1] += y * _mat[0][3];
		_mat[1][1] += y * _mat[1][3];
		_mat[2][1] += y * _mat[2][3];
		_mat[3][1] += y * _mat[3][3];
	}
	if (z != 0) {
		_mat[0][2] += z * _mat[0][3];
		_mat[1][2] += z * _mat[1][3];
		_mat[2][2] += z * _mat[2][3];
		_mat[3][2] += z * _mat[3][3];
	}
}

inline void ofxMatrix4x4::preMultScale( const ofxVec3f& v ) {
	_mat[0][0] *= v.v[0];
	_mat[0][1] *= v.v[0];
	_mat[0][2] *= v.v[0];
	_mat[0][3] *= v.v[0];
	_mat[1][0] *= v.v[1];
	_mat[1][1] *= v.v[1];
	_mat[1][2] *= v.v[1];
	_mat[1][3] *= v.v[1];
	_mat[2][0] *= v.v[2];
	_mat[2][1] *= v.v[2];
	_mat[2][2] *= v.v[2];
	_mat[2][3] *= v.v[2];
}

inline void ofxMatrix4x4::postMultScale( const ofxVec3f& v ) {
	_mat[0][0] *= v.v[0];
	_mat[1][0] *= v.v[0];
	_mat[2][0] *= v.v[0];
	_mat[3][0] *= v.v[0];
	_mat[0][1] *= v.v[1];
	_mat[1][1] *= v.v[1];
	_mat[2][1] *= v.v[1];
	_mat[3][1] *= v.v[1];
	_mat[0][2] *= v.v[2];
	_mat[1][2] *= v.v[2];
	_mat[2][2] *= v.v[2];
	_mat[3][2] *= v.v[2];
}

// AARON METHOD
inline void ofxMatrix4x4::postMultScale( float x, float y, float z ) {
	_mat[0][0] *= x;
	_mat[1][0] *= x;
	_mat[2][0] *= x;
	_mat[3][0] *= x;
	_mat[0][1] *= y;
	_mat[1][1] *= y;
	_mat[2][1] *= y;
	_mat[3][1] *= y;
	_mat[0][2] *= z;
	_mat[1][2] *= z;
	_mat[2][2] *= z;
	_mat[3][2] *= z;
}


inline void ofxMatrix4x4::preMultRotate( const ofxQuat& q ) {
	if (q.zeroRotation())
		return;
	ofxMatrix4x4 r;
	r.setRotate(q);
	preMult(r);
}

inline void ofxMatrix4x4::postMultRotate( const ofxQuat& q ) {
	if (q.zeroRotation())
		return;
	ofxMatrix4x4 r;
	r.setRotate(q);
	postMult(r);
}

// AARON METHOD
inline void ofxMatrix4x4::postMultRotate(float angle, float x, float y, float z) {
	ofxMatrix4x4 r;
	r.makeRotate(angle, x, y, z);
	postMult(r);
}

inline ofxVec3f operator* (const ofxVec3f& v, const ofxMatrix4x4& m ) {
	return m.preMult(v);
}
inline ofxVec4f operator* (const ofxVec4f& v, const ofxMatrix4x4& m ) {
	return m.preMult(v);
}

inline ofxVec3f ofxMatrix4x4::operator* (const ofxVec3f& v) const {
	return postMult(v);
}
inline ofxVec4f ofxMatrix4x4::operator* (const ofxVec4f& v) const {
	return postMult(v);
}

#endif
