
/*
 *  ofxMatrix4x4.h
 *  
 *  Created by Aaron Meyers on 6/22/09 -- modified by Arturo Castro and Zach Lieberman
 *  based on code from OSG - 
 *  see OSG license for more details: 
 *  http://www.openscenegraph.org/projects/osg/wiki/Legal
 * 
 */


#ifndef _OFX_MATRIX_4X4
#define _OFX_MATRIX_4X4 1

#include "ofxVec3f.h"
#include "ofxVec4f.h"
#include "ofxQuaternion.h"
#include <cmath>

#if (_MSC_VER)       
		// make microsoft visual studio complain less about double / float conversion and
		// truncation
		#pragma warning(disable : 4244)
		#pragma warning(disable : 4305)
#endif


class ofxMatrix4x4;

class ofxMatrix4x4 {
public:
	float _mat[4][4];

	//---------------------------------------------
	// constructors
	ofxMatrix4x4() {
		makeIdentityMatrix();
	}
	ofxMatrix4x4( const ofxMatrix4x4& mat) {
		set(mat.getPtr());
	}
	ofxMatrix4x4( float const * const ptr ) {
		set(ptr);
	}
	ofxMatrix4x4( const ofxQuaternion& quat ) {
		makeRotationMatrix(quat);
	}
	ofxMatrix4x4(	float a00, float a01, float a02, float a03,
	              float a10, float a11, float a12, float a13,
	              float a20, float a21, float a22, float a23,
	              float a30, float a31, float a32, float a33);

	//---------------------------------------------
	// destructor
	~ofxMatrix4x4() {}

//	int compare(const ofxMatrix4x4& m) const;
//
//	bool operator < (const ofxMatrix4x4& m) const { return compare(m)<0; }
//	bool operator == (const ofxMatrix4x4& m) const { return compare(m)==0; }
//	bool operator != (const ofxMatrix4x4& m) const { return compare(m)!=0; }

	//---------------------------------------------
	// write data with matrix(row,col)=number
	float& operator()(int row, int col) {
		return _mat[row][col];
	}

	//---------------------------------------------
	// read data with var=matrix(row,col)
	float operator()(int row, int col) const {
		return _mat[row][col];
	}

	//---------------------------------------------
	// check if the matrix is valid
	bool isValid() const {
		return !isNaN();
	}

	bool isNaN() const;

	//---------------------------------------------
	// copy a matrix using = operator
	ofxMatrix4x4& operator = (const ofxMatrix4x4& rhs);

	//---------------------------------------------
	// methods to set the data of the matrix
	void set(const ofxMatrix4x4& rhs);
	void set(float const * const ptr);
	void set(double const * const ptr);
	void set(float a00, float a01, float a02, float a03,
	         float a10, float a11, float a12, float a13,
	         float a20, float a21, float a22, float a23,
	         float a30, float a31, float a32, float a33);

	//---------------------------------------------
	// access the internal data in float* format
	// useful for opengl matrix transformations
	float * getPtr() {
		return (float*)_mat;
	}
	const float * getPtr() const {
		return (const float *)_mat;
	}

	//---------------------------------------------
	// check matrix identity
	bool isIdentity() const;


	//---------------------------------------------
	// init matrix as identity, scale, translation...
	// all make* methods delete the current data
	void makeIdentityMatrix();

	void makeScaleMatrix( const ofxVec3f& );
	void makeScaleMatrix( float, float, float );

	void makeTranslationMatrix( const ofxVec3f& );
	void makeTranslationMatrix( float, float, float );

	void makeRotationMatrix( const ofxVec3f& from, const ofxVec3f& to );
	void makeRotationMatrix( float angle, const ofxVec3f& axis );
	void makeRotationMatrix( float angle, float x, float y, float z );
	void makeRotationMatrix( const ofxQuaternion& );
	void makeRotationMatrix( float angle1, const ofxVec3f& axis1,
	                 float angle2, const ofxVec3f& axis2,
	                 float angle3, const ofxVec3f& axis3);


	// init related to another matrix
	bool makeInvertOf( const ofxMatrix4x4& rhs);
	void makeOrthoNormalOf(const ofxMatrix4x4& rhs);
	void makeFromMultiplicationOf( const ofxMatrix4x4&, const ofxMatrix4x4& );


	//---------------------------------------------
	// init as opengl related matrix for perspective settings
	// see opengl docs of the related funciton for further details

	// glOrtho
	void makeOrthoMatrix(double left,   double right,
	               double bottom, double top,
	               double zNear,  double zFar);

	// glOrtho2D
	void makeOrtho2DMatrix(double left,   double right,
	                        double bottom, double top);

	// glFrustum
	void makeFrustumMatrix(double left,   double right,
	                 double bottom, double top,
	                 double zNear,  double zFar);

	// gluPerspective
	// Aspect ratio is defined as width/height.
	void makePerspectiveMatrix(double fovy,  double aspectRatio,
						 double zNear, double zFar);

	// gluLookAt.
	void makeLookAtMatrix(const ofxVec3f& eye, const ofxVec3f& center, const ofxVec3f& up);


	//---------------------------------------------
	// Get the perspective components from a matrix
	// this only works with pure perspective projection matrices

	bool getOrtho(double& left,   double& right,
	              double& bottom, double& top,
	              double& zNear,  double& zFar) const;

	bool getFrustum(double& left,   double& right,
	                double& bottom, double& top,
	                double& zNear,  double& zFar) const;

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

	// will only work for modelview matrices
	void getLookAt(ofxVec3f& eye, ofxVec3f& center, ofxVec3f& up,
	               float lookDistance = 1.0f) const;



	//---------------------------------------------
	// decompose the matrix into translation, rotation,
	// scale and scale orientation.
	void decompose( ofxVec3f& translation,
					ofxQuaternion& rotation,
					ofxVec3f& scale,
					ofxQuaternion& so ) const;


	//---------------------------------------------
	// basic utility functions to create new matrices
	inline static ofxMatrix4x4 newIdentityMatrix( void );
	inline static ofxMatrix4x4 newScaleMatrix( const ofxVec3f& sv);
	inline static ofxMatrix4x4 newScaleMatrix( float sx, float sy, float sz);
	inline static ofxMatrix4x4 newTranslationMatrix( const ofxVec3f& dv);
	inline static ofxMatrix4x4 newTranslationMatrix( float x, float y, float z);
	inline static ofxMatrix4x4 newRotationMatrix( const ofxVec3f& from, const ofxVec3f& to);
	inline static ofxMatrix4x4 newRotationMatrix( float angle, float x, float y, float z);
	inline static ofxMatrix4x4 newRotationMatrix( float angle, const ofxVec3f& axis);
	inline static ofxMatrix4x4 newRotationMatrix( float angle1, const ofxVec3f& axis1,
	                                   float angle2, const ofxVec3f& axis2,
	                                   float angle3, const ofxVec3f& axis3);
	inline static ofxMatrix4x4 newRotationMatrix( const ofxQuaternion& quat);


	// create new matrices as transformation of another
	inline static ofxMatrix4x4 getInverseOf( const ofxMatrix4x4& matrix);
	inline static ofxMatrix4x4 getTransposedOf( const ofxMatrix4x4& matrix);
	inline static ofxMatrix4x4 getOrthoNormalOf(const ofxMatrix4x4& matrix);


	// create new matrices related to glFunctions

	// glOrtho
	inline static ofxMatrix4x4 newOrthoMatrix(double left,   double right,
	                                 double bottom, double top,
	                                 double zNear,  double zFar);

	// glOrtho2D
	inline static ofxMatrix4x4 newOrtho2DMatrix(double left,   double right,
	                                   double bottom, double top);

	// glFrustum
	inline static ofxMatrix4x4 newFrustumMatrix(double left,   double right,
	                                   double bottom, double top,
	                                   double zNear,  double zFar);

	// gluPerspective
	inline static ofxMatrix4x4 newPerspectiveMatrix(double fovy,  double aspectRatio,
	                                       double zNear, double zFar);

	// gluLookAt
	inline static ofxMatrix4x4 newLookAtMatrix(const ofxVec3f& eye,
	                                  const ofxVec3f& center,
	                                  const ofxVec3f& up);


	//---------------------------------------------
	// matrix - vector multiplication
	// although opengl uses premultiplication
	// because of the way matrices are used in opengl:
	//
	// ofxMatrix4x4				openGL
	// [0]  [1]  [2]  [3]		[0] [4] [8]  [12]
	// [4]  [5]  [6]  [7]		[1] [5] [9]  [13]
	// [8]  [9]  [10] [11]		[2] [6] [10] [14]
	// [12] [13] [14] [15]		[3] [7] [11] [15]
	//
	// in memory though both are layed in the same way
	// so when uploading a matrix it just works without
	// needing to transpose
	//
	// so although opengl docs explain transformations
	// like:
	//
	// ofxVec3f c = rotateZ 30º ofxVec3f a around ofxVec3f b
	//
	// openGL docs says: c = T(b)*R(30)*a;
	//
	// with ofxMatrix4x4:
	// ofxMatrix4x4 R = ofxMatrix4x4::newRotationMatrix(30,0,0,1);
	// ofxMatrix4x4 T = ofxMatrix4x4::newTranlationMatrix(b);
	// ofxVec3f c = a*R*T;
	// where * is calling postMult

	inline ofxVec3f postMult( const ofxVec3f& v ) const;
	inline ofxVec3f operator* (const ofxVec3f& v) const {
		return postMult(v);
	}

	inline ofxVec4f postMult( const ofxVec4f& v ) const;
	inline ofxVec4f operator* (const ofxVec4f& v) const {
		return postMult(v);
	}

	inline ofxVec3f preMult( const ofxVec3f& v ) const;
	inline ofxVec4f preMult( const ofxVec4f& v ) const;


	//---------------------------------------------
	// set methods: all these alter the components
	// deleting the previous data only in that components
	void setRotate(const ofxQuaternion& q);
	void setTranslation( float tx, float ty, float tz );
	void setTranslation( const ofxVec3f& v );

	//---------------------------------------------
	// all these apply the transformations over the
	// current one, it's actually postMult... and behaves
	// the opposite to the equivalent gl functions
	// glTranslate + glRotate == rotate + translate
	void rotate(float angle, float x, float y, float z);
	void rotateRad(float angle, float x, float y, float z);
	void rotate(const ofxQuaternion& q);
	void translate( float tx, float ty, float tz );
	void translate( const ofxVec3f& v );
	void scale(float x, float y, float z);
	void scale( const ofxVec3f& v );

	//---------------------------------------------
	// all these apply the transformations over the
	// current one, it's actually preMult... and behaves
	// the the same the equivalent gl functions
	void glRotate(float angle, float x, float y, float z);
	void glRotateRad(float angle, float x, float y, float z);
	void glRotate(const ofxQuaternion& q);
	void glTranslate( float tx, float ty, float tz );
	void glTranslate( const ofxVec3f& v );
	void glScale(float x, float y, float z);
	void glScale( const ofxVec3f& v );

	//---------------------------------------------
	// get methods: return matrix components
	// rotation and scale can only be used if the matrix
	// only has rotation or scale.
	// for matrices with both use decompose instead.
	ofxQuaternion getRotate() const;
	ofxVec3f getTranslation() const;
	ofxVec3f getScale() const;


	//---------------------------------------------
	// apply a 3x3 transform of v*M[0..2,0..2].
	inline static ofxVec3f transform3x3(const ofxVec3f& v, const ofxMatrix4x4& m);

	// apply a 3x3 transform of M[0..2,0..2]*v.
	inline static ofxVec3f transform3x3(const ofxMatrix4x4& m, const ofxVec3f& v);


	//---------------------------------------------
	// basic Matrixf multiplication, our workhorse methods.
	void postMult( const ofxMatrix4x4& );
	inline void operator *= ( const ofxMatrix4x4& other ) {
		if ( this == &other ) {
			ofxMatrix4x4 temp(other);
			postMult( temp );
		} else postMult( other );
	}

	inline ofxMatrix4x4 operator * ( const ofxMatrix4x4 &m ) const {
		ofxMatrix4x4 r;
		r.makeFromMultiplicationOf(*this, m);
		return  r;
	}

	void preMult( const ofxMatrix4x4& );



	//---------------------------------------------
	// specialized postMult methods, usually you want to use this
	// for transforming ofxVec not preMult
	// equivalent to postMult(newTranslationMatrix(v)); */
	inline void postMultTranslate( const ofxVec3f& v );
	// equivalent to postMult(scale(v));
	inline void postMultScale( const ofxVec3f& v );
	// equivalent to postMult(newRotationMatrix(q));
	inline void postMultRotate( const ofxQuaternion& q );

	// AARON METHODS
	inline void postMultTranslate(float x, float y, float z);
	inline void postMultRotate(float angle, float x, float y, float z);
	inline void postMultScale(float x, float y, float z);


	//---------------------------------------------
	// equivalent to preMult(newScaleMatrix(v));
	inline void preMultScale( const ofxVec3f& v );
	// equivalent to preMult(newTranslationMatrix(v));
	inline void preMultTranslate( const ofxVec3f& v );
	// equivalent to preMult(newRotationMatrix(q));
	inline void preMultRotate( const ofxQuaternion& q );



};


//--------------------------------------------------
// implementation of inline methods

inline bool ofxMatrix4x4::isNaN() const {
	
#if (_MSC_VER)
#ifndef isnan
#define isnan(a) ((a) != (a))
#endif

return isnan(_mat[0][0]) || isnan(_mat[0][1]) || isnan(_mat[0][2]) || isnan(_mat[0][3]) ||
	       isnan(_mat[1][0]) || isnan(_mat[1][1]) || isnan(_mat[1][2]) || isnan(_mat[1][3]) ||
	       isnan(_mat[2][0]) || isnan(_mat[2][1]) || isnan(_mat[2][2]) || isnan(_mat[2][3]) ||
	       isnan(_mat[3][0]) || isnan(_mat[3][1]) || isnan(_mat[3][2]) || isnan(_mat[3][3]);

#else
return std::isnan(_mat[0][0]) || std::isnan(_mat[0][1]) || std::isnan(_mat[0][2]) || std::isnan(_mat[0][3]) ||
	       std::isnan(_mat[1][0]) || std::isnan(_mat[1][1]) || std::isnan(_mat[1][2]) || std::isnan(_mat[1][3]) ||
	       std::isnan(_mat[2][0]) || std::isnan(_mat[2][1]) || std::isnan(_mat[2][2]) || std::isnan(_mat[2][3]) ||
	       std::isnan(_mat[3][0]) || std::isnan(_mat[3][1]) || std::isnan(_mat[3][2]) || std::isnan(_mat[3][3]);

#endif
	
}

inline ofxMatrix4x4& ofxMatrix4x4::operator = (const ofxMatrix4x4& rhs) {
	if ( &rhs == this ) return *this;
	set(rhs.getPtr());
	return *this;
}

inline void ofxMatrix4x4::set(const ofxMatrix4x4& rhs) {
	set(rhs.getPtr());
}

inline void ofxMatrix4x4::set(float const * const ptr) {
	float* local_ptr = (float*)_mat;
	for (int i = 0;i < 16;++i) local_ptr[i] = (float)ptr[i];
}

inline void ofxMatrix4x4::set(double const * const ptr) {
	float* local_ptr = (float*)_mat;
	for (int i = 0;i < 16;++i) local_ptr[i] = (float)ptr[i];
}

inline bool ofxMatrix4x4::isIdentity() const {
	return _mat[0][0] == 1.0f && _mat[0][1] == 0.0f && _mat[0][2] == 0.0f &&  _mat[0][3] == 0.0f &&
		   _mat[1][0] == 0.0f && _mat[1][1] == 1.0f && _mat[1][2] == 0.0f &&  _mat[1][3] == 0.0f &&
		   _mat[2][0] == 0.0f && _mat[2][1] == 0.0f && _mat[2][2] == 1.0f &&  _mat[2][3] == 0.0f &&
		   _mat[3][0] == 0.0f && _mat[3][1] == 0.0f && _mat[3][2] == 0.0f &&  _mat[3][3] == 1.0f;
}

inline void ofxMatrix4x4::makeOrtho2DMatrix(double left,   double right,
	                        double bottom, double top) {
	makeOrthoMatrix(left, right, bottom, top, -1.0, 1.0);
}

inline ofxVec3f ofxMatrix4x4::getTranslation() const {
	return ofxVec3f(_mat[3][0], _mat[3][1], _mat[3][2]);
}

inline ofxVec3f ofxMatrix4x4::getScale() const {
	ofxVec3f x_vec(_mat[0][0], _mat[1][0], _mat[2][0]);
	ofxVec3f y_vec(_mat[0][1], _mat[1][1], _mat[2][1]);
	ofxVec3f z_vec(_mat[0][2], _mat[1][2], _mat[2][2]);
	return ofxVec3f(x_vec.length(), y_vec.length(), z_vec.length());
}

//static utility methods
inline ofxMatrix4x4 ofxMatrix4x4::newIdentityMatrix(void) {
	ofxMatrix4x4 m;
	m.makeIdentityMatrix();
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::newScaleMatrix(float sx, float sy, float sz) {
	ofxMatrix4x4 m;
	m.makeScaleMatrix(sx, sy, sz);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::newScaleMatrix(const ofxVec3f& v ) {
	return newScaleMatrix(v.x, v.y, v.z );
}

inline ofxMatrix4x4 ofxMatrix4x4::newTranslationMatrix(float tx, float ty, float tz) {
	ofxMatrix4x4 m;
	m.makeTranslationMatrix(tx, ty, tz);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::newTranslationMatrix(const ofxVec3f& v ) {
	return newTranslationMatrix(v.x, v.y, v.z );
}

inline ofxMatrix4x4 ofxMatrix4x4::newRotationMatrix( const ofxQuaternion& q ) {
	return ofxMatrix4x4(q);
}
inline ofxMatrix4x4 ofxMatrix4x4::newRotationMatrix(float angle, float x, float y, float z ) {
	ofxMatrix4x4 m;
	m.makeRotationMatrix(angle, x, y, z);
	return m;
}
inline ofxMatrix4x4 ofxMatrix4x4::newRotationMatrix(float angle, const ofxVec3f& axis ) {
	ofxMatrix4x4 m;
	m.makeRotationMatrix(angle, axis);
	return m;
}
inline ofxMatrix4x4 ofxMatrix4x4::newRotationMatrix(	float angle1, const ofxVec3f& axis1,
    float angle2, const ofxVec3f& axis2,
    float angle3, const ofxVec3f& axis3) {
	ofxMatrix4x4 m;
	m.makeRotationMatrix(angle1, axis1, angle2, axis2, angle3, axis3);
	return m;
}
inline ofxMatrix4x4 ofxMatrix4x4::newRotationMatrix(const ofxVec3f& from, const ofxVec3f& to ) {
	ofxMatrix4x4 m;
	m.makeRotationMatrix(from, to);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::getInverseOf( const ofxMatrix4x4& matrix) {
	ofxMatrix4x4 m;
	m.makeInvertOf(matrix);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::getTransposedOf( const ofxMatrix4x4& matrix) {
	ofxMatrix4x4 m(matrix._mat[0][0], matrix._mat[1][0], matrix._mat[2][0],
	               matrix._mat[3][0], matrix._mat[0][1], matrix._mat[1][1], matrix._mat[2][1],
	               matrix._mat[3][1], matrix._mat[0][2], matrix._mat[1][2], matrix._mat[2][2],
	               matrix._mat[3][2], matrix._mat[0][3], matrix._mat[1][3], matrix._mat[2][3],
	               matrix._mat[3][3]);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::getOrthoNormalOf(const ofxMatrix4x4& matrix) {
	ofxMatrix4x4 m;
	m.makeOrthoNormalOf(matrix);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::newOrthoMatrix(double left, double right,
                                        double bottom, double top,
                                        double zNear, double zFar) {
	ofxMatrix4x4 m;
	m.makeOrthoMatrix(left, right, bottom, top, zNear, zFar);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::newOrtho2DMatrix(double left, double right,
    double bottom, double top) {
	ofxMatrix4x4 m;
	m.makeOrtho2DMatrix(left, right, bottom, top);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::newFrustumMatrix(double left, double right,
    double bottom, double top,
    double zNear, double zFar) {
	ofxMatrix4x4 m;
	m.makeFrustumMatrix(left, right, bottom, top, zNear, zFar);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::newPerspectiveMatrix(double fovy, double aspectRatio,
    double zNear, double zFar) {
	ofxMatrix4x4 m;
	m.makePerspectiveMatrix(fovy, aspectRatio, zNear, zFar);
	return m;
}

inline ofxMatrix4x4 ofxMatrix4x4::newLookAtMatrix(const ofxVec3f& eye, const ofxVec3f& center, const ofxVec3f& up) {
	ofxMatrix4x4 m;
	m.makeLookAtMatrix(eye, center, up);
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

inline void ofxMatrix4x4::rotate(const ofxQuaternion& q){
	postMultRotate(q);
}

inline void ofxMatrix4x4::rotate(float angle, float x, float y, float z){
	postMultRotate(angle*DEG_TO_RAD,x,y,z);
}

inline void ofxMatrix4x4::rotateRad(float angle, float x, float y, float z){
	postMultRotate(angle,x,y,z);
}

inline void ofxMatrix4x4::translate( float tx, float ty, float tz ){
	postMultTranslate(tx,ty,tz);
}

inline void ofxMatrix4x4::translate( const ofxVec3f& v ){
	postMultTranslate(v);
}

inline void ofxMatrix4x4::scale(float x, float y, float z){
	postMultScale(x,y,z);
}

inline void ofxMatrix4x4::scale( const ofxVec3f& v ){
	postMultScale(v);
}

inline void ofxMatrix4x4::glRotate(float angle, float x, float y, float z){
	preMultRotate(ofxQuaternion(angle*DEG_TO_RAD,ofxVec3f(x,y,z)));
}

inline void ofxMatrix4x4::glRotateRad(float angle, float x, float y, float z){
	preMultRotate(ofxQuaternion(angle,ofxVec3f(x,y,z)));
}

inline void ofxMatrix4x4::glRotate(const ofxQuaternion& q){
	preMultRotate(q);
}

inline void ofxMatrix4x4::glTranslate( float tx, float ty, float tz ){
	preMultTranslate(ofxVec3f(tx,ty,tz));
}

inline void ofxMatrix4x4::glTranslate( const ofxVec3f& v ){
	preMultTranslate(v);
}

inline void ofxMatrix4x4::glScale(float x, float y, float z){
	preMultScale(ofxVec3f(x,y,z));
}

inline void ofxMatrix4x4::glScale( const ofxVec3f& v ){
	preMultScale(v);
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


inline void ofxMatrix4x4::preMultRotate( const ofxQuaternion& q ) {
	if (q.zeroRotation())
		return;
	ofxMatrix4x4 r;
	r.setRotate(q);
	preMult(r);
}

inline void ofxMatrix4x4::postMultRotate( const ofxQuaternion& q ) {
	if (q.zeroRotation())
		return;
	ofxMatrix4x4 r;
	r.setRotate(q);
	postMult(r);
}

// AARON METHOD
inline void ofxMatrix4x4::postMultRotate(float angle, float x, float y, float z) {
	ofxMatrix4x4 r;
	r.makeRotationMatrix(angle, x, y, z);
	postMult(r);
}



inline ofxVec3f operator* (const ofxVec3f& v, const ofxMatrix4x4& m ) {
	return m.preMult(v);
}
inline ofxVec4f operator* (const ofxVec4f& v, const ofxMatrix4x4& m ) {
	return m.preMult(v);
}



#endif
