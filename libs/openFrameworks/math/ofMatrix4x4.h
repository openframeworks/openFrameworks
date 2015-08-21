/*
 *  ofMatrix4x4.h
 *  
 *  Created by Aaron Meyers on 6/22/09 -- modified by Arturo Castro, Zach Lieberman, Memo Akten
 *  based on code from OSG - 
 *  see OSG license for more details: 
 *  http://www.openscenegraph.org/projects/osg/wiki/Legal
 * 
 */

#pragma once

#include "ofVec3f.h"
#include "ofVec4f.h"
#include "ofQuaternion.h"
#include "ofConstants.h"
#include <cmath>

/// \brief The ofMatrix4x4 is the big class of the math part of openFrameworks.
/// 
/// You'll sometimes see it used for doing things like setting where the camera
/// in OpenGL (the mathematically calculated one, not the ofCamera one) is 
/// looking or is pointed, or figuring how to position something in 3d space,
/// doing scaling, etc. The great thing about the 4x4 matrix is that it can do 
/// all these things at the same time. A single ofMatrix4x4 can represent a ton 
/// of different information about a stuff that goes on in doing 3d 
/// programming: where an object is, how you want to scale an object, where a 
/// camera is. Let's look at a few really basic examples:
/// 
/// ![MATS](math/mats.png)
/// 
/// Not particularly exciting, but you can see how they'd be useful. Luckily most
/// of the need to transform, rotate, scale, shear, or further bazzlemunge (just
/// kidding, bazzlemunging is not a thing) stuff in oF is handled internally by
/// objects like ofNode or ofCamera.
/// 
/// oF uses row-vector style by default, meaning that when transforming a vector
/// by multiplying with a matrix, you should put the vector on the left side and
/// the matrix (or matrices) to its right. When multiplying by multiple matrices,
/// the order of application of the transforms is left-to-right. This means that
/// the standard order of manipulation operations is 
/// vector * scale * rotate * translate.
/// 
/// Note that in GLSL, this convention is reversed, and column-vector style is
/// used. oF uploads the matrices to the GL context correctly, but you should
/// reverse the order of your vertex manipulations to right-to-left style, e.g.
/// translate * rotate * scale * vector.
/// 
/// On the application side, oF has operators which let you do matrix-vector 
/// multiplication with the vector on the right if that's your preferred style. 
/// To set up a combined transformation matrix for working in this style, you
/// should do matrix transformations with the functions like glTranslate,
/// glRotate, and glScale.
/// 
class ofMatrix4x4 {
public:
//	float _mat[4][4];
	
	/// \brief The values of the matrix, stored in row-major order.
	ofVec4f _mat[4];
	/// \cond INTERNAL
	// Should this be moved to private?
	/// \endcond

	/// \name Constructors
	/// \{

	/// \brief The default constructor provides an identity matrix.
	ofMatrix4x4() {
		makeIdentityMatrix();
	}

	/// \brief You can pass another ofMatrix4x4 to create a copy.
	ofMatrix4x4( const ofMatrix4x4& mat) {
		set(mat.getPtr());
	}

	/// \brief Construct with a pointer.
	/// 
	/// You can pass a pointer to floats, and the first 16 contents will be 
	/// extracted into this matrix.
	/// 
	/// \warning the validity of these values is not checked!
	ofMatrix4x4( float const * const ptr ) {
		set(ptr);
	}

	/// \brief Rotation matrices can be constructed from a quaternion.
	ofMatrix4x4( const ofQuaternion& quat ) {
		makeRotationMatrix(quat);
	}

	/// \brief Positional style.
	/// 
	/// All 16 values of the matrix as positional arguments in row-major order.
	ofMatrix4x4(	float a00, float a01, float a02, float a03,
	              float a10, float a11, float a12, float a13,
	              float a20, float a21, float a22, float a23,
	              float a30, float a31, float a32, float a33);

	/// \} end Constructor group

	/// \brief destructor.
	~ofMatrix4x4() {}

	/// \name make* Methods
	/// \{
	/// These methods perform a total matrix makeover. All make* methods delete
	/// the current data and the matrix becomes a new type of transform.
	
	/// \brief Matrix becomes the identity matrix.
	void makeIdentityMatrix();

	/// \name Scale
	/// \{
	/// Matrix becomes a scale transform.
	/// 
	/// Accepts x, y, z scale values as a vector or separately.
	void makeScaleMatrix( const ofVec3f& );
	void makeScaleMatrix( float, float, float );
	/// \}

	/// \name Translation
	/// \{
	/// Matrix becomes a translation transform.
	/// 
	/// Accepts x, y, z translation values as a vector or separately.
	void makeTranslationMatrix( const ofVec3f& );
	void makeTranslationMatrix( float, float, float );
	/// \}

	/// \name Rotation
	/// \{
	/// Matrix becomes a rotation transform.
	/// 
	/// \param from Matrix becomes a rotation from this vector direction.
	/// \param to Matrix becomes a rotation to this vector direction.
	void makeRotationMatrix( const ofVec3f& from, const ofVec3f& to );
	/// \param angle Matrix becomes a rotation by angle (degrees).
	/// \param axis Rotation is performed around this vector.
	void makeRotationMatrix( float angle, const ofVec3f& axis );
	/// \param angle Matrix becomes a rotation by angle (degrees).
	/// \param x X-value of the rotation axis.
	/// \param y Y-value of the rotation axis.
	/// \param z Z-value of the rotation axis.
	void makeRotationMatrix( float angle, float x, float y, float z );
	/// \param quaternion Matrix becomes a rotation that produces the quaternion's orientation.
	void makeRotationMatrix( const ofQuaternion& quaternion );
	/// \brief Matrix becomes a rotation around multiple axes.
	/// 
	/// The final rotation is the result of rotating around each of the three
	/// axes, in order. Angles are given in degrees, and axes can be arbitrary
	/// vectors.
	void makeRotationMatrix( float angle1, const ofVec3f& axis1,
	                 float angle2, const ofVec3f& axis2,
	                 float angle3, const ofVec3f& axis3);
	/// \}


	/// \name Make* methods related to another matrix
	/// \{

	/// \brief Matrix becomes the inverse of the provided matrix.
	bool makeInvertOf( const ofMatrix4x4& rhs);

	/// \brief Matrix becomes an orthonormalized version of the provided matrix.
	/// 
	/// The basis vectors (the 3x3 chunk embedded in the upper left of the matrix)
	/// are normalized. This means the resulting matrix has had scaling effects
	/// removed. The fourth column and the fourth row are transferred over
	/// untouched, so translation will be included as well.
	void makeOrthoNormalOf(const ofMatrix4x4& rhs);

	/// \brief Matrix becomes the result of the multiplication of two other matrices.
	void makeFromMultiplicationOf( const ofMatrix4x4&, const ofMatrix4x4& );

	/// \}

	/// \name View matrix make*
	/// \{
	/// These functions are based on some OpenGL matrix functions used for
	/// perspective settings. See the OpenGL docs of the related function
	/// for further details.

	/// \brief Matrix becomes an orthographic projection matrix.
	/// 
	/// Related to: glOrtho. The orthographic projection has a box-shaped
	/// viewing volume described by the six parameters. Left, right, bottom,
	/// and top specify coordinates in the zNear clipping plane where the
	/// corresponding box sides intersect it.
	void makeOrthoMatrix(double left,   double right,
	               double bottom, double top,
	               double zNear,  double zFar);

	/// \brief Matrix becomes a 2D orthographic projection matrix.
	/// 
	/// Related to: glOrtho2D. The box-shaped viewing volume is 
	/// described by the four parameters and, implicitly, a zNear of -1 
	/// and a zFar of 1.
	void makeOrtho2DMatrix(double left,   double right,
	                        double bottom, double top);

	/// \brief Matrix becomes a perspective projection matrix.
	/// 
	/// Related to: glFrustum. The viewing volume is frustum-shaped and
	/// defined by the six parameters. Left, right, top, and bottom specify 
	/// coordinates in the zNear clipping plane where the frustum edges intersect
	/// it, and the zNear and zFar parameters define the forward distances of 
	/// the view volume. The resulting volume can be vertically and 
	/// horizontally asymmetrical around the center of the near plane.
	void makeFrustumMatrix(double left,   double right,
	                 double bottom, double top,
	                 double zNear,  double zFar);

	/// \brief Matrix becomes a perspective projection matrix.
	/// 
	/// Related to: gluPerspective. The viewing volume is frustum-shaped amd
	/// defined by the four parameters. The fovy and aspect ratio
	/// are used to compute the positions of the left, right, top, and bottom sides
	/// of the viewing volume in the zNear plane. The fovy is the y field-of-view,
	/// the angle made by the top and bottom sides of frustum if they were to
	/// intersect. The aspect ratio is the width of the frustum divided by its
	/// height. Note that the resulting volume is both vertically and 
	/// horizontally symmetrical around the center of the near plane.
	void makePerspectiveMatrix(double fovy,  double aspectRatio,
						 double zNear, double zFar);

	/// \brief Matrix becomes a combination of translation and rotation.
	/// 
	/// Matrix becomes a combination of a translation to the position of 'eye'
	/// and a rotation matrix which orients an object to point towards 'center'
	/// along its z-axis. Use this function if you want an object to look at a 
	/// point from another point in space.
	/// 
	/// \param eye The position of the object.
	/// \param center The point which the object is "looking" at.
	/// \param up The direction which the object considers to be "up".
	void makeLookAtMatrix(const ofVec3f& eye, const ofVec3f& center, const ofVec3f& up);

	/// \brief Matrix becomes a combination of an inverse translation and rotation.
	/// 
	/// Related to: gluLookAt. This creates the inverse of makeLookAtMatrix.
	/// The matrix will be an opposite translation from the 'eye' position,
	/// and it will rotate things in the opposite direction of the eye-to-center
	/// orientation. This is definitely confusing, but the main reason to use
	/// this transform is to set up a view matrix for a camera that's looking 
	/// at a certain point. To achieve the effect of moving the camera somewhere
	/// and rotating it so that it points at something, the rest of the world
	/// is moved in the *opposite* direction and rotated in the *opposite* way 
	/// around the camera. This way, you get the same effect as moving the actual
	/// camera, but all the projection math can still be done with the camera
	/// positioned at the origin (which makes it way simpler).
	void makeLookAtViewMatrix(const ofVec3f& eye, const ofVec3f& center, const ofVec3f& up);

	/// \} end view-related group

	/// \} end make* group

	/// \name Static new* matrix functions
	/// \{
	/// 
	/// These are static utility functions to create new matrices. These
	/// functions generally return the equivalent of declaring a matrix and
	/// calling the corresponding "make..." function on it.

	/// \sa makeIdentityMatrix
	inline static ofMatrix4x4 newIdentityMatrix( void );

	/// \sa makeScaleMatrix
	inline static ofMatrix4x4 newScaleMatrix( const ofVec3f& sv);
	inline static ofMatrix4x4 newScaleMatrix( float sx, float sy, float sz);

	/// \sa makeTranslationMatrix
	inline static ofMatrix4x4 newTranslationMatrix( const ofVec3f& dv);
	inline static ofMatrix4x4 newTranslationMatrix( float x, float y, float z);

	/// \sa makeRotationMatrix
	inline static ofMatrix4x4 newRotationMatrix( const ofVec3f& from, const ofVec3f& to);
	inline static ofMatrix4x4 newRotationMatrix( float angle, float x, float y, float z);
	inline static ofMatrix4x4 newRotationMatrix( float angle, const ofVec3f& axis);
	inline static ofMatrix4x4 newRotationMatrix( float angle1, const ofVec3f& axis1,
	                                   float angle2, const ofVec3f& axis2,
	                                   float angle3, const ofVec3f& axis3);
	inline static ofMatrix4x4 newRotationMatrix( const ofQuaternion& quat);

	/// These functions create new matrices related to glFunctions. See 
	/// the description of the corresponding make* methods for more info.

	/// \sa makeOrthoMatrix
	inline static ofMatrix4x4 newOrthoMatrix(double left,   double right,
	                                 double bottom, double top,
	                                 double zNear,  double zFar);

	/// \sa makeOrtho2DMatrix
	inline static ofMatrix4x4 newOrtho2DMatrix(double left,   double right,
	                                   double bottom, double top);

	/// \sa makeFrustumMatrix
	inline static ofMatrix4x4 newFrustumMatrix(double left,   double right,
	                                   double bottom, double top,
	                                   double zNear,  double zFar);

	/// \sa makePerspectiveMatrix
	inline static ofMatrix4x4 newPerspectiveMatrix(double fovy,  double aspectRatio,
	                                       double zNear, double zFar);

	/// \sa makeLookAtMatrix
	inline static ofMatrix4x4 newLookAtMatrix(const ofVec3f& eye,
	                                  const ofVec3f& center,
	                                  const ofVec3f& up);

	/// \} end Static new* matrix group

	//---------------------
	/// \name Accessors
	/// \{

	/// \brief Write data with `matrix(row,col)=number`
	float& operator()(std::size_t row, std::size_t col) {
		return _mat[row][col];
	}

	/// \brief Read data with `matrix(row, col)`
	float operator()(std::size_t row, std::size_t col) const {
		return _mat[row][col];
	}

	/// \brief returns a copy of row i
	ofVec3f getRowAsVec3f(std::size_t i) const {
		return ofVec3f(_mat[i][0], _mat[i][1], _mat[i][2]);
	}
	
	/// \brief returns a copy of row i
	ofVec4f getRowAsVec4f(std::size_t i) const {
		return _mat[i];
	}
	
	/// \cond INTERNAL
	friend ostream& operator<<(ostream& os, const ofMatrix4x4& M);
	friend istream& operator>>(istream& is, ofMatrix4x4& M);
	/// \endcond
	
	/// \brief Access the internal data in `float*` format
	/// useful for opengl matrix transformations
	float * getPtr() {
		return (float*)_mat;
	}
	const float * getPtr() const {
		return (const float *)_mat;
	}
	
	/// \} end accessors group

	//---------------------
	/// \name Checking
	/// \{
	
	/// \brief Checks if the matrix is valid by ensuring its items are numbers.
	bool isValid() const {
		return !isNaN();
	}

	/// \brief Checks if the matrix contains items that are not numbers.
	bool isNaN() const;

	/// \brief Checks if the matrix is the identity matrix.
	bool isIdentity() const;

	/// \}

	//---------------------
	/// \name Setters
	/// \{

	/// \brief Copy a matrix using `=` operator.
	ofMatrix4x4& operator = (const ofMatrix4x4& rhs);

	
	/// \brief Set the data of the matrix.
	/// 
	/// These functions are analogous to the corresponding constructors.
	void set(const ofMatrix4x4& rhs);
	void set(float const * const ptr);
	void set(double const * const ptr);
	void set(float a00, float a01, float a02, float a03,
	         float a10, float a11, float a12, float a13,
	         float a20, float a21, float a22, float a23,
	         float a30, float a31, float a32, float a33);

	/// \}

	//---------------------
	/// \name Getters
	/// \{

	/// \brief Gets the inverse matrix.
	ofMatrix4x4 getInverse() const;

	/// \brief Get the perspective components from a matrix.
	/// 
	/// This only works with pure perspective projection matrices.
	bool getOrtho(double& left,   double& right,
	              double& bottom, double& top,
	              double& zNear,  double& zFar) const;

	/// \brief Gets the perspective components for a frustum projection matrix.
	bool getFrustum(double& left,   double& right,
	                double& bottom, double& top,
	                double& zNear,  double& zFar) const;

	/// \brief Get the frustum settings of a symmetric perspective projection
	/// matrix.
	/// 
	/// Note, if matrix is not a symmetric perspective matrix then the
	/// shear will be lost.
	/// Asymmetric matrices occur when stereo, power walls, caves and
	/// reality center display are used.
	/// In these configuration one should use the getFrustum method instead.
	///
	/// \returns false if matrix is not a perspective matrix,
	/// where parameter values are undefined.
	bool getPerspective(double& fovy,  double& aspectRatio,
	                    double& zNear, double& zFar) const;

	/// \brief Gets the lookAt determiners of the matrix.
	/// 
	/// This function will only work for modelview matrices.
	void getLookAt(ofVec3f& eye, ofVec3f& center, ofVec3f& up,
	               float lookDistance = 1.0f) const;

	/// \brief Decompose the matrix into translation, rotation,
	/// scale and scale orientation.
	void decompose( ofVec3f& translation,
					ofQuaternion& rotation,
					ofVec3f& scale,
					ofQuaternion& so ) const;

	/// Create new matrices as transformation of another.

	/// \brief Makes a new matrix which is the inverse of the given matrix.
	inline static ofMatrix4x4 getInverseOf( const ofMatrix4x4& matrix);
	/// \brief Makes a new matrix which is the transpose of the given matrix.
	inline static ofMatrix4x4 getTransposedOf( const ofMatrix4x4& matrix);
	/// \brief Makes a new matrix which is the given matrix, normalized.
	inline static ofMatrix4x4 getOrthoNormalOf(const ofMatrix4x4& matrix);


	/// \}

	//---------------------
	/// \name Matrix multiplication
	/// \{

	/// Although OpenGL uses post-multiplication (vector-on-the-right) with
	/// column-major matrix memory layout, oF uses pre-multiplication
	/// (vector-on-the-left) with row-major matrix memory layout by default.
	/// 
	/// openGL:
	///
	/// |   |   |   |   |
	/// |:-:|:-:|:-:|:-:|
	/// | 0 | 4 | 8 | 12|
	/// | 1 | 5 | 9 | 13|
	/// | 2 | 6 | 10| 14|
	/// | 3 | 7 | 11| 15|
	/// 
	/// ofMatrix4x4:
	/// 
	/// |   |   |   |   |
	/// |:-:|:-:|:-:|:-:|
	/// | 0 | 1 | 2 | 3 |
	/// | 4 | 5 | 6 | 7 |
	/// | 8 | 9 | 10| 11|
	/// | 12| 13| 14| 15|
	///
	/// However, the two memory layouts are compatible because of a funny trick.
	/// 
	/// When the ofMatrix4x4 is uploaded into OpenGL's memory, OpenGL treats it
	/// like a column-major matrix. The rows of the ofMatrix4x4 are loaded as
	/// columns for the GLSL mat4. The result is that the matrix is transposed.
	/// This seems like a bug, but it's in fact exactly what we want, because to
	/// do the transition from pre-multiplication to post-multiplication style,
	/// we need to perform the very same transpose.
	/// 
	/// By using pre-multiplication, oF treats vectors as 1x4 matrices, since 
	/// that provides a valid 1x4 * 4x4 operation. When moving to
	/// post-multiplication, OpenGL is treating vectors like columns, 
	/// providing a similarly valid 4x4 * 4x1 operation. This means that the
	///  resulting vector in OGL-land is the transpose of the result when
	/// done in oF-land.
	/// 
	/// Recall that in matrix multiplication,
	/// 
	/// (V * M * S)^T = (S^T) * (M^T) * (V^T)
	/// 
	/// What this means is that to convert from pre-multiplication to 
	/// post-multiplication, we need to transpose our matrices (and vectors) 
	/// and reverse the order of multiplication. You're already reversing
	/// the order of multiplication by writing your shaders with the vector
	/// on the right, and the implicit transpose that happens when your matrix
	/// is uploaded to GL memory accomplishes the transposition for free!
	/// 
	/// For more information on this subject, check out
	/// [this post](http://seanmiddleditch.com/matrices-handedness-pre-and-post-multiplication-row-vs-column-major-and-notations/).

	/// \brief Matrix * vector multiplication.
	/// 
	/// This operation implicitly treat vectors as column-matrices.
	inline ofVec3f postMult( const ofVec3f& v ) const;
	inline ofVec4f postMult( const ofVec4f& v ) const;

	/// \brief Post-multiply by another matrix.
	/// 
	/// This matrix becomes `this * other`.
	void postMult( const ofMatrix4x4& );

	/// \brief Vector * matrix multiplication.
	/// 
	/// This operation implicitly treats vectors as row-matrices.
	inline ofVec3f preMult( const ofVec3f& v ) const;
	inline ofVec4f preMult( const ofVec4f& v ) const;

	/// \brief Pre-multiply by another matrix.
	/// 
	/// This matrix becomes `other * this`.
	void preMult( const ofMatrix4x4& );

	/// \brief The *= operation for matrices.
	/// 
	/// This is equivalent to calling postMult(other), but it allows you to do
	/// someMatrix *= someMatrix without breaking const-correctness. Calling
	/// someMatrix.postMult(someMatrix) won't work.
	inline void operator *= ( const ofMatrix4x4& other ) {
		if ( this == &other ) {
			ofMatrix4x4 temp(other);
			postMult( temp );
		} else postMult( other );
	}

	/// \brief creates a new matrix from the product of two matrices.
	inline ofMatrix4x4 operator * ( const ofMatrix4x4 &m ) const {
		ofMatrix4x4 r;
		r.makeFromMultiplicationOf(*this, m);
		return  r;
	}

	/// \brief Matrix * Vector operator.
	/// 
	/// Calls postMult() internally.
	inline ofVec3f operator* (const ofVec3f& v) const {
		return postMult(v);
	}

	inline ofVec4f operator* (const ofVec4f& v) const {
		return postMult(v);
	}

	/// These are specialized postMult methods, usually you want to use these
	/// for transforming with ofVec.

	/// \brief Equivalent to postMult(newTranslationMatrix(v)).
	inline void postMultTranslate( const ofVec3f& v );
	/// \brief Equivalent to postMult(scale(v)).
	inline void postMultScale( const ofVec3f& v );
	/// \brief Equivalent to postMult(newRotationMatrix(q)).
	inline void postMultRotate( const ofQuaternion& q );

	inline void postMultTranslate(float x, float y, float z);
	inline void postMultRotate(float angle, float x, float y, float z);
	inline void postMultScale(float x, float y, float z);

	/// \brief Equivalent to preMult(newScaleMatrix(v)).
	inline void preMultScale( const ofVec3f& v );
	/// \brief Equivalent to preMult(newTranslationMatrix(v)).
	inline void preMultTranslate( const ofVec3f& v );
	/// \brief Equivalent to preMult(newRotationMatrix(q)).
	inline void preMultRotate( const ofQuaternion& q );

	/// \}

	//---------------------
	/// \name Matrix transformation
	/// \{

	/// \name Set methods
	/// \{
	/// 
	/// All of these methods alter the components,
	/// deleting the previous data only in that component.
	void setRotate(const ofQuaternion& q);
	void setTranslation( float tx, float ty, float tz );
	void setTranslation( const ofVec3f& v );
	/// \}

	/// \name PostMult Transformations
	/// \{
	/// 
	/// All of these methods apply the transformations over the current one,
	/// calling postMult under the hood. These work as the opposite of the 
	/// equivalent OpenGL functions.
	/// 
	/// For example, glTranslate + glRotate == rotate + translate.

	/// \brief Rotates by angle (degrees) around the given x, y, z axis.
	void rotate(float angle, float x, float y, float z);
	/// \brief Rotates by angle (radians) around the given x, y, z axis.
	void rotateRad(float angle, float x, float y, float z);
	/// \brief Rotates based on the quarternion.
	void rotate(const ofQuaternion& q);
	/// \brief Translates by tx, ty, tz.
	void translate( float tx, float ty, float tz );
	/// \brief Translates along the vector.
	void translate( const ofVec3f& v );
	/// \brief Scales each axis by the corresponding x, y, z.
	void scale(float x, float y, float z);
	/// \brief Scales each axis by the corresponding x, y, z of the vector.
	void scale( const ofVec3f& v );
	/// \}

	/// \name PreMult Transformations
	/// \{
	/// 
	/// All of these methods apply the transformations over the current one,
	/// calling preMult under the hood. These work the the same the equivalent 
	/// OpenGL functions.

	/// \sa rotate
	void glRotate(float angle, float x, float y, float z);
	/// \sa rotate
	void glRotateRad(float angle, float x, float y, float z);
	/// \sa rotate
	void glRotate(const ofQuaternion& q);
	/// \sa translate
	void glTranslate( float tx, float ty, float tz );
	/// \sa translate
	void glTranslate( const ofVec3f& v );
	/// \sa scale
	void glScale(float x, float y, float z);
	/// \sa scale
	void glScale( const ofVec3f& v );
	/// \}

	/// \name Get Methods
	/// \{
	/// 
	/// These return matrix components. getRotate and getScale can only be 
	/// used if the matrix only has rotation or only has scale, since these
	/// transform values are stored in the same area of the matrix.
	/// For matrices with both use decompose instead.
	ofQuaternion getRotate() const;
	ofVec3f getTranslation() const;
	ofVec3f getScale() const;
	/// \}

	/// \brief Apply a 3x3 transform (no translation) of v * M.
	inline static ofVec3f transform3x3(const ofVec3f& v, const ofMatrix4x4& m);

	/// \brief Apply a 3x3 transform (no translation) of M * v.
	inline static ofVec3f transform3x3(const ofMatrix4x4& m, const ofVec3f& v);

	/// \}
	
};

/// \cond INTERNAL

//--------------------------------------------------
// implementation of inline methods

inline bool ofMatrix4x4::isNaN() const {
	
#if (_MSC_VER) || defined (TARGET_ANDROID)
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



inline ostream& operator<<(ostream& os, const ofMatrix4x4& M) {
	int w = 8;
	os	<< setw(w)
		<< M._mat[0][0] << ", " << setw(w)
		<< M._mat[0][1] << ", " << setw(w)
		<< M._mat[0][2] << ", " << setw(w) 
		<< M._mat[0][3] << std::endl;
		
	os	<< setw(w)
		<< M._mat[1][0] << ", " << setw(w) 
		<< M._mat[1][1] << ", " << setw(w)
		<< M._mat[1][2] << ", " << setw(w) 
		<< M._mat[1][3] << std::endl;
	
	os	<< setw(w)
		<< M._mat[2][0] << ", " << setw(w) 
		<< M._mat[2][1] << ", " << setw(w)
		<< M._mat[2][2] << ", " << setw(w) 
		<< M._mat[2][3] << std::endl;
	
	os	<< setw(w)
		<< M._mat[3][0] << ", " << setw(w) 
		<< M._mat[3][1] << ", " << setw(w)
		<< M._mat[3][2] << ", " << setw(w) 
		<< M._mat[3][3];
	
	return os;
}

inline istream& operator>>(istream& is, ofMatrix4x4& M) {
	is >> M._mat[0][0]; is.ignore(2); 
	is >> M._mat[0][1]; is.ignore(2);
	is >> M._mat[0][2]; is.ignore(2);
	is >> M._mat[0][3]; is.ignore(1);
	
	is >> M._mat[1][0]; is.ignore(2); 
	is >> M._mat[1][1]; is.ignore(2);
	is >> M._mat[1][2]; is.ignore(2);
	is >> M._mat[1][3]; is.ignore(1);
	
	is >> M._mat[2][0]; is.ignore(2); 
	is >> M._mat[2][1]; is.ignore(2);
	is >> M._mat[2][2]; is.ignore(2);
	is >> M._mat[2][3]; is.ignore(1);
	
	is >> M._mat[3][0]; is.ignore(2); 
	is >> M._mat[3][1]; is.ignore(2);
	is >> M._mat[3][2]; is.ignore(2);
	is >> M._mat[3][3];
	return is;
}


inline ofMatrix4x4& ofMatrix4x4::operator = (const ofMatrix4x4& rhs) {
	if ( &rhs == this ) return *this;
	set(rhs.getPtr());
	return *this;
}

inline void ofMatrix4x4::set(const ofMatrix4x4& rhs) {
	set(rhs.getPtr());
}

inline void ofMatrix4x4::set(float const * const ptr) {
	float* local_ptr = (float*)_mat;
	for (std::size_t i = 0;i < 16;++i) local_ptr[i] = (float)ptr[i];
}

inline void ofMatrix4x4::set(double const * const ptr) {
	float* local_ptr = (float*)_mat;
	for (std::size_t i = 0;i < 16;++i) local_ptr[i] = (float)ptr[i];
}

inline bool ofMatrix4x4::isIdentity() const {
	return _mat[0][0] == 1.0f && _mat[0][1] == 0.0f && _mat[0][2] == 0.0f &&  _mat[0][3] == 0.0f &&
		   _mat[1][0] == 0.0f && _mat[1][1] == 1.0f && _mat[1][2] == 0.0f &&  _mat[1][3] == 0.0f &&
		   _mat[2][0] == 0.0f && _mat[2][1] == 0.0f && _mat[2][2] == 1.0f &&  _mat[2][3] == 0.0f &&
		   _mat[3][0] == 0.0f && _mat[3][1] == 0.0f && _mat[3][2] == 0.0f &&  _mat[3][3] == 1.0f;
}

inline void ofMatrix4x4::makeOrtho2DMatrix(double left,   double right,
	                        double bottom, double top) {
	makeOrthoMatrix(left, right, bottom, top, -1.0, 1.0);
}

inline ofVec3f ofMatrix4x4::getTranslation() const {
	return ofVec3f(_mat[3][0], _mat[3][1], _mat[3][2]);
}

inline ofVec3f ofMatrix4x4::getScale() const {
	ofVec3f x_vec(_mat[0][0], _mat[1][0], _mat[2][0]);
	ofVec3f y_vec(_mat[0][1], _mat[1][1], _mat[2][1]);
	ofVec3f z_vec(_mat[0][2], _mat[1][2], _mat[2][2]);
	return ofVec3f(x_vec.length(), y_vec.length(), z_vec.length());
}

//static utility methods
inline ofMatrix4x4 ofMatrix4x4::newIdentityMatrix(void) {
	ofMatrix4x4 m;
	m.makeIdentityMatrix();
	return m;
}

inline ofMatrix4x4 ofMatrix4x4::newScaleMatrix(float sx, float sy, float sz) {
	ofMatrix4x4 m;
	m.makeScaleMatrix(sx, sy, sz);
	return m;
}

inline ofMatrix4x4 ofMatrix4x4::newScaleMatrix(const ofVec3f& v ) {
	return newScaleMatrix(v.x, v.y, v.z );
}

inline ofMatrix4x4 ofMatrix4x4::newTranslationMatrix(float tx, float ty, float tz) {
	ofMatrix4x4 m;
	m.makeTranslationMatrix(tx, ty, tz);
	return m;
}

inline ofMatrix4x4 ofMatrix4x4::newTranslationMatrix(const ofVec3f& v ) {
	return newTranslationMatrix(v.x, v.y, v.z );
}

inline ofMatrix4x4 ofMatrix4x4::newRotationMatrix( const ofQuaternion& q ) {
	return ofMatrix4x4(q);
}
inline ofMatrix4x4 ofMatrix4x4::newRotationMatrix(float angle, float x, float y, float z ) {
	ofMatrix4x4 m;
	m.makeRotationMatrix(angle, x, y, z);
	return m;
}
inline ofMatrix4x4 ofMatrix4x4::newRotationMatrix(float angle, const ofVec3f& axis ) {
	ofMatrix4x4 m;
	m.makeRotationMatrix(angle, axis);
	return m;
}
inline ofMatrix4x4 ofMatrix4x4::newRotationMatrix(	float angle1, const ofVec3f& axis1,
    float angle2, const ofVec3f& axis2,
    float angle3, const ofVec3f& axis3) {
	ofMatrix4x4 m;
	m.makeRotationMatrix(angle1, axis1, angle2, axis2, angle3, axis3);
	return m;
}
inline ofMatrix4x4 ofMatrix4x4::newRotationMatrix(const ofVec3f& from, const ofVec3f& to ) {
	ofMatrix4x4 m;
	m.makeRotationMatrix(from, to);
	return m;
}

inline ofMatrix4x4 ofMatrix4x4::getInverseOf( const ofMatrix4x4& matrix) {
	ofMatrix4x4 m;
	m.makeInvertOf(matrix);
	return m;
}

inline ofMatrix4x4 ofMatrix4x4::getTransposedOf( const ofMatrix4x4& matrix) {
	ofMatrix4x4 m(matrix._mat[0][0], matrix._mat[1][0], matrix._mat[2][0],
	               matrix._mat[3][0], matrix._mat[0][1], matrix._mat[1][1], matrix._mat[2][1],
	               matrix._mat[3][1], matrix._mat[0][2], matrix._mat[1][2], matrix._mat[2][2],
	               matrix._mat[3][2], matrix._mat[0][3], matrix._mat[1][3], matrix._mat[2][3],
	               matrix._mat[3][3]);
	return m;
}

inline ofMatrix4x4 ofMatrix4x4::getOrthoNormalOf(const ofMatrix4x4& matrix) {
	ofMatrix4x4 m;
	m.makeOrthoNormalOf(matrix);
	return m;
}

inline ofMatrix4x4 ofMatrix4x4::newOrthoMatrix(double left, double right,
                                        double bottom, double top,
                                        double zNear, double zFar) {
	ofMatrix4x4 m;
	m.makeOrthoMatrix(left, right, bottom, top, zNear, zFar);
	return m;
}

inline ofMatrix4x4 ofMatrix4x4::newOrtho2DMatrix(double left, double right,
    double bottom, double top) {
	ofMatrix4x4 m;
	m.makeOrtho2DMatrix(left, right, bottom, top);
	return m;
}

inline ofMatrix4x4 ofMatrix4x4::newFrustumMatrix(double left, double right,
    double bottom, double top,
    double zNear, double zFar) {
	ofMatrix4x4 m;
	m.makeFrustumMatrix(left, right, bottom, top, zNear, zFar);
	return m;
}

inline ofMatrix4x4 ofMatrix4x4::newPerspectiveMatrix(double fovy, double aspectRatio,
    double zNear, double zFar) {
	ofMatrix4x4 m;
	m.makePerspectiveMatrix(fovy, aspectRatio, zNear, zFar);
	return m;
}

inline ofMatrix4x4 ofMatrix4x4::newLookAtMatrix(const ofVec3f& eye, const ofVec3f& center, const ofVec3f& up) {
	ofMatrix4x4 m;
	m.makeLookAtMatrix(eye, center, up);
	return m;
}

inline ofVec3f ofMatrix4x4::postMult( const ofVec3f& v ) const {
	float d = 1.0f / (_mat[3][0] * v.x + _mat[3][1] * v.y + _mat[3][2] * v.z + _mat[3][3]) ;
	return ofVec3f( (_mat[0][0]*v.x + _mat[0][1]*v.y + _mat[0][2]*v.z + _mat[0][3])*d,
	                 (_mat[1][0]*v.x + _mat[1][1]*v.y + _mat[1][2]*v.z + _mat[1][3])*d,
	                 (_mat[2][0]*v.x + _mat[2][1]*v.y + _mat[2][2]*v.z + _mat[2][3])*d) ;
}

inline ofVec3f ofMatrix4x4::preMult( const ofVec3f& v ) const {
	float d = 1.0f / (_mat[0][3] * v.x + _mat[1][3] * v.y + _mat[2][3] * v.z + _mat[3][3]) ;
	return ofVec3f( (_mat[0][0]*v.x + _mat[1][0]*v.y + _mat[2][0]*v.z + _mat[3][0])*d,
	                 (_mat[0][1]*v.x + _mat[1][1]*v.y + _mat[2][1]*v.z + _mat[3][1])*d,
	                 (_mat[0][2]*v.x + _mat[1][2]*v.y + _mat[2][2]*v.z + _mat[3][2])*d);
}

/// \brief post-multiplies the vector by the matrix (i.e. returns M mult v).
/// The vector is implicitly treated as a column-matrix
inline ofVec4f ofMatrix4x4::postMult( const ofVec4f& v ) const {
	return ofVec4f( (_mat[0][0]*v.x + _mat[0][1]*v.y + _mat[0][2]*v.z + _mat[0][3]*v.w),
	                 (_mat[1][0]*v.x + _mat[1][1]*v.y + _mat[1][2]*v.z + _mat[1][3]*v.w),
	                 (_mat[2][0]*v.x + _mat[2][1]*v.y + _mat[2][2]*v.z + _mat[2][3]*v.w),
	                 (_mat[3][0]*v.x + _mat[3][1]*v.y + _mat[3][2]*v.z + _mat[3][3]*v.w)) ;
}

/// \brief pre-multiplies the vector by the matrix (i.e. returns v mult M)
/// The vector is implicitly treated as a row-matrix
inline ofVec4f ofMatrix4x4::preMult( const ofVec4f& v ) const {
	return ofVec4f( (_mat[0][0]*v.x + _mat[1][0]*v.y + _mat[2][0]*v.z + _mat[3][0]*v.w),
	                 (_mat[0][1]*v.x + _mat[1][1]*v.y + _mat[2][1]*v.z + _mat[3][1]*v.w),
	                 (_mat[0][2]*v.x + _mat[1][2]*v.y + _mat[2][2]*v.z + _mat[3][2]*v.w),
	                 (_mat[0][3]*v.x + _mat[1][3]*v.y + _mat[2][3]*v.z + _mat[3][3]*v.w));
}

/// \brief performs a pre-multiplication transformation on the vector using only the
/// upper left 3x3 portion of the matrix (i.e. only the rotation part).
inline ofVec3f ofMatrix4x4::transform3x3(const ofVec3f& v, const ofMatrix4x4& m) {
	return ofVec3f( (m._mat[0][0]*v.x + m._mat[1][0]*v.y + m._mat[2][0]*v.z),
	                 (m._mat[0][1]*v.x + m._mat[1][1]*v.y + m._mat[2][1]*v.z),
	                 (m._mat[0][2]*v.x + m._mat[1][2]*v.y + m._mat[2][2]*v.z));
}

/// \brief performs a post-multiplication transformation on the vector using only the
/// upper left 3x3 portion of the matrix (i.e. only the rotation part).
inline ofVec3f ofMatrix4x4::transform3x3(const ofMatrix4x4& m, const ofVec3f& v) {
	return ofVec3f( (m._mat[0][0]*v.x + m._mat[0][1]*v.y + m._mat[0][2]*v.z),
	                 (m._mat[1][0]*v.x + m._mat[1][1]*v.y + m._mat[1][2]*v.z),
	                 (m._mat[2][0]*v.x + m._mat[2][1]*v.y + m._mat[2][2]*v.z) ) ;
}

/// \brief translates this matrix by treating the ofVec3f like a translation matrix,
/// and multiplying this Matrix by it in a pre-multiplication manner (T mult M)
inline void ofMatrix4x4::preMultTranslate( const ofVec3f& v ) {
	for (unsigned i = 0; i < 3; ++i) {
		float tmp = v.getPtr()[i];
		if (tmp == 0)
			continue;
		_mat[3][0] += tmp * _mat[i][0];
		_mat[3][1] += tmp * _mat[i][1];
		_mat[3][2] += tmp * _mat[i][2];
		_mat[3][3] += tmp * _mat[i][3];
	}
}

/// \brief translates this matrix by treating the ofVec3f like a translation matrix,
/// and multiplying this Matrix by it in a post-multiplication manner (M mult T)
inline void ofMatrix4x4::postMultTranslate( const ofVec3f& v ) {
	for (std::size_t i = 0; i < 3; ++i) {
		float tmp = v.getPtr()[i];
		if (tmp == 0)
			continue;
		_mat[0][i] += tmp * _mat[0][3];
		_mat[1][i] += tmp * _mat[1][3];
		_mat[2][i] += tmp * _mat[2][3];
		_mat[3][i] += tmp * _mat[3][3];
	}
}

// AARON METHOD
/// \brief the positional argument version of the above
inline void ofMatrix4x4::postMultTranslate( float x, float y, float z) {
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

/// \brief treats the ofVec3f like a scaling matrix and edits this Matrix
/// by multiplying the vector with it in a pre-multiplication style (V mult M)
inline void ofMatrix4x4::preMultScale( const ofVec3f& v ) {
	_mat[0][0] *= v.getPtr()[0];
	_mat[0][1] *= v.getPtr()[0];
	_mat[0][2] *= v.getPtr()[0];
	_mat[0][3] *= v.getPtr()[0];
	_mat[1][0] *= v.getPtr()[1];
	_mat[1][1] *= v.getPtr()[1];
	_mat[1][2] *= v.getPtr()[1];
	_mat[1][3] *= v.getPtr()[1];
	_mat[2][0] *= v.getPtr()[2];
	_mat[2][1] *= v.getPtr()[2];
	_mat[2][2] *= v.getPtr()[2];
	_mat[2][3] *= v.getPtr()[2];
}

/// \brief treats the ofVec3f like a scaling matrix and edits this Matrix
/// by multiplying the vector with it in a post-multiplication style (M mult V)
inline void ofMatrix4x4::postMultScale( const ofVec3f& v ) {
	_mat[0][0] *= v.getPtr()[0];
	_mat[1][0] *= v.getPtr()[0];
	_mat[2][0] *= v.getPtr()[0];
	_mat[3][0] *= v.getPtr()[0];
	_mat[0][1] *= v.getPtr()[1];
	_mat[1][1] *= v.getPtr()[1];
	_mat[2][1] *= v.getPtr()[1];
	_mat[3][1] *= v.getPtr()[1];
	_mat[0][2] *= v.getPtr()[2];
	_mat[1][2] *= v.getPtr()[2];
	_mat[2][2] *= v.getPtr()[2];
	_mat[3][2] *= v.getPtr()[2];
}

/// \brief rotates this Matrix by the provided quaternion
inline void ofMatrix4x4::rotate(const ofQuaternion& q){
	postMultRotate(q);
}

/// \brief rotates this Matrix by the provided angle (in degrees) around an axis defined by the three values
inline void ofMatrix4x4::rotate(float angle, float x, float y, float z){
	postMultRotate(angle,x,y,z);
}

/// \brief Rotates this Matrix by the provided angle (in Radians) around an axis defined by the three values
inline void ofMatrix4x4::rotateRad(float angle, float x, float y, float z){
	postMultRotate(angle*static_cast<float>(RAD_TO_DEG),x,y,z);
}

/// \brief Translates this matrix by the provided amount
inline void ofMatrix4x4::translate( float tx, float ty, float tz ){
	postMultTranslate(tx,ty,tz);
}

/// \brief Translates this matrix by the provided vector
inline void ofMatrix4x4::translate( const ofVec3f& v ){
	postMultTranslate(v);
}

/// \brief scales this matrix by the provided scales
inline void ofMatrix4x4::scale(float x, float y, float z){
	postMultScale(x,y,z);
}

/// \brief scales this matrix, treating the ofVec3f as the diagonal of a scaling matrix.
inline void ofMatrix4x4::scale( const ofVec3f& v ){
	postMultScale(v);
}

/// implementation of the gl-style pre-multiplication versions of the above functions
inline void ofMatrix4x4::glRotate(float angle, float x, float y, float z){
	preMultRotate(ofQuaternion(angle,ofVec3f(x,y,z)));
}

inline void ofMatrix4x4::glRotateRad(float angle, float x, float y, float z){
	preMultRotate(ofQuaternion(angle*static_cast<float>(RAD_TO_DEG),ofVec3f(x,y,z)));
}

inline void ofMatrix4x4::glRotate(const ofQuaternion& q){
	preMultRotate(q);
}

inline void ofMatrix4x4::glTranslate( float tx, float ty, float tz ){
	preMultTranslate(ofVec3f(tx,ty,tz));
}

inline void ofMatrix4x4::glTranslate( const ofVec3f& v ){
	preMultTranslate(v);
}

inline void ofMatrix4x4::glScale(float x, float y, float z){
	preMultScale(ofVec3f(x,y,z));
}

inline void ofMatrix4x4::glScale( const ofVec3f& v ){
	preMultScale(v);
}

// AARON METHOD
inline void ofMatrix4x4::postMultScale( float x, float y, float z ) {
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


inline void ofMatrix4x4::preMultRotate( const ofQuaternion& q ) {
	if (q.zeroRotation())
		return;
	ofMatrix4x4 r;
	r.setRotate(q);
	preMult(r);
}

inline void ofMatrix4x4::postMultRotate( const ofQuaternion& q ) {
	if (q.zeroRotation())
		return;
	ofMatrix4x4 r;
	r.setRotate(q);
	postMult(r);
}

// AARON METHOD
inline void ofMatrix4x4::postMultRotate(float angle, float x, float y, float z) {
	ofMatrix4x4 r;
	r.makeRotationMatrix(angle, x, y, z);
	postMult(r);
}

/// \brief provides Vector3 * Matrix multiplication. Vectors are implicitly treated as row-matrices.
inline ofVec3f operator* (const ofVec3f& v, const ofMatrix4x4& m ) {
	return m.preMult(v);
}
/// \brief provides Vector4 * Matrix multiplication. Vectors are implicitly treated as row-matrices.
inline ofVec4f operator* (const ofVec4f& v, const ofMatrix4x4& m ) {
	return m.preMult(v);
}


/// \endcond
