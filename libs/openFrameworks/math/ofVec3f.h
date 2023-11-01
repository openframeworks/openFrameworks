#pragma once

#include "ofVec2f.h"
#include "ofVec4f.h"
#include "ofMathConstants.h"
#include "ofConstants.h"
#include "glm/vec3.hpp"
#include <cmath>
#include <iostream>

/// \brief  ofVec3f is a class for storing a three dimensional vector.
/// 
/// Moving through space requires knowledge of where things are and where they are
/// going. Vector Maths is the class of mathematics that gives us control over
/// these things in space, allowing for elegant and intuitive descriptions of
/// complex structures and movement. Vectors are at the heart of animations,
/// particle systems, and 2D and 3D graphics.
/// 
/// Vectors in mathematics in general are entities with magnitude (also called
/// length) and direction. A vector whose magnitude is 1 (ie a vector that is
/// *normalized*) is called a *unit vector*. Unit vectors are very handy for
/// storing directions as they can be easily scaled up (or down) to represent
/// motion in a particular direction with a particular length.
/// 
/// *You will also see the term 'vector' used to describe an array of objects in
/// C++ (such as text strings). Don't let this confuse you, they are quite
/// different: one of them is a mathematical term for a fixed-length list of
/// numbers that you can do mathematical operations on, the other is a
/// C++-specific term that means 'dynamically sizeable array'.*
/// 
/// 'ofVec3f has three member variables, x, y, and z, which allow to conveniently
/// 'store 3D properties of an object such as its position, velocity, or
/// 'acceleration.
/// 
/// ~~~~{.cpp}
/// ofVec3f v1; // v1.x is 0, v1.y is 0, v1.z is 0
/// v1.set(10, 50, 80); // now v1.x is 10, v1.y is 50, v1.z is 80
/// ~~~~
/// 
/// Using 'ofVec3f' greatly simplifies arithmetic operations in three dimensions.
/// For example if you have two vectors 'v1' and 'v2', both of which represent a
/// 3D change in position, you can find the total change of position of both of
/// them just by doing an addition 'v1 + v2':
/// 
/// ~~~~{.cpp}
/// ofVec3f v1(5, 2, 1); 
/// // so now v1 represents walking 5 steps forward then 2 steps 
/// // sideways then 1 step upwards
/// ofVec3f v2;
/// v2.set(1, 1, 1); 
/// // so now v2 represents walking 1 step forward then 1 step 
/// // sideways then 1 step upwards
/// 
/// // what happens if you do v1 followed by v2?
/// // to find out just add v1 and v2 together:
/// ofVec3f result = v1 + v2; 
/// // result is (6, 3, 1), or 6 steps forward then 3 steps sideways
/// // then 2 steps upwards
/// ~~~~
/// 
/// You can scale an 'ofVec3f' by multiplying it with a float:
/// 
/// ~~~~{.cpp}
/// // walk 5 steps forward then 2 steps sideways then 1 step upwards
/// ofVec3f v1(5, 2, 1); 
/// // what happens if we do v1 three times?
/// ofVec3f result = v1 * 3; // result is (15, 6, 3), or 
/// // 15 steps forward, 6 steps sideways and 3 steps upwards
/// ~~~~
/// 
/// This also works for subtraction and division.
/// 
/// As you can see this really makes dealing with vectors as easy as dealing with
/// single 'float's or 'int's, and can reduce the number of lines of code you have
/// to write by half, at the same time making your code much easier to read and
/// understand!
/// \sa ofVec2f for 2D vectors
/// \sa ofVec4f for 4D vectors
class ofVec3f {
public:
   	/// \cond INTERNAL
	static const int DIM = 3;
	/// \endcond
	
	/// \brief Stores the `X` component of this vector.
	float x;
	
	/// \brief Stores the `Y` component of this vector.
	float y;
	
	/// \brief Stores the `Z` component of this vector.
	float z;
    
	//---------------------
	/// \name Construct a 3D vector
	/// \{
    
	/// \brief Construct a 3D vector. Defaults to (0,0,0).
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1; 
	/// // default: v1.x is 0, v1.y is 0, v1.z is 0
	///
	/// ofVec3f v2 = ofVec3f(40, 20, 10); 
	/// // v2.x is 40, v2.y is 20, v2.z is 10
	///
	/// ofVec3f v3(0.1, 0.3, -1.5); 
	/// // v3.x is 0.1, v3.y is 0.3, v3.z is -1.5
	/// ~~~~
	ofVec3f();

	/// \brief Construt a 3D vector with `x`, `y` and `z` specified
	ofVec3f( float x, float y, float z=0 );
	
	/// \brief Construct a 3D vector with `x`, `y` and `z` set to `scalar`
	explicit ofVec3f( float scalar );
	
    ofVec3f( const ofVec2f& vec );

	/// \brief Construct a new 3D vector from a 4D vector by 
	/// throwing away the 'w' component.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f mom = ofVec4f(40, 20, 10, 100);
	/// ofVec3f v(mom); // v is (40, 20, 10)
	/// ~~~~
    ofVec3f( const ofVec4f& vec );

	ofVec3f( const glm::vec2 & vec );
	ofVec3f( const glm::vec3 & vec );
	ofVec3f( const glm::vec4 & vec );
	operator glm::vec3() const;

	/// \}

	//---------------------
	/// \name Access components
	/// \{


	/// \brief Returns a pointer to the memory position of the first element of the vector
	/// ('x'); the other elements ('y' and 'z') immediately follow it in memory.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1 = ofVec3f(40, 20, 10);
	/// float * v1Ptr = v1.getPtr();
	/// float x = *(v1Ptr); // x is 40
	/// float y = *(v1Ptr+1); // y is 20
	/// float z = *(v1Ptr+2); // z is 10
	/// ~~~~
	/// 
	/// This is very useful when using arrays of 'ofVec3f's to store geometry
	/// information, as it allows the vector to be treated as a simple C array of
	/// 'float's that can be passed verbatim to OpenGL.
	/// 
	float * getPtr() {
		return (float*)&x;
	}
	const float * getPtr() const {
		return (const float *)&x;
	}
	

	/// \brief Allows to access the individual components of an 'ofVec3f' as though it is an
	/// array
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1 = ofVec3f(40, 20, 10);
	/// float x = v1[0]; // x is 40
	/// float y = v1[1]; // y is 20
	/// float z = v1[2]; // z is 10
	/// ~~~~
	/// 
	/// This function can be handy if you want to do the same operation to all 'x',
	/// 'y' and 'z' components, as it means you can just make a 'for' loop that
	/// repeats 3 times.
	float& operator[]( int n ){
		return getPtr()[n];
	}
	
	float operator[]( int n ) const {
		return getPtr()[n];
	}
	
    
	/// \brief Set 'x', 'y' and 'z' components of this vector with just one function call.
	/// 'z' is optional, it defaults to 0.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1;
	/// v1.set(40, 20, 70);
	/// ~~~~
    void set( float x, float y, float z = 0 );


	/// \brief Setting the values by using other 3 dimension vector ofVec3f.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1;
	/// ofVec3f v2;
	/// v1.set(40, 20, 70);
	/// v2.set(v1);
	/// ~~~~
    void set( const ofVec3f& vec );
	void set( float _scalar );

	/// \}

    //---------------------
	/// \name Comparison 
	/// \{

    /// \brief Check for equality between two ofVec3f
    ///
	/// Returns 'true' if each component is the same as the corresponding component in
	/// 'vec', ie if 'x == vec.x' and 'y == vec.y' and 'z == vec.z'; otherwise returns
	/// 'false'. But you should probably be using ['match'](#match) instead.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(40, 20, 10); 
	/// ofVec3f v2(50, 30, 10); 
	/// ofVec3f v3(40, 20, 10); 
	/// // ( v1 == v2 ) is false
	/// // ( v1 == v3 ) is true
	/// ~~~~
    bool operator==( const ofVec3f& vec ) const;
    
	/// \brief Returns 'true' if any component is different to its corresponding component in
	/// 'vec', ie if 'x != vec.x' or 'y != vec.y' or 'z != vec.z'; otherwise returns
	/// 'false'.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(40, 20, 10); 
	/// ofVec3f v2(50, 20, 40); 
	/// ofVec3f v3(40, 20, 10); 
	/// // ( v1 != v2 ) is true
	/// // ( v1 != v3 ) is false
	/// ~~~~
    bool operator!=( const ofVec3f& vec ) const;

	/// \brief Let you check if two vectors are similar given a tolerance threshold
	/// 'tolerance' (default = 0.0001).	
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1 = ofVec3f(40, 20, 70);
	/// ofVec3f v2 = ofVec3f(40.01, 19.999, 70.05);
	/// // v1.match(v2, 0.1) is true
	/// // v1.match(v2, 0.01) is false (because (70.5-70) > 0.01)
	/// ~~~~
	/// 
    bool match( const ofVec3f& vec, float tolerance = 0.0001f ) const;
    /**
	 * Checks if vectors look in the same direction.
	 */
    bool isAligned( const ofVec3f& vec, float tolerance = 0.0001f ) const;
    bool isAlignedRad( const ofVec3f& vec, float tolerance = 0.0001f ) const;
    
	/// \brief Returns 'true' if this vector is pointing in the same direction as
	/// 'vec', with an angle error threshold 'tolerance' in degrees (default
	/// 0.0001 degrees).	
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1 = ofVec3f(40, 20, 70);
	/// ofVec3f v2 = ofVec3f(4, 2, 7);
	/// // v1.align(v2, 0.0) is true
	/// ~~~~
    bool align( const ofVec3f& vec, float tolerance = 0.0001f ) const;
    
	/// \brief Returns 'true' if this vector is pointing in the same direction
	/// as 'vec', with an angle error threshold 'tolerance' in radians
	/// (default 0.0001).
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1 = ofVec3f(40, 20, 70);
	/// ofVec3f v2 = ofVec3f(4, 2, 7);
	/// // v1.align(v2, 0.0) is true
	/// ~~~~
    bool alignRad( const ofVec3f& vec, float tolerance = 0.0001f ) const;
	
	
    /// \}

	//---------------------
	/// \name Operators
	/// \{

	/// Super easy vector addition. Returns a new vector
	/// ('x'+'vec.x','y'+'vec.y','z'+'vec.z').
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1 = ofVec3f(40, 20, 10); 
	/// ofVec3f v2 = ofVec3f(25, 50, 10);
	/// ofVec3f v3 = v1 + v2; // v3 is (65, 70, 20)
	/// ~~~~
	ofVec3f  operator+( const ofVec3f& pnt ) const;

	/// Returns a new vector with a float value 'f' added to 'x', 'y' and 'z'
	/// members.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(2, 5, 1);
	/// ofVec3f v2 = v1 + 10; // (12, 15, 11)
	/// ~~~~

    ofVec3f  operator+( const float f ) const;
    
	/// Super easy addition assignment. Adds 'vec.x' to 'x', adds 'vec.y' to 'y' and
	/// adds 'vec.z' to 'z'.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1 = ofVec3f(40, 20, 10); 
	/// ofVec3f v2 = ofVec3f(25, 50, 10);
	/// v1 += v2; // v1 is (65, 70, 20)
	/// ~~~~
	ofVec3f& operator+=( const ofVec3f& pnt );
    
	/// Adds a float value 'f' to 'x', 'y' and 'z' members.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(2, 5, 1);
	/// v1 += 10; // (12, 15, 11)
	/// ~~~~
    ofVec3f& operator+=( const float f );
    
	/// Super easy vector subtraction. Returns a new vector
	/// ('x'-'vec.x','y'-'vec.y','z'-'vec.z').	
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1 = ofVec3f(40, 20, 10); 
	/// ofVec3f v2 = ofVec3f(25, 50, 10);
	/// ofVec3f v3 = v1 - v2; // v3 is (15, -30, 0)
	/// ~~~~
	ofVec3f  operator-( const ofVec3f& vec ) const;



	/// Returns a new vector with a float value 'f' subtracted from 'x', 'y' and 'z'
	/// members.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(2, 5, 1);
	/// ofVec3f v2 = v1 - 10; // (-8, -5, -9)
	/// ~~~~
 	ofVec3f  operator-( const float f ) const;

	/// Returns a new 'ofVec3f' that is the inverted version (mirrored in X, Y and Z)
	/// of this vector.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(2, 5, 1);
	/// ofVec3f v2 = -v1; // (-2, -5, -1)
	/// ~~~~
	/// 
    ofVec3f  operator-() const;

	/// Super easy subtraction assignment. Subtracts 'vec.x' from 'x', subtracts
	/// 'vec.y' from 'y' and subtracts 'vec.z' from 'z'.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1 = ofVec3f(40, 20, 10); 
	/// ofVec3f v2 = ofVec3f(25, 50, 10);
	/// v1 -= v2; // v1 is (15, -30, 0)
	/// ~~~~    
	ofVec3f& operator-=( const ofVec3f& vec );

	/// Subtract a float value 'f' from 'x', 'y', and 'z' members.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(2, 5, 1);
	/// v1 -= 10; // (-8, -5, -9)
	/// ~~~~
	ofVec3f& operator-=( const float f );

	/// Returns a new vector ('x'*'vec.x','y'*'vec.y','z'*'vec.z').
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1 = ofVec3f(40, 20, 10); inline
	/// ofVec3f v2 = ofVec3f(2, 4, 10);
	/// ofVec3f v3 = v1 * v2; // (80, 80, 100)
	/// ~~~~
	/// 
	/// Useful for scaling a 3D point by a non-uniform scale.
	/// 
    ofVec3f  operator*( const ofVec3f& vec ) const;

	/// Return a new 'ofVec3f' that is this vector scaled by multiplying 'x', 'y', 'z'
	/// members by 'f'.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(2, 5, 1);
	/// ofVec3f v2 = v1 * 4; // (8, 20, 4)
	/// ~~~~
    ofVec3f  operator*( const float f ) const;
    
	/// Multiplies 'x' by 'vec.x', and multiplies 'y' by 'vec.y', and multiplies 'z'
	/// by 'vec.z'.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1 = ofVec3f(40, 20, 10); 
	/// ofVec3f v2 = ofVec3f(2, 4, 10);
	/// v1 *= v2; // v1 is now (80, 80, 100)
	/// ~~~~
	/// 
	/// Useful for scaling a 3D point by a non-uniform scale.
    ofVec3f& operator*=( const ofVec3f& vec );

	/// Scale this vector by multiplying 'x', 'y' and 'z' members by 'f'.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(2, 5, 1);
	/// v1 *= 4; // (8, 20, 4)
	/// ~~~~
    ofVec3f& operator*=( const float f );
    
	/// Returns a new vector ('x'/'vec.x','y'/'vec.y','z'/'vec.z').
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1 = ofVec3f(40, 20, 10); 
	/// ofVec3f v2 = ofVec3f(2, 4, 10);
	/// ofVec3f v3 = v1 / v2; // (20, 5, 1)
	/// ~~~~
	/// 
	/// Useful for scaling a 3D point by a non-uniform scale.
    ofVec3f  operator/( const ofVec3f& vec ) const;
    
	/// Return a new 'ofVec3f' that is this vector scaled by dividing 'x', 'y'
	/// and 'z' members by 'f'.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(2, 5, 1);
	/// ofVec3f v2 = v1 / 4; // (0.5, 1.25, 0.25)
	/// ~~~~
    ofVec3f  operator/( const float f ) const;
    
	/// Divides 'x' by 'vec.x', divides 'y' by 'vec.y', and divides 'z' by 'vec.z'.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1 = ofVec3f(40, 20, 10); 
	/// ofVec3f v2 = ofVec3f(2, 4, 10);
	/// v1 *= v2; // v1 is now (20, 5, 1)
	/// ~~~~
	/// 
	/// Useful for scaling a 3D point by a non-uniform scale.
    ofVec3f& operator/=( const ofVec3f& vec );

	/// Scale this vector by dividing 'x', 'y' and 'z' members by 'f'.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(2, 5, 1);
	/// v1 /= 4; // (0.5, 1.25, 0.25)
	/// ~~~~
    ofVec3f& operator/=( const float f );
	
    /// \cond INTERNAL
	friend std::ostream& operator<<(std::ostream& os, const ofVec3f& vec);
	friend std::istream& operator>>(std::istream& is, ofVec3f& vec);
	/// \endcond

    /// \}
	
	//---------------------
	/// \name Simple manipulations
	/// \{

	/// \brief Return a new 'ofVec3f' that is the result of scaling this vector up or down so that it has
	/// the requested length.	
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(3, 4); // length is 5
	/// ofVec3f v2 = v1.getScaled(15); // v2 is (9, 12), which has length of 15
	/// ~~~~
	/// 
    ofVec3f  getScaled( const float length ) const;


	/// \brief Scales this vector up or down so that it has the requested length.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(3, 4); // length is 5
	/// v1.scale(15); // v1 is now (9, 12), which has length of 15
	/// ~~~~
    ofVec3f& scale( const float length );
    
	
	/// \brief Returns a new vector that is the result of rotating this vector
	/// by 'angle' degrees about the given axis.
	///
	/// ~~~~{.cpp}
	/// ofVec3f v1(1, 0, 0);
	/// ofVec3f v2 = v1.getRotated(45, ofVec3f(0, 0, 1)); // v2 is (0.707, 0.707, 0)
	/// ofVec3f v3 = v1.getRotated(45, ofVec3f(0, 1, 0)); // v3 is (0.707, 0, -0.707)
	/// ~~~~
	///
    ofVec3f  getRotated( float angle, const ofVec3f& axis ) const;

	/// \brief Returns a new vector that is the result of performing an Euler
	/// rotation of this vector by 'ax' degrees about the x axis, 'ay' degrees
	/// about the y axis, and 'az' degrees about the z axis.
	///
	/// ~~~~~{.cpp}
	/// ofVec3f v1(1, 0, 0);
	/// ofVec3f v2 = v1.getRotated(0, 0, 45); // v2 is (0.707, 0.707, 0)
	/// ofVec3f v3 = v1.getRotated(0, 45, 0); // v3 is (0.707, 0, -0.707)
	/// ~~~~~
	///
	/// Watch out for gimbal lock when specifying multiple rotations in the same call.
	///
    ofVec3f  getRotated(float ax, float ay, float az) const;

	/// \brief Returns a new vector that is the result of rotating this vector by
	/// 'angle' degrees about the axis specified by 'axis', using 'pivot' as the
	/// origin of rotation.
    ofVec3f  getRotated( float angle, const ofVec3f& pivot, const ofVec3f& axis ) const;


	/// \brief Returns a new vector that is the result of rotating this vector by
	/// 'angle' radians about the given axis.
	///
	/// ~~~~{.cpp}
	/// ofVec3f v1(1, 0, 0);
	/// ofVec3f v2 = v1.getRotated(PI / 4, ofVec3f(0, 0, 1)); // v2 is (0.707, 0.707, 0)
	/// ofVec3f v3 = v1.getRotated(PI / 4, ofVec3f(0, 1, 0)); // v3 is (0.707, 0, -0.707)
	/// ~~~~
    ofVec3f  getRotatedRad( float angle, const ofVec3f& axis ) const;

	/// \brief Returns a new vector that is the result of performing an Euler
	/// rotation of this vector by 'ax' radians about the x axis, 'ay' radians
	/// about the y axis, and 'az' radians about the z axis.
	///
	/// ~~~~~{.cpp}
	/// ofVec3f v1(1, 0, 0);
	/// ofVec3f v2 = v1.getRotatedRad(0, 0, PI / 4); // v2 is (0.707, 0.707, 0)
	/// ofVec3f v3 = v1.getRotatedRad(0, PI / 4, 0); // v3 is (0.707, 0, -0.707)
	/// ~~~~~
	///
	/// Watch out for gimbal lock when specifying multiple rotations in the same call.
	///
	ofVec3f  getRotatedRad(float ax, float ay, float az) const;

	/// \brief Returns a new vector that is the result of rotating this vector by
	/// 'angle' radians about the axis specified by 'axis', using 'pivot' as the
	/// origin of rotation.
    ofVec3f   getRotatedRad( float angle, const ofVec3f& pivot, const ofVec3f& axis ) const;

	/// \brief Rotates this vector by 'angle' degrees around the given axis.
	///
	/// ~~~~{.cpp}
	/// ofVec3f v1(1, 0, 0);
	/// ofVec3f v2(1, 0, 0);
	/// v1.rotate(45, ofVec3f(0, 0, 1)); // v1 is now (0.707, 0.707, 0)
	/// v2.rotate(45, ofVec3f(0, 1, 0)); // v2 is now (0.707, 0, -0.707)
	/// ~~~~
	ofVec3f& rotate( float angle, const ofVec3f& axis );

	/// \brief Performs an Euler rotation of this vector by 'ax' degrees about the
	/// x axis, 'ay' degrees about the y axis, and 'az' degrees about the z axis.
	///
	/// ~~~~~{.cpp}
	/// ofVec3f v1(1, 0, 0);
	/// ofVec3f v2(1, 0, 0);
	/// v1.rotate(0, 0, 45); // v1 is now (0.707, 0.707, 0)
	/// v2.rotate(0, 45, 0); // v2 is now (0.707, 0, -0.707)
	/// ~~~~~
	///
	/// Watch out for gimbal lock when specifying multiple rotations in the same call.
	///
    ofVec3f& rotate(float ax, float ay, float az);

	/// \brief Rotates this vector by 'angle' degrees about the axis specified by 'axis',
	/// using 'pivot' as the origin of rotation.
	ofVec3f& rotate( float angle, const ofVec3f& pivot, const ofVec3f& axis );


	/// \brief Rotates this vector by 'angle' radians about the given axis.
	///
	/// ~~~~{.cpp}
	/// ofVec3f v1(1, 0, 0);
	/// ofVec3f v2(1, 0, 0)
	/// v1.rotateRad(PI / 4, ofVec3f(0, 0, 1)); // v1 is now (0.707, 0.707, 0)
	/// v2.rotateRad(PI / 4, ofVec3f(0, 1, 0)); // v2 is now (0.707, 0, -0.707)
	/// ~~~~
    ofVec3f& rotateRad( float angle, const ofVec3f& axis );

	/// \brief Performs an Euler rotation of this vector by 'ax' radians about
	/// the x axis, 'ay' radians about the y axis, and 'az' radians about the z axis.
	///
	/// ~~~~~{.cpp}
	/// ofVec3f v1(1, 0, 0);
	/// ofVec3f v2(1, 0, 0);
	/// v1.rotateRad(0, 0, PI / 4); // v1 is now (0.707, 0.707, 0)
	/// v2.rotateRad(0, PI / 4, 0); // v2 is now (0.707, 0, -0.707)
	/// ~~~~~
	///
	/// Watch out for gimbal lock when specifying multiple rotations in the same call.
    ofVec3f& rotateRad(float ax, float ay, float az);

	/// \brief Rotates this vector by 'angle' radians around the axis specified by 'axis',
	/// using 'pivot' as the origin of rotation.
    ofVec3f& rotateRad( float angle, const ofVec3f& pivot, const ofVec3f& axis );


    	
	
	/// \brief Return a new 'ofVec3f' calculated by copying this vector and then mapping from
	/// its default coordinate system -- origin (0,0,0), X direction (1,0,0), Y
	/// direction (0,1,0), Z direction (0,0,1) -- to a new coordinate system defined
	/// with origin at 'origin', X direction 'vx', and Y direction 'vy', and Z
	/// direction 'vz'.
	/// 
	/// *In most cases you want 'vx', 'vy', and 'vz' to be perpendicular and of unit
	/// length; if they are not perpendicular you will have shearing as part of the
	/// mapping, and if they are not of unit length you will have scaling as part of
	/// the mapping.*
	/// 
	ofVec3f getMapped( const ofVec3f& origin,
					  const ofVec3f& vx,
					  const ofVec3f& vy,
					  const ofVec3f& vz ) const;

	/// \brief Map this vector from its default coordinate system -- origin (0,0,0), X
	/// direction (1,0,0), Y direction (0,1,0), Z direction (0,0,1) -- to a new
	/// coordinate system defined with origin at 'origin', X direction 'vx', and Y
	/// direction 'vy', and Z direction 'vz'.
	///
	/// *In most cases you want 'vx', 'vy', and 'vz' to be perpendicular and of
	/// unit length; if they are not perpendicular you will have shearing as part
	/// of the mapping, and if they are not of unit length you will have scaling
	/// as part of the mapping.*
	/// 
    ofVec3f& map( const ofVec3f& origin,
				 const ofVec3f& vx,
				 const ofVec3f& vy,
				 const ofVec3f& vz );
	
	

    /// \}
	
	
	//---------------------
	/// \name Distance
	/// \{


	/// \brief Treats both this vector and 'pnt' as points in 3D space, and
	/// calculates and returns the distance between them.	
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f p1(3, 4, 2);
	/// ofVec3f p2(6, 8, 5);
	/// float distance = p1.distance( p2 ); // distance is 5.8310
	/// ~~~~
	/// 	
	/// 'distance' involves a square root calculation, which is one of the
	/// slowest things you can do in programming. If you don't need an exact
	/// number but rather just a rough idea of distance (for example when
	/// finding the shortest distance of a bunch of points to a reference
	/// point, where it doesn't matter exactly what the distances are, you
	/// just want the shortest), you can use squareDistance() instead.
	float distance( const ofVec3f& pnt) const;
    
	/// \brief Treats both this vector and 'pnt' as points in 3D space, and calculates and
	/// returns the squared distance between them.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f p1(3, 4, 2);
	/// ofVec3f p2(6, 8, 5);
	/// float distance = p1.distance( p2 ); // distance is 5.8310
	/// ~~~~
	/// 
	/// Use as a much faster alternative to distance() if you don't need
	/// to know an exact number but rather just a rough idea of distance (for example
	/// when finding the shortest distance of a bunch of points to a reference point,
	/// where it doesn't matter exactly what the distances are, you just want the
	/// shortest). It avoids the square root calculation that is ordinarily required
	/// to calculate a length.
	/// 
    float squareDistance( const ofVec3f& pnt ) const;
	
		
	/// \}

	//---------------------
	/// \name Interpolation
	/// \{

	/// \brief Perform a linear interpolation of this vector's position towards 'pnt'
	/// and return the interpolated vector without altering the original. 'p'
	/// controls the amount to move towards 'pnt'. 'p' is normally between 0
	/// and 1 and where 0 means stay the original position and 1 means move
	/// all the way to 'pnt', but you can also have 'p' greater than 1
	/// overshoot 'pnt', or less than 0 to move backwards away from 'pnt'.	
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(0, 5, 0);
	/// ofVec3f v2(10, 10, 20);
	/// ofVec3f v3 = v1.getInterpolated(p2, 0.5); // v3 is (5, 7.5, 10)
	/// ofVec3f v4 = v1.getInterpolated(p2, 0.8); // v4 is (8, 9, 16)
	/// ~~~~
	/// 
    ofVec3f   getInterpolated( const ofVec3f& pnt, float p ) const;

	/// \brief Perform a linear interpolation of this vector's position towards
	/// 'pnt'. 'p' controls the amount to move towards 'pnt'. 'p' is normally
	/// between 0 and 1 and where 0 means stay the original position and 1
	/// means move all the way to 'pnt', but you can also have 'p' greater
	/// than 1 overshoot 'pnt', or less than 0 to move backwards away from
	/// 'pnt'.	
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1( 0, 5, 0 );
	/// ofVec3f v2( 10, 10, 20 );
	/// // go go gadget zeno
	/// v1.interpolate( v2, 0.5 ); // v1 is now (5, 7.5, 10)
	/// v1.interpolate( v2, 0.5 ); // v1 is now (7.5, 8.75, 15)
	/// v1.interpolate( v2, 0.5 ); // v1 is now (8.75, 9.375, 17.5)
	/// v1.interpolate( v2, 0.5 ); // v1 is now (9.375, 9.6875, 18.75)
	/// ~~~~
    ofVec3f&  interpolate( const ofVec3f& pnt, float p );
    
	/// \brief Calculate and return the midpoint between this vector and 'pnt'.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(5, 0, 0);
	/// ofVec3f v2(10, 10, 20);
	/// ofVec3f mid = v1.getMiddle(v2); // mid gets (7.5, 5, 10)
	/// ~~~~
    ofVec3f   getMiddle( const ofVec3f& pnt ) const;

	/// Set this vector to the midpoint between itself and 'pnt'.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1( 0, 5, 0 );
	/// ofVec3f v2( 10, 10, 20);
	/// // go go gadget zeno
	/// v1.middle( v2 ); // v1 is now (5, 7.5, 10)
	/// v1.middle( v2 ); // v1 is now (7.5, 8.75, 15)
	/// v1.middle( v2 ); // v1 is now (8.75, 9.375, 17.5)
	/// v1.middle( v2 ); // v1 is now (9.375, 9.6875, 18.75)
	/// ~~~~
    ofVec3f&  middle( const ofVec3f& pnt );
    
	/// \brief Sets this vector to be the average (*centre of gravity* or
	/// *centroid*) of a given array of 'ofVec3f's. 'points' is the array of
	/// 'ofVec3f's and 'num' specifies the number of 'ofVec3f's in the array.	
	/// 
	/// ~~~~{.cpp}
	/// int numPoints = 10;
	/// ofVec3f points[numPoints];
	/// for ( int i=0; i<numPoints; i++ ) {
	/// 	points[i].set( ofRandom(0,100), ofRandom(0,100), ofRandom(0,100) );
	/// }
	/// ofVec3f centroid;
	/// centroid.average( points, numPoints ); 
	/// // centroid now is the centre of gravity/average of all the random points
	/// ~~~~
	ofVec3f&  average( const ofVec3f* points, int num );
    
    /// \}

    //---------------------
	/// \name Limit
	/// \{
	
	/// \brief Return a normalized copy of this vector. 
	/// 
	/// *Normalization* means to scale the vector so that its length
	/// (magnitude) is exactly 1, at which stage all that is left is the
	/// direction. A normalized vector is usually called a *unit vector*, and
	/// can be used to represent a pure direction (heading).	
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(5, 0, 0);
	/// ofVec3f v1Normalized = v1.getNormalized(); // (1, 0, 0)
	/// ofVec3f v2(5, 0, 5);
	/// ofVec3f v2Normalized = v2.getNormalized(); // (√2, 0, √2)
	/// ~~~~
	ofVec3f  getNormalized() const;
   
	/// \brief Normalize the vector.
	/// 
	/// *Normalizing* means to scale the vector so that its length (magnitude)
	/// is exactly 1, at which stage all that is left is the direction. A
	/// normalized vector is usually called a *unit vector*, and can be used
	/// to represent a pure direction (heading).	
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(5, 0, 0);
	/// v1.normalize(); // v2 is now (1, 0, 0)
	/// ofVec3f v2(5, 0, 5);
	/// v2.normalize(); // v2 is now (√2, 0, √2)
	/// ~~~~
	/// 
  	ofVec3f& normalize();
	
	
	/// \brief Return a copy of this vector with its length (magnitude) restricted to a
	/// maximum of 'max' units by scaling down if necessary.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(5, 0, 1); // length is about 5.1
	/// ofVec3f v2(2, 0, 1); // length is about 2.2
	/// ofVec3f v1Limited = v1.getLimited(3); 
	/// // v1Limited is (2.9417, 0, 0.58835) which has length of 3 in the same direction as v1
	/// ofVec3f v2Limited = v2.getLimited(3);
	/// // v2Limited is (2, 0, 1) (same as v2)
	/// ~~~~
	ofVec3f  getLimited(float max) const;

	/// \brief Restrict the length (magnitude) of this vector to a maximum of 'max'
	/// units by scaling down if necessary.	
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(5, 0, 1); // length is about 5.1
	/// ofVec3f v2(2, 0, 1); // length is about 2.2
	/// v1.limit(3); 
	/// // v1 is now (2.9417, 0, 0.58835) which has length of 3 in the same direction as at initialization
	/// v2.limit(3);
	/// // v2 is unchanged
	/// ~~~~
    ofVec3f& limit(float max);

	
	/// \}

	//---------------------
	/// \name Measurement
	/// \{


	/// Return the length (magnitude) of this vector.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v(3, 4, 1);
	/// float len = v.length(); // len is 5.0990
	/// ~~~~
	/// 
	/// `length' involves a square root calculation, which is one of the
	/// slowest things you can do in programming. If you don't need an exact
	/// number but rather just a rough idea of a length (for example when
	/// finding the shortest distance of a bunch of points to a reference
	/// point, where it doesn't matter exactly what the lengths are, you just
	/// want the shortest), you can use lengthSquared() instead.
	///    
    float length() const;

    /// \brief Return the squared length (squared magnitude) of this vector.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v(3, 4, 1);
	/// float len = v.length(); // len is 5.0990
	/// ~~~~
	/// 
	/// Use as a much faster alternative to length() if you don't need
	/// to know an accurate length but rather just a rough idea of a length (for
	/// example when finding the shortest distance of a bunch of points to a
	/// reference point, where it doesn't matter exactly what the lengths are, you
	/// just want the shortest). It avoids the square root calculation that is
	/// ordinarily required to calculate a length.
    float lengthSquared() const;

	/// \brief Calculate and return the coplanar angle in degrees between this vector
	/// and 'vec'.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(1,0,0);
	/// ofVec3f v2(0,1,0);
	/// float angle = v1.angle(v2); // angle is 90
	/// ~~~~    
	float angle( const ofVec3f& vec ) const;
    
	/// \brief Calculate and return the coplanar angle in radians between this 
	/// vector and 'vec'.
	/// 
	/// ~~~~{.cpp}
	/// ofVec3f v1(1,0,0);
	/// ofVec3f v2(0,1,0);
	/// float angle = v1.angle(v2); // angle is 90
	/// ~~~~
	/// 
    float angleRad( const ofVec3f& vec ) const;
	

	/// \}

	//---------------------
	/// \name Perpendicular
	/// \{

    
	/// \brief Construct a plane using this vector and 'vec' (by finding the plane that both
	/// lectors lie on), and return the vector that is perpendicular to that plane
	/// (the normal to that plane).
	/// 
	/// ~~~~{.cpp}
	/// ofSetLogLevel(OF_LOG_NOTICE);
	/// ofVec3f v1(1,0,0);
	/// ofVec3f v2(0,1,0);
	/// ofVec3f p = v1.getPerpendicular(v2);
	/// ofLog(OF_LOG_NOTICE, "%1.1f, %1.1f, %1.1f\n", p.x, p.y, p.z);
	/// // prints "0.0, 0.0, 1.0"
	/// ~~~~
	/// 
	/// This method is usually used to calculate a normal vector to a surface, which
	/// can then be used to calculate lighting, collisions, and other 3D effects.
	/// 
	ofVec3f  getPerpendicular( const ofVec3f& vec ) const;


	/// \brief Construct a plane using this vector and 'vec' (by finding the plane that both
	/// lie on), and set our 'x', 'y' and 'z' to be the vector that is perpendicular
	/// to the constructed plane (the normal to the plane).
	/// 
	/// ~~~~{.cpp}
	/// ofSetLogLevel(OF_LOG_NOTICE);
	/// ofVec3f v1(1,0,0);
	/// ofVec3f v2(0,1,0); 
	/// v1.perpendicular(v2); 
	/// ofLog(OF_LOG_NOTICE, "%1.1f, %1.1f, %1.1f\n", v1.x, v1.y, v1.z);
	/// // prints "0.0, 0.0, 1.0'
	/// ~~~~
	/// 
	/// This method is usually used to calculate a normal vector to a surface, which
	/// can then be used to calculate lighting, collisions, and other 3D effects.
	/// 
    ofVec3f& perpendicular( const ofVec3f& vec );
	
	

	/// \brief Returns the cross product (vector product) of this vector and 'vec'. This is a
	/// binary operation on two vectors in three-dimensional space, which results in a
	/// vector that is perpendicular to both of the vectors being multiplied, and
	/// normal to the plane containing them. The name *cross product* is derived from
	/// the cross symbol X that is often used to designate this operation; the
	/// alternative name *vector product* emphasizes the vector (rather than scalar)
	/// nature of the result.
	/// 
	/// ![CROSS](math/crossproduct.png)
	/// Image courtesy of Wikipedia
    ofVec3f  getCrossed( const ofVec3f& vec ) const;
    
	/// Set this vector to the cross product (vector product) of itself and
	/// 'vec'. This is a binary operation on two vectors in three-dimensional
	/// space, which results in a vector that is perpendicular to both of the
	/// vectors being multiplied, and normal to the plane containing them. The
	/// name *cross product* is derived from the cross symbol X that is often
	/// used to designate this operation; the alternative name *vector
	/// product* emphasizes the vector (rather than scalar) nature of the
	/// result.
    ofVec3f& cross( const ofVec3f& vec );

	
	
	
	/// \brief Returns the dot product of this vector with 'vec'.
	///
	/// The *dot product* of two vectors, also known as the *scalar product*, is
	/// the product of the magnitude of the two vectors and the cosine of the
	/// angle between them.
	///
	/// One interpretation of the dot product is as a measure of how closely two
	/// vectors align with each other. If they point in exactly the same
	/// direction, their dot product will simply be the product of their
	/// magnitudes, if they are perpendicular, their dot product will be 0, and
	/// if they point in opposite directions, their dot product will be
	/// negative.
	///
	/// The dot product is in contrast to the *cross product*, which returns a
	/// vector rather than a scalar.
	///
	/// ~~~~{.cpp}
	/// ofVec3f a1(2, 0, 0); // magnitude 2, parallel to x-axis
	/// ofVec3f b1(3, 4, 0); // magnitude 5, 53.13 degree angle to a1
	/// float dot = a1.dot(b1); // dot is 2 * 5 * cos(53.13) = 6.0
	///
	/// ofVec3f a2(1, 0, 0); // magnitude 1, parallel to x-axis
	/// ofVec3f b2(0, 0, 1); // magnitude 1, 90 degree angle to a2
	/// dot = a2.dot(b2); // dot is 1 * 1 * cos(90) = 0.0
	///
	/// ofVec3f a3(0, 1, 0); // magnitude 1, parallel to y-axis
	/// ofVec3f b3(0, -1, 0); // magnitude 1, 180 degree angle to a3
	/// dot = a3.dot(b3); // dot is 1 * 1 * cos(180) = -1.0
	/// ~~~~
	/// 
    float dot( const ofVec3f& vec ) const;
	
	/// \}


	
    //-----------------------------------------------
    // this methods are deprecated in 006 please use:
	/// \cond INTERNAL

    // getScaled
    OF_DEPRECATED_MSG("Use member method getScaled() instead.", ofVec3f rescaled( const float length ) const);
	
    // scale
    OF_DEPRECATED_MSG("Use member method scale() instead.", ofVec3f& rescale( const float length ));
	
    // getRotated
    OF_DEPRECATED_MSG("Use member method getRotated() instead.", ofVec3f rotated( float angle, const ofVec3f& axis ) const);
	
    // getRotated should this be const???
    OF_DEPRECATED_MSG("Use member method getRotated() instead.", ofVec3f rotated(float ax, float ay, float az));
	
    // getNormalized
    OF_DEPRECATED_MSG("Use member method getNormalized() instead.", ofVec3f normalized() const);
	
    // getLimited
    OF_DEPRECATED_MSG("Use member method getLimited() instead.", ofVec3f limited(float max) const);
	
    // getCrossed
    OF_DEPRECATED_MSG("Use member method getCrossed() instead.", ofVec3f crossed( const ofVec3f& vec ) const);
	
    // getPerpendicular
    OF_DEPRECATED_MSG("Use member method getPerpendicular() instead.", ofVec3f perpendiculared( const ofVec3f& vec ) const);
    
    // use getMapped
    OF_DEPRECATED_MSG("Use member method getMapped() instead.", ofVec3f mapped( const ofVec3f& origin,
					const ofVec3f& vx,
					const ofVec3f& vy,
					const ofVec3f& vz ) const);
	
    // use squareDistance
    OF_DEPRECATED_MSG("Use member method squareDistance() instead.", float  distanceSquared( const ofVec3f& pnt ) const);
	
    // use getInterpolated
    OF_DEPRECATED_MSG("Use member method getInterpolated() instead.", ofVec3f interpolated( const ofVec3f& pnt, float p ) const);
	
    // use getMiddle
    OF_DEPRECATED_MSG("Use member method getMiddle() instead.", ofVec3f middled( const ofVec3f& pnt ) const);
    
    // use getRotated
    OF_DEPRECATED_MSG("Use member method getRotated() instead.", ofVec3f rotated( float angle,
						const ofVec3f& pivot,
						const ofVec3f& axis ) const);    

    // return all zero vector
    static ofVec3f zero() { return ofVec3f(0, 0, 0); }
    
    // return all one vector
    static ofVec3f one() { return ofVec3f(1, 1, 1); }

    /// \endcond

};


/// \cond INTERNAL


// Non-Member operators
//
//
ofVec3f operator+( float f, const ofVec3f& vec );
ofVec3f operator-( float f, const ofVec3f& vec );
ofVec3f operator*( float f, const ofVec3f& vec );
ofVec3f operator/( float f, const ofVec3f& vec );


/////////////////
// Implementation
/////////////////


inline ofVec3f::ofVec3f( const ofVec2f& vec ):x(vec.x), y(vec.y), z(0) {}
inline ofVec3f::ofVec3f( const ofVec4f& vec ):x(vec.x), y(vec.y), z(vec.z) {}
inline ofVec3f::ofVec3f(): x(0), y(0), z(0) {}
inline ofVec3f::ofVec3f( float _all ): x(_all), y(_all), z(_all) {}
inline ofVec3f::ofVec3f( float _x, float _y, float _z ):x(_x), y(_y), z(_z) {}

inline ofVec3f::ofVec3f( const glm::vec2 & vec ):x(vec.x), y(vec.y), z(0.f){}
inline ofVec3f::ofVec3f( const glm::vec3 & vec ):x(vec.x), y(vec.y), z(vec.z){}
inline ofVec3f::ofVec3f( const glm::vec4 & vec ):x(vec.x), y(vec.y), z(vec.z){}

inline ofVec3f::operator glm::vec3() const{
	return glm::vec3(x,y,z);
}


// Getters and Setters.
//
//
inline void ofVec3f::set( float _scalar ) {
	x = _scalar;
	y = _scalar;
	z = _scalar;
}

inline void ofVec3f::set( float _x, float _y, float _z ) {
	x = _x;
	y = _y;
	z = _z;
}

inline void ofVec3f::set( const ofVec3f& vec ) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
}


// Check similarity/equality.
//
//
inline bool ofVec3f::operator==( const ofVec3f& vec ) const {
	return (x == vec.x) && (y == vec.y) && (z == vec.z);
}

inline bool ofVec3f::operator!=( const ofVec3f& vec ) const {
	return (x != vec.x) || (y != vec.y) || (z != vec.z);
}

inline bool ofVec3f::match( const ofVec3f& vec, float tolerance ) const{
	return (fabs(x - vec.x) < tolerance)
	&& (fabs(y - vec.y) < tolerance)
	&& (fabs(z - vec.z) < tolerance);
}

/**
 * Checks if vectors look in the same direction.
 */
inline bool ofVec3f::isAligned( const ofVec3f& vec, float tolerance ) const {
	float angle = this->angle( vec );
	return  angle < tolerance;
}
inline bool ofVec3f::align( const ofVec3f& vec, float tolerance ) const {
    return isAligned( vec, tolerance );
}

inline bool ofVec3f::isAlignedRad( const ofVec3f& vec, float tolerance ) const {
	float angle = this->angleRad( vec );
	return  angle < tolerance;
}
inline bool ofVec3f::alignRad( const ofVec3f& vec, float tolerance ) const {
    return isAlignedRad( vec, tolerance );
}


// Operator overloading for ofVec3f
//
//

inline std::ostream& operator<<(std::ostream& os, const ofVec3f& vec) {
	os << vec.x << ", " << vec.y << ", " << vec.z;
	return os;
}

inline std::istream& operator>>(std::istream& is, ofVec3f& vec) {
	is >> vec.x;
	is.ignore(2);
	is >> vec.y;
	is.ignore(2);
	is >> vec.z;
	return is;
}

inline ofVec3f ofVec3f::operator+( const ofVec3f& pnt ) const {
	return ofVec3f( x+pnt.x, y+pnt.y, z+pnt.z );
}

inline ofVec3f& ofVec3f::operator+=( const ofVec3f& pnt ) {
	x+=pnt.x;
	y+=pnt.y;
	z+=pnt.z;
	return *this;
}

inline ofVec3f ofVec3f::operator-( const ofVec3f& vec ) const {
	return ofVec3f( x-vec.x, y-vec.y, z-vec.z );
}

inline ofVec3f& ofVec3f::operator-=( const ofVec3f& vec ) {
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

inline ofVec3f ofVec3f::operator*( const ofVec3f& vec ) const {
	return ofVec3f( x*vec.x, y*vec.y, z*vec.z );
}

inline ofVec3f& ofVec3f::operator*=( const ofVec3f& vec ) {
	x*=vec.x;
	y*=vec.y;
	z*=vec.z;
	return *this;
}

inline ofVec3f ofVec3f::operator/( const ofVec3f& vec ) const {
	return ofVec3f( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y, vec.z!=0 ? z/vec.z : z );
}

inline ofVec3f& ofVec3f::operator/=( const ofVec3f& vec ) {
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	vec.z!=0 ? z/=vec.z : z;
	return *this;
}

inline ofVec3f ofVec3f::operator-() const {
	return ofVec3f( -x, -y, -z );
}


//operator overloading for float
//
//
//inline void ofVec3f::operator=( const float f){
//	x = f;
//	y = f;
//	z = f;
//}

inline ofVec3f ofVec3f::operator+( const float f ) const {
	return ofVec3f( x+f, y+f, z+f);
}

inline ofVec3f& ofVec3f::operator+=( const float f ) {
	x += f;
	y += f;
	z += f;
	return *this;
}

inline ofVec3f ofVec3f::operator-( const float f ) const {
	return ofVec3f( x-f, y-f, z-f);
}

inline ofVec3f& ofVec3f::operator-=( const float f ) {
	x -= f;
	y -= f;
	z -= f;
	return *this;
}

inline ofVec3f ofVec3f::operator*( const float f ) const {
	return ofVec3f( x*f, y*f, z*f );
}

inline ofVec3f& ofVec3f::operator*=( const float f ) {
	x*=f;
	y*=f;
	z*=f;
	return *this;
}

inline ofVec3f ofVec3f::operator/( const float f ) const {
	if(f == 0) return ofVec3f( x, y, z);
	
	return ofVec3f( x/f, y/f, z/f );
}

inline ofVec3f& ofVec3f::operator/=( const float f ) {
	if(f == 0) return *this;
	
	x/=f;
	y/=f;
	z/=f;
	return *this;
}


//Scale
//
//
inline ofVec3f ofVec3f::rescaled( const float length ) const {
	return getScaled(length);
}
inline ofVec3f ofVec3f::getScaled( const float length ) const {
	float l = (float)sqrt(x*x + y*y + z*z);
	if( l > 0 )
		return ofVec3f( (x/l)*length, (y/l)*length, (z/l)*length );
	else
		return ofVec3f();
}
inline ofVec3f& ofVec3f::rescale( const float length ) {
	return scale(length);
}
inline ofVec3f& ofVec3f::scale( const float length ) {
	float l = (float)sqrt(x*x + y*y + z*z);
	if (l > 0) {
		x = (x/l)*length;
		y = (y/l)*length;
		z = (z/l)*length;
	}
	return *this;
}



// Rotation
//
//
inline ofVec3f ofVec3f::rotated( float angle, const ofVec3f& axis ) const {
	return getRotated(angle, axis);
}
inline ofVec3f ofVec3f::getRotated( float angle, const ofVec3f& axis ) const {
	ofVec3f ax = axis.getNormalized();
	float a = (float)(ofDegToRad(angle));
	float sina = sin( a );
	float cosa = cos( a );
	float cosb = 1.0f - cosa;
	
	return ofVec3f( x*(ax.x*ax.x*cosb + cosa)
				   + y*(ax.x*ax.y*cosb - ax.z*sina)
				   + z*(ax.x*ax.z*cosb + ax.y*sina),
				   x*(ax.y*ax.x*cosb + ax.z*sina)
				   + y*(ax.y*ax.y*cosb + cosa)
				   + z*(ax.y*ax.z*cosb - ax.x*sina),
				   x*(ax.z*ax.x*cosb - ax.y*sina)
				   + y*(ax.z*ax.y*cosb + ax.x*sina)
				   + z*(ax.z*ax.z*cosb + cosa) );
}

inline ofVec3f ofVec3f::getRotatedRad( float angle, const ofVec3f& axis ) const {
	ofVec3f ax = axis.getNormalized();
	float a = angle;
	float sina = sin( a );
	float cosa = cos( a );
	float cosb = 1.0f - cosa;
	
	return ofVec3f( x*(ax.x*ax.x*cosb + cosa)
				   + y*(ax.x*ax.y*cosb - ax.z*sina)
				   + z*(ax.x*ax.z*cosb + ax.y*sina),
				   x*(ax.y*ax.x*cosb + ax.z*sina)
				   + y*(ax.y*ax.y*cosb + cosa)
				   + z*(ax.y*ax.z*cosb - ax.x*sina),
				   x*(ax.z*ax.x*cosb - ax.y*sina)
				   + y*(ax.z*ax.y*cosb + ax.x*sina)
				   + z*(ax.z*ax.z*cosb + cosa) );
}

inline ofVec3f& ofVec3f::rotate( float angle, const ofVec3f& axis ) {
	ofVec3f ax = axis.getNormalized();
	float a = (float)(ofDegToRad(angle));
	float sina = sin( a );
	float cosa = cos( a );
	float cosb = 1.0f - cosa;
	
	float nx = x*(ax.x*ax.x*cosb + cosa)
	+ y*(ax.x*ax.y*cosb - ax.z*sina)
	+ z*(ax.x*ax.z*cosb + ax.y*sina);
	float ny = x*(ax.y*ax.x*cosb + ax.z*sina)
	+ y*(ax.y*ax.y*cosb + cosa)
	+ z*(ax.y*ax.z*cosb - ax.x*sina);
	float nz = x*(ax.z*ax.x*cosb - ax.y*sina)
	+ y*(ax.z*ax.y*cosb + ax.x*sina)
	+ z*(ax.z*ax.z*cosb + cosa);
	x = nx; y = ny; z = nz;
	return *this;
}


inline ofVec3f& ofVec3f::rotateRad(float angle, const ofVec3f& axis ) {
	ofVec3f ax = axis.getNormalized();
	float a = angle;
	float sina = sin( a );
	float cosa = cos( a );
	float cosb = 1.0f - cosa;
	
	float nx = x*(ax.x*ax.x*cosb + cosa)
	+ y*(ax.x*ax.y*cosb - ax.z*sina)
	+ z*(ax.x*ax.z*cosb + ax.y*sina);
	float ny = x*(ax.y*ax.x*cosb + ax.z*sina)
	+ y*(ax.y*ax.y*cosb + cosa)
	+ z*(ax.y*ax.z*cosb - ax.x*sina);
	float nz = x*(ax.z*ax.x*cosb - ax.y*sina)
	+ y*(ax.z*ax.y*cosb + ax.x*sina)
	+ z*(ax.z*ax.z*cosb + cosa);
	x = nx; y = ny; z = nz;
	return *this;
}

// const???
inline ofVec3f ofVec3f::rotated(float ax, float ay, float az) {
	return getRotated(ax,ay,az);
}

inline ofVec3f ofVec3f::getRotated(float ax, float ay, float az) const {
	float a = (float)cos(ofDegToRad(ax));
	float b = (float)sin(ofDegToRad(ax));
	float c = (float)cos(ofDegToRad(ay));
	float d = (float)sin(ofDegToRad(ay));
	float e = (float)cos(ofDegToRad(az));
	float f = (float)sin(ofDegToRad(az));
	
	float nx = c * e * x - c * f * y + d * z;
	float ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	float nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
	
	return ofVec3f( nx, ny, nz );
}

inline ofVec3f ofVec3f::getRotatedRad(float ax, float ay, float az) const {
	float a = cos(ax);
	float b = sin(ax);
	float c = cos(ay);
	float d = sin(ay);
	float e = cos(az);
	float f = sin(az);
	
	float nx = c * e * x - c * f * y + d * z;
	float ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	float nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
	
	return ofVec3f( nx, ny, nz );
}


inline ofVec3f& ofVec3f::rotate(float ax, float ay, float az) {
	float a = (float)cos(ofDegToRad(ax));
	float b = (float)sin(ofDegToRad(ax));
	float c = (float)cos(ofDegToRad(ay));
	float d = (float)sin(ofDegToRad(ay));
	float e = (float)cos(ofDegToRad(az));
	float f = (float)sin(ofDegToRad(az));
	
	float nx = c * e * x - c * f * y + d * z;
	float ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	float nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
	
	x = nx; y = ny; z = nz;
	return *this;
}


inline ofVec3f& ofVec3f::rotateRad(float ax, float ay, float az) {
	float a = cos(ax);
	float b = sin(ax);
	float c = cos(ay);
	float d = sin(ay);
	float e = cos(az);
	float f = sin(az);
	
	float nx = c * e * x - c * f * y + d * z;
	float ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	float nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
	
	x = nx; y = ny; z = nz;
	return *this;
}


// Rotate point by angle (deg) around line defined by pivot and axis.
//
//
inline ofVec3f ofVec3f::rotated( float angle,
								const ofVec3f& pivot,
								const ofVec3f& axis ) const{
	return getRotated(angle, pivot, axis);
}

inline ofVec3f ofVec3f::getRotated( float angle,
								   const ofVec3f& pivot,
								   const ofVec3f& axis ) const
{
	ofVec3f ax = axis.getNormalized();
	float tx = x - pivot.x;
	float ty = y - pivot.y;
	float tz = z - pivot.z;
	
	float a = ofDegToRad(angle);
	float sina = sin( a );
	float cosa = cos( a );
	float cosb = 1.0f - cosa;
	
	float xrot = tx*(ax.x*ax.x*cosb + cosa)
	+ ty*(ax.x*ax.y*cosb - ax.z*sina)
	+ tz*(ax.x*ax.z*cosb + ax.y*sina);
	float yrot = tx*(ax.y*ax.x*cosb + ax.z*sina)
	+ ty*(ax.y*ax.y*cosb + cosa)
	+ tz*(ax.y*ax.z*cosb - ax.x*sina);
	float zrot = tx*(ax.z*ax.x*cosb - ax.y*sina)
	+ ty*(ax.z*ax.y*cosb + ax.x*sina)
	+ tz*(ax.z*ax.z*cosb + cosa);
	
	
	return ofVec3f( xrot+pivot.x, yrot+pivot.y, zrot+pivot.z );
}


inline ofVec3f ofVec3f::getRotatedRad( float angle,
									  const ofVec3f& pivot,
									  const ofVec3f& axis ) const
{
	ofVec3f ax = axis.getNormalized();
	float tx = x - pivot.x;
	float ty = y - pivot.y;
	float tz = z - pivot.z;
	
	float a = angle;
	float sina = sin( a );
	float cosa = cos( a );
	float cosb = 1.0f - cosa;
	
	float xrot = tx*(ax.x*ax.x*cosb + cosa)
	+ ty*(ax.x*ax.y*cosb - ax.z*sina)
	+ tz*(ax.x*ax.z*cosb + ax.y*sina);
	float yrot = tx*(ax.y*ax.x*cosb + ax.z*sina)
	+ ty*(ax.y*ax.y*cosb + cosa)
	+ tz*(ax.y*ax.z*cosb - ax.x*sina);
	float zrot = tx*(ax.z*ax.x*cosb - ax.y*sina)
	+ ty*(ax.z*ax.y*cosb + ax.x*sina)
	+ tz*(ax.z*ax.z*cosb + cosa);
	
	
	return ofVec3f( xrot+pivot.x, yrot+pivot.y, zrot+pivot.z );
}


inline ofVec3f& ofVec3f::rotate( float angle,
								const ofVec3f& pivot,
								const ofVec3f& axis )
{
	ofVec3f ax = axis.getNormalized();
	x -= pivot.x;
	y -= pivot.y;
	z -= pivot.z;
	
	float a = ofDegToRad(angle);
	float sina = sin( a );
	float cosa = cos( a );
	float cosb = 1.0f - cosa;
	
	float xrot = x*(ax.x*ax.x*cosb + cosa)
	+ y*(ax.x*ax.y*cosb - ax.z*sina)
	+ z*(ax.x*ax.z*cosb + ax.y*sina);
	float yrot = x*(ax.y*ax.x*cosb + ax.z*sina)
	+ y*(ax.y*ax.y*cosb + cosa)
	+ z*(ax.y*ax.z*cosb - ax.x*sina);
	float zrot = x*(ax.z*ax.x*cosb - ax.y*sina)
	+ y*(ax.z*ax.y*cosb + ax.x*sina)
	+ z*(ax.z*ax.z*cosb + cosa);
	
	x = xrot + pivot.x;
	y = yrot + pivot.y;
	z = zrot + pivot.z;
	
	return *this;
}


inline ofVec3f& ofVec3f::rotateRad( float angle,
								   const ofVec3f& pivot,
								   const ofVec3f& axis )
{
	ofVec3f ax = axis.getNormalized();
	x -= pivot.x;
	y -= pivot.y;
	z -= pivot.z;
	
	float a = angle;
	float sina = sin( a );
	float cosa = cos( a );
	float cosb = 1.0f - cosa;
	
	float xrot = x*(ax.x*ax.x*cosb + cosa)
	+ y*(ax.x*ax.y*cosb - ax.z*sina)
	+ z*(ax.x*ax.z*cosb + ax.y*sina);
	float yrot = x*(ax.y*ax.x*cosb + ax.z*sina)
	+ y*(ax.y*ax.y*cosb + cosa)
	+ z*(ax.y*ax.z*cosb - ax.x*sina);
	float zrot = x*(ax.z*ax.x*cosb - ax.y*sina)
	+ y*(ax.z*ax.y*cosb + ax.x*sina)
	+ z*(ax.z*ax.z*cosb + cosa);
	
	x = xrot + pivot.x;
	y = yrot + pivot.y;
	z = zrot + pivot.z;
	
	return *this;
}




// Map point to coordinate system defined by origin, vx, vy, and vz.
//
//
inline ofVec3f ofVec3f::mapped( const ofVec3f& origin,
							   const ofVec3f& vx,
							   const ofVec3f& vy,
							   const ofVec3f& vz ) const{
	return getMapped(origin, vx, vy, vz);
}

inline ofVec3f ofVec3f::getMapped( const ofVec3f& origin,
								  const ofVec3f& vx,
								  const ofVec3f& vy,
								  const ofVec3f& vz ) const
{
	return ofVec3f( origin.x + x*vx.x + y*vy.x + z*vz.x,
				   origin.y + x*vx.y + y*vy.y + z*vz.y,
				   origin.z + x*vx.z + y*vy.z + z*vz.z );
}

inline ofVec3f& ofVec3f::map( const ofVec3f& origin,
							 const ofVec3f& vx,
							 const ofVec3f& vy,
							 const ofVec3f& vz )
{
	float xmap = origin.x + x*vx.x + y*vy.x + z*vz.x;
	float ymap =  origin.y + x*vx.y + y*vy.y + z*vz.y;
	z = origin.z + x*vx.z + y*vy.z + z*vz.z;
	x = xmap;
	y = ymap;
	return *this;
}


// Distance between two points.
//
//
inline float ofVec3f::distance( const ofVec3f& pnt) const {
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	float vz = z-pnt.z;
	return (float)sqrt(vx*vx + vy*vy + vz*vz);
}

inline float  ofVec3f::distanceSquared( const ofVec3f& pnt ) const{
	return squareDistance(pnt);
}

inline float  ofVec3f::squareDistance( const ofVec3f& pnt ) const {
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	float vz = z-pnt.z;
	return vx*vx + vy*vy + vz*vz;
}



// Linear interpolation.
//
//
/**
 * p==0.0 results in this point, p==0.5 results in the
 * midpoint, and p==1.0 results in pnt being returned.
 */

inline ofVec3f ofVec3f::interpolated( const ofVec3f& pnt, float p ) const {
	return getInterpolated(pnt,p);
}

inline ofVec3f ofVec3f::getInterpolated( const ofVec3f& pnt, float p ) const {
	return ofVec3f( x*(1-p) + pnt.x*p,
				   y*(1-p) + pnt.y*p,
				   z*(1-p) + pnt.z*p );
}

inline ofVec3f& ofVec3f::interpolate( const ofVec3f& pnt, float p ) {
	x = x*(1-p) + pnt.x*p;
	y = y*(1-p) + pnt.y*p;
	z = z*(1-p) + pnt.z*p;
	return *this;
}


inline ofVec3f ofVec3f::middled( const ofVec3f& pnt ) const {
	return getMiddle(pnt);
}

inline ofVec3f ofVec3f::getMiddle( const ofVec3f& pnt ) const {
	return ofVec3f( (x+pnt.x)/2.0f, (y+pnt.y)/2.0f, (z+pnt.z)/2.0f );
}

inline ofVec3f& ofVec3f::middle( const ofVec3f& pnt ) {
	x = (x+pnt.x)/2.0f;
	y = (y+pnt.y)/2.0f;
	z = (z+pnt.z)/2.0f;
	return *this;
}


// Average (centroid) among points.
// Addition is sometimes useful for calculating averages too.
//
//
inline ofVec3f& ofVec3f::average( const ofVec3f* points, int num ) {
	x = 0.f;
	y = 0.f;
	z = 0.f;
	for( int i=0; i<num; i++) {
		x += points[i].x;
		y += points[i].y;
		z += points[i].z;
	}
	x /= num;
	y /= num;
	z /= num;
	return *this;
}



// Normalization
//
//
inline ofVec3f ofVec3f::normalized() const {
	return getNormalized();
}

inline ofVec3f ofVec3f::getNormalized() const {
	float length = (float)sqrt(x*x + y*y + z*z);
	if( length > 0 ) {
		return ofVec3f( x/length, y/length, z/length );
	} else {
		return ofVec3f();
	}
}

inline ofVec3f& ofVec3f::normalize() {
	float length = (float)sqrt(x*x + y*y + z*z);
	if( length > 0 ) {
		x /= length;
		y /= length;
		z /= length;
	}
	return *this;
}



// Limit length.
//
//

inline ofVec3f ofVec3f::limited(float max) const {
	return getLimited(max);
}

inline ofVec3f ofVec3f::getLimited(float max) const {
    ofVec3f limited;
    float lengthSquared = (x*x + y*y + z*z);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        float ratio = max/(float)sqrt(lengthSquared);
        limited.set( x*ratio, y*ratio, z*ratio);
    } else {
        limited.set(x,y,z);
    }
    return limited;
}

inline ofVec3f& ofVec3f::limit(float max) {
    float lengthSquared = (x*x + y*y + z*z);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        float ratio = max/(float)sqrt(lengthSquared);
        x *= ratio;
        y *= ratio;
        z *= ratio;
    }
    return *this;
}


// Perpendicular vector.
//
//
inline ofVec3f ofVec3f::crossed( const ofVec3f& vec ) const {
	return getCrossed(vec);
}
inline ofVec3f ofVec3f::getCrossed( const ofVec3f& vec ) const {
	return ofVec3f( y*vec.z - z*vec.y,
				   z*vec.x - x*vec.z,
				   x*vec.y - y*vec.x );
}

inline ofVec3f& ofVec3f::cross( const ofVec3f& vec ) {
	float _x = y*vec.z - z*vec.y;
	float _y = z*vec.x - x*vec.z;
	z = x*vec.y - y*vec.x;
	x = _x;
	y = _y;
	return *this;
}

/**
 * Normalized perpendicular.
 */
inline ofVec3f ofVec3f::perpendiculared( const ofVec3f& vec ) const {
	return getPerpendicular(vec);
}

inline ofVec3f ofVec3f::getPerpendicular( const ofVec3f& vec ) const {
	float crossX = y*vec.z - z*vec.y;
	float crossY = z*vec.x - x*vec.z;
	float crossZ = x*vec.y - y*vec.x;
	
	float length = (float)sqrt(crossX*crossX +
							   crossY*crossY +
							   crossZ*crossZ);
	
	if( length > 0 )
		return ofVec3f( crossX/length, crossY/length, crossZ/length );
	else
		return ofVec3f();
}

inline ofVec3f& ofVec3f::perpendicular( const ofVec3f& vec ) {
	float crossX = y*vec.z - z*vec.y;
	float crossY = z*vec.x - x*vec.z;
	float crossZ = x*vec.y - y*vec.x;
	
	float length = (float)sqrt(crossX*crossX +
							   crossY*crossY +
							   crossZ*crossZ);
	
	if( length > 0 ) {
		x = crossX/length;
		y = crossY/length;
		z = crossZ/length;
	} else {
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}
	
	return *this;
}


// Length
//
//
inline float ofVec3f::length() const {
	return (float)sqrt( x*x + y*y + z*z );
}

inline float ofVec3f::lengthSquared() const {
	return (float)(x*x + y*y + z*z);
}



/**
 * Angle (deg) between two vectors.
 * This is an unsigned relative angle from 0 to 180.
 * http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
 */
inline float ofVec3f::angle( const ofVec3f& vec ) const {
	ofVec3f n1 = this->getNormalized();
	ofVec3f n2 = vec.getNormalized();
	return ofRadToDeg(acos( n1.dot(n2) ));
}

inline float ofVec3f::angleRad( const ofVec3f& vec ) const {
	ofVec3f n1 = this->getNormalized();
	ofVec3f n2 = vec.getNormalized();
	return (float)acos( n1.dot(n2) );
}



/**
 * Dot Product.
 */
inline float ofVec3f::dot( const ofVec3f& vec ) const {
	return x*vec.x + y*vec.y + z*vec.z;
}





// Non-Member operators
//
//
inline ofVec3f operator+( float f, const ofVec3f& vec ) {
    return ofVec3f( f+vec.x, f+vec.y, f+vec.z );
}

inline ofVec3f operator-( float f, const ofVec3f& vec ) {
    return ofVec3f( f-vec.x, f-vec.y, f-vec.z );
}

inline ofVec3f operator*( float f, const ofVec3f& vec ) {
    return ofVec3f( f*vec.x, f*vec.y, f*vec.z );
}

inline ofVec3f operator/( float f, const ofVec3f& vec ) {
    return ofVec3f( f/vec.x, f/vec.y, f/vec.z);
}

/// \endcond
