#pragma once

class ofVec3f;
class ofVec4f;
#include "ofConstants.h"

/// \brief
/// ofVec2f is a class for storing a two dimensional vector. 
///
/// Moving through space requires knowledge of where things are and where they are going.
/// Vector Maths is the class of mathematics that gives us control over these
/// things in space, allowing for elegant and intuitive descriptions of complex
/// structures and movement. Vectors are at the heart of animations, particle
/// systems, and 2D and 3D graphics.
/// 
/// Vectors in mathematics in general are entities with magnitude (also called
/// length) and direction. A vector whose magnitude is 1 (ie a vector that is
/// *normalized*) is called a *unit vector*. Unit vectors are very handy for
/// storing directions as they can be easily scaled up (or down) to represent
/// motion in a particular direction with a particular length.
/// 
/// *You will also see the term vector used to describe an array of objects in C++
/// (such as text strings). Don't let this confuse you, they are quite different:
/// one of them is a mathematical term for a fixed-length list of numbers that
/// you can do mathematical operations on, the other is a C++-specific term that
/// means 'dynamically sizeable array'.*
/// 
/// ofVec2f has two member variables, x and y, which allow to conveniently store
/// 2D properties of an object such as its position, velocity, or acceleration.
/// 
/// ~~~~{.cpp}
/// ofVec2f v1; // v1.x is 0, v1.y is 0
/// v1.set( 10, 50 ); // now v1.x is 10, v1.y is 50
/// ~~~~
/// 
/// Using ofVec2f greatly simplifies arithmetic operations in two dimensions. For
/// example if you have two vectors v1 and v2, both of which represent a 2D change
/// in position, you can find the total change of position of both of them just by
/// doing an addition v1 + v2:
/// 
/// ~~~~{.cpp}
/// ofVec2f v1(5, 2); // v1 represents walking 5 steps forward then 2 steps sideways
/// ofVec2f v2;
/// v2.set(1, 1); // v2 represents walking 1 step forward then 1 step sideways
/// // what happens if you do v1 followed by v2? just add v1 and v2 together:
/// ofVec2f result = v1 + v2; // result is 6 steps forward then 3 steps sideways
/// ~~~~
/// 
/// You can scale an ofVec2f by multiplying it with a float:
/// 
/// ~~~~{.cpp}
/// ofVec2f v1(5, 2); // walk 5 steps forward and 2 steps right
/// // what happens if we do v1 three times?
/// ofVec2f result = v1 * 3; // result is 15 steps forward and 6 steps right
/// ~~~~
/// 
/// This also works for subtraction and division.
/// 
/// As you can see this really makes dealing with vectors as easy as dealing with
/// single floats or ints, and can reduce the number of lines of code you have to
/// write by half, at the same time making your code much easier to read and
/// understand!
///
/// \sa ofVec3f for 3D vectors
/// \sa ofVec4f for 4D vectors
class ofVec2f {
public:
	/// \cond INTERNAL
	static const int DIM = 2;
	//// \endcond
	
	/// \brief Stores the `x` component of the vector.
	float x;

	/// \brief Stores the `y` component of the vector.
	float y;
    
    //---------------------
	/// \name Construct a 2D vector
	/// \{
    
	/// \brief Construct a 2D vector.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1; // default: v1.x is 0, v1.y is 0
	/// ofVec2f v2 = ofVec2f(40, 20); // v2.x is 40, v2.y is 20
	/// ofVec3f v3(0.1, 0.3); // v3.x is 0.1, v3.y is 0.3
	/// ~~~~
	///
	/// \param x The x component
	/// \param y The y component
	/// \returns A new ofVec2f with the `x` and `y` components set to 0
	ofVec2f();

	/// \brief Construct a 2D vector with `x` and `y` set to `scalar`
	explicit ofVec2f( float scalar );
	
	/// \brief Construct a 2D vector with specific `x` and `y components
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1; // default: v1.x is 0, v1.y is 0
	/// ofVec2f v2 = ofVec2f(40, 20); // v2.x is 40, v2.y is 20
	/// ofVec3f v3(0.1, 0.3); // v3.x is 0.1, v3.y is 0.3
	/// ~~~~
	///
	/// \param x The x component
	/// \param y The y component
	/// \returns A new ofVec2f with the `x` and `y` components set
	ofVec2f( float x, float y );

	/// \brief Create a 2D vector (ofVec2f) from a 3D vector (ofVec3f) by
	/// \throwing away the z component of the 3D vector.
	///
	/// ~~~~{.cpp}
	/// ofVec3f mom3d(40, 20, 50); // 3d vector 
	/// ofVec2f v(mom3d); // v.x is 40, v.y is 20
	/// ~~~~
	/// 
    ofVec2f( const ofVec3f& vec );

	/// \brief Create a 2D vector (ofVec2f) from a 4D vector (ofVec4f) by throwing away the z
	/// and w components of the 4D vector.
	/// 
	/// ~~~~{.cpp}
	/// ofVec4f mom4d(40, 20, 50, 80); // 4d vector 
	/// ofVec2f v(mom4d); // v.x is 40, v.y is 20
	/// ~~~~
	/// 
    ofVec2f( const ofVec4f& vec );
	
    /// \}

	//---------------------
	/// \name Access components
	/// \{


	/// \brief Returns a pointer to the memory position of the first element of the vector (x);
	/// the second element (y) immediately follows it in memory.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1 = ofVec2f(40, 20);
	/// float * v1Ptr = v1.getPtr();
	/// float x = *(v1Ptr); // x is 40
	/// float y = *(v1Ptr+1); // y is 20
	/// ~~~~
	/// 
	/// This is very useful when using arrays of ofVec2fs to store geometry
	/// information, as it allows the vector to be treated as a simple C array of
	/// floats that can be passed verbatim to OpenGL.     
	float * getPtr() {
		return (float*)&x;     
	}

	const float * getPtr() const {
		return (const float *)&x;
	}
	
	/// \brief Allows to access the x and y components of an ofVec2f as though it is an array
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1 = ofVec2f(40, 20);
	/// float x = v1[0]; // x is 40
	/// float y = v1[1]; // y is 20
	/// ~~~~
	/// 
	/// This function can be handy if you want to do the same operation to both x and
	/// y components, as it means you can just make a for loop that repeats twice.
	float& operator[]( int n ){
		return getPtr()[n];
	}
	
	float operator[]( int n ) const {
		return getPtr()[n];
	}
	
	
    
	/// \brief Set x and y components of this vector with just one function call.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1;
	/// v1.set(40, 20);
	/// ~~~~
	/// 
    void set( float x, float y );

	/// \brief Set the x and y components of this vector by copying the corresponding values from vec.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(40, 20);
	/// ofVec2f v2;
	/// v2.set(v1); // v2.x is 40, v2.y is 20
	/// ~~~~
	/// 
    void set( const ofVec2f& vec );
	
	void set( float scalar );

    /// \}

    //---------------------
	/// \name Comparison 
	/// \{

	
	/// \brief Check for equality between two ofVec2f
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(40, 20); 
	/// ofVec2f v2(50, 30); 
	/// ofVec2f v3(40, 20); 
	/// // ( v1 == v2 ) is false
	/// // ( v1 == v3 ) is true
	/// ~~~~
	///
	/// \returns true if each component is the same as the corresponding
	/// component in vec, ie if x == vec.x and y == vec.y; otherwise returns
	/// false.
    bool operator==( const ofVec2f& vec ) const;

    /// \brief Check for inequality between two ofVec2f
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(40, 20); 
	/// ofVec2f v2(50, 30); 
	/// ofVec2f v3(40, 20); 
	/// // ( v1 != v2 ) is true
	/// // ( v1 != v3 ) is false
	/// ~~~~
	///
	/// \returns true if any component is different to its corresponding
	/// component in vec, ie if 'x != vec.x' or 'y != vec.y', otherwise returns
	/// false.
    bool operator!=( const ofVec2f& vec ) const;

	/// \brief Returns true if each component is *close enough* to its corresponding
	/// component in vec, where what is *close enough* is determined by the value of
	/// tolerance:
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1 = ofVec2f(40, 20);
	/// ofVec2f v2 = ofVec2f(40.01, 19.999);
	/// // v1.match(v2, 0.1) returns true
	/// // v1.match(v2, 0.001) returns false
	/// ~~~~
	/// 
	/// This is handy if, for example, you want to find out when a point becomes
	/// *close enough* to another point to trigger an event.
	/// 
    bool match( const ofVec2f& vec, float tolerance=0.0001 ) const;
    
	/// \brief Determine if two vectors are aligned
    /// 
    /// ~~~~{.cpp}
	/// ofVec2f v1 = ofVec2f(40, 20);
	/// ofVec2f v2 = ofVec2f(4, 2);
	/// v1.isAligned(v2) // returns true
	/// ~~~~
    /// \param vec The vector to compare alignment with
    /// \param tolerance an angle tolerance/threshold (specified in degrees) for deciding if the vectors are sufficiently aligned.
    /// \returns true if both vectors are aligned (pointing in the same direction). 
    bool isAligned( const ofVec2f& vec, float tolerance=0.0001 ) const;
    
    /// \brief Determine if two vectors are aligned with tolerance in radians
    /// \param vec The vector to compare alignment with
    /// \param tolerance an angle tolerance/threshold (specified in radians) for deciding if the vectors are sufficiently aligned.
    /// \sa isAligned()
    bool isAlignedRad( const ofVec2f& vec, float tolerance=0.0001 ) const;

    /// \brief Determine if two vectors are aligned
    /// 
    /// ~~~~{.cpp}
	/// ofVec2f v1 = ofVec2f(40, 20);
	/// ofVec2f v2 = ofVec2f(4, 2);
	/// v1.align(v2) // returns true
	/// ~~~~
    /// \param vec The vector to compare alignment with
    /// \param tolerance an angle tolerance/threshold (specified in degrees) for deciding if the vectors are sufficiently aligned.
    /// \returns true if both vectors are aligned (pointing in the same direction). 
    bool align( const ofVec2f& vec, float tolerance=0.0001 ) const;

    /// \brief Determine if two vectors are aligned with tolerance in radians
    /// \param vec The vector to compare alignment with
    /// \param tolerance an angle tolerance/threshold (specified in radians) for deciding if the vectors are sufficiently aligned.
    /// \sa align()
    bool alignRad( const ofVec2f& vec, float tolerance=0.0001 ) const;
	
	/// \}

	//---------------------
	/// \name Operators
	/// \{

    
	/// \brief Super easy vector addition. Returns a new vector (x+vec.x,y+vec.y).
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1 = ofVec2f(40, 20); 
	/// ofVec2f v2 = ofVec2f(25, 50);
	/// ofVec3f v3 = v1 + v2; // v3 is (65, 70)
	/// ~~~~
    ofVec2f  operator+( const ofVec2f& vec ) const;
    
    /// \brief Returns a new vector with a float value f added to both x and y members.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(2, 5);
	/// ofVec2f v2 = v1 + 10; // (12, 15)
	/// ~~~~
    ofVec2f  operator+( const float f ) const;

	/// \brief Super easy addition assignment. Adds vec.x to x, and adds vec.y to y.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1 = ofVec2f(40, 20); 
	/// ofVec2f v2 = ofVec2f(25, 50);
	/// v1 += v2; // v1 is (65, 70)
	/// ~~~~
    ofVec2f& operator+=( const ofVec2f& vec );

	/// \brief Adds a float value f to both x and y members.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(2, 5);
	/// v1 += 10; // (12, 15)
	/// ~~~~
    ofVec2f& operator+=( const float f );

    /// \brief Super easy vector subtraction. Returns a new vector (x-vec.x,y-vec.y).
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1 = ofVec2f(40, 20); 
	/// ofVec2f v2 = ofVec2f(25, 50);
	/// ofVec3f v3 = v1 - v2; // v3 is (15, -30)
	/// ~~~~
    ofVec2f  operator-( const ofVec2f& vec ) const;
    
	/// \brief Returns a new vector with a float value f subtracted from both x and y members.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(2, 5);
	/// ofVec2f v2 = v1 - 10; // (-8, -5)
	/// ~~~~
    ofVec2f  operator-( const float f ) const;

	/// \brief Returns a new ofVec2f that is the inverted version (mirrored in X and Y) of this vector.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(2, 5);
	/// ofVec2f v2 = -v1; // (-2, -5)
	/// ~~~~    
    ofVec2f  operator-() const;

	/// \brief Super easy subtraction assignment. Subtracts vec.x from x, and subtracts vec.y from y.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1 = ofVec2f(40, 20); 
	/// ofVec2f v2 = ofVec2f(25, 50);
	/// v1 -= v2; // v1 is (15, -30)
	/// ~~~~
    ofVec2f& operator-=( const ofVec2f& vec );

    /// \brief Subtract a float value f from both x and y members.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(2, 5);
	/// v1 -= 10; // (-8, -5)
	/// ~~~~
	ofVec2f& operator-=( const float f );

	/// \brief Returns a new vector (x*vec.x , y*vec.y).
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1 = ofVec2f(40, 20); 
	/// ofVec2f v2 = ofVec2f(2, 4);
	/// ofVec2f v3 = v1 * v2; // (80, 80)
	/// ~~~~
	/// 
	/// Useful for scaling a 2D point by a non-uniform scale.
	/// 
    ofVec2f  operator*( const ofVec2f& vec ) const;
    
	/// \brief Return a new ofVec2f that is this vector scaled by multiplying both x
	/// and y members by the float.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(2, 5);
	/// ofVec2f v2 = v1 * 4; // (8, 20)
	/// ~~~~
    ofVec2f  operator*( const float f ) const;

	/// \brief Multiplies x by vec.x, and multiplies y by vec.y.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1 = ofVec2f(40, 20); 
	/// ofVec2f v2 = ofVec2f(2, 4);
	/// v1 *= v2; // v1 is now (80, 80)
	/// ~~~~
	/// 
	/// Useful for scaling a 2D point by a non-uniform scale.
    ofVec2f& operator*=( const ofVec2f& vec );

	/// \brief Scale this vector by multiplying both x and y members by f.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(2, 5);
	/// v1 *= 4; // (8, 20)
	/// ~~~~
    ofVec2f& operator*=( const float f );

	/// \brief Returns a new vector (x/vec.x,y/vec.y).
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1 = ofVec2f(40, 20); 
	/// ofVec2f v2 = ofVec2f(2, 4);
	/// ofVec3f v3 = v1 / v2; // (20, 5)
	/// ~~~~
	/// 
	/// Useful for scaling a 2D point by a non-uniform scale.
    ofVec2f  operator/( const ofVec2f& vec ) const;

	/// \brief Return a new ofVec2f that is this vector scaled by dividing
	/// both x and y members by f.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(2, 5);
	/// ofVec2f v2 = v1 / 4; // (0.5, 1.25)
	/// ~~~~
    ofVec2f  operator/( const float f ) const;

	/// \brief Divides x by vec.x, and divides y by vec.y.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1 = ofVec2f(40, 20); 
	/// ofVec2f v2 = ofVec2f(2, 4);
	/// v1 *= v2; // v1 is now (20, 5)
	/// ~~~~
	/// 
	/// Useful for scaling a 2D point by a non-uniform scale.
    ofVec2f& operator/=( const ofVec2f& vec );
	
	/// \brief Scale this vector by dividing both x and y members by f.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(2, 5);
	/// v1 /= 4; // (0.5, 1.25)
	/// ~~~~
    ofVec2f& operator/=( const float f );

	
	/// \cond INTERNAL
	friend ostream& operator<<(ostream& os, const ofVec2f& vec);
	friend istream& operator>>(istream& is, const ofVec2f& vec);
	/// \endcond
	
	/// \}
	
	//---------------------
	/// \name Simple manipulations
	/// \{

	/// Return a new ofVec2f that is the result of scaling this vector up or down so
	/// that it has the requested length.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1( 3, 4 ); // length is 5
	/// ofVec2f v2 = v1.getScaled( 15 ); // ( 9, 12 ), length is now 15
	/// ~~~~ofVec2f  
	/// 
	/// \sa scale()
    ofVec2f  getScaled( const float length ) const;
    
	/// \brief Scales this vector up or down so that it has the requested length.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1( 3, 4 ); // length is 5
	/// v1.scale( 15 ); // v1 is now (9, 12), with length 15
	/// ~~~~
	///
	/// \sa getScaled()
    ofVec2f& scale( const float length );
	
	
	/// \brief Return a new ofVec2f that is the result of rotating this vector by angle
	/// degrees around the origin.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(1, 0);
	/// ofVec2f v2 = v1.getRotated( 45 ); // v2 is (√2, √2)
	/// ofVec3f v3 = v2.getRotated( 45 ); // v3 is (0, 1)
	/// ~~~~
	/// 
	/// \sa getRotatedRad()
	/// \sa rotate()
    ofVec2f  getRotated( float angle ) const;

    /// \brief Like getRotated() but rotates around `pivot` rather than around the origin
    ofVec2f  getRotated( float angle, const ofVec2f& pivot ) const;
    
	/// \brief Return a new ofVec2f that is the result of rotating this vector by angle
	/// radians around the origin.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(1, 0);
	/// ofVec2f v2 = v1.getRotatedRad( PI/4 ); // v2 is (√2, √2)
	/// ofVec3f v3 = v2.getRotatedRad( PI/4 ); // v3 is (0, 1)
	/// ~~~~
	///     
    ofVec2f  getRotatedRad( float angle ) const;

    /// \brief Like getRotatedRad() but rotates around `pivot` rather than around the origin
    ofVec2f  getRotatedRad( float angle, const ofVec2f& pivot ) const;


	/// \brief Rotate this vector by angle degrees around the origin.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(1, 0);
	/// v1.rotate( 45 ); // (√2, √2)
	/// v1.rotate( 45 ); // (0, 1)
	/// ~~~~
	///
	/// \sa getRotated()
    ofVec2f& rotate( float angle );

    /// \brief Like rotate() but rotates around `pivot` rather than around the origin
    ofVec2f& rotate( float angle, const ofVec2f& pivot );
    
	/// \brief Rotate this vector by angle radians around the origin.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(1, 0);
	/// v1.rotate( PI/4 ); // (√2, √2)
	/// v1.rotate( PI/4 ); // (0, 1)
	/// ~~~~
	///
	/// \sa getRotatedRad()
    ofVec2f& rotateRad( float angle );
	
    /// \brief Like rotateRad() but rotates around `pivot` rather than around the origin
	ofVec2f& rotateRad( float angle, const ofVec2f& pivot );
	
	
    
	/// \brief Get vector mapped to new coordinate system
	/// 
	/// In most cases you want `vx` and `vy` to be perpendicular and of unit length; if
	/// they are not perpendicular you will have shearing as part of the mapping, and
	/// if they are not of unit length you will have scaling as part of the mapping.
	///
	/// \returns A new ofVec2f calculated by copying this vector and then mapping from
	/// its default coordinate system -- origin (0,0), X direction (1,0), Y direction
	/// (0,1) -- to a new coordinate system defined with origin at origin, X direction
	/// vx, and Y direction vy.
    ofVec2f getMapped( const ofVec2f& origin,
					  const ofVec2f& vx,
					  const ofVec2f& vy ) const;


	/// \brief Maps this vector from its default coordinate system -- origin (0,0), X
	/// direction (1,0), Y direction (0,1) -- to a new coordinate system defined with
	/// origin at origin, X direction vx, and Y direction vy.
	/// 
	/// In most case you want vx and vy to be perpendicular and of unit length; if
	/// they are not perpendicular you will have shearing as part of the mapping, and
	/// if they are not of unit length you will have scaling as part of the mapping.
    /// 
	/// \sa perpendicular()
    ofVec2f& map( const ofVec2f& origin,
				 const ofVec2f& vx, const ofVec2f& vy );


    /// \}
	
	
	//---------------------
	/// \name Distance
	/// \{

    /// \brief Distance between two points.
    ///
    /// Treats both this vector and pnt as points in 2D space, and calculates and
    /// returns the distance between them.
    ///
	/// ~~~~{.cpp}
	/// ofVec2f p1( 3, 4 );
	/// ofVec2f p2( 6, 8 );
	/// float distance = p1.distance( p2 ); // distance is 5
	/// ~~~~
	/// 
	/// Distance involves a square root calculation, which is one of the slowest
	/// things you can do in programming. If you don't need an exact number but rather
	/// just a rough idea of distance (for example when finding the shortest distance
	/// of a bunch of points to a reference point, where it doesn't matter exactly
	/// what the distances are, you just want the shortest), you can use
	/// squareDistance() instead.
	/// 
	/// \param pnt The point to calculate the distance to
	/// \returns The distance as float
	/// \sa squareDistance()
    float distance( const ofVec2f& pnt) const;

    /// \brief Distance between two points squared.
    ///
	/// Treats both this vector and pnt as points in 2D space, and calculates and
	/// returns the squared distance between them.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f p1( 3, 4 );
	/// ofVec2f p2( 6, 8 );
	/// float distance = p1.distance( p2 ); // distance is 5
	/// ~~~~
	/// 
	/// Use as a much faster alternative to [distance](#distance) if you don't need to
	/// know an exact number but rather just a rough idea of distance (for example
	/// when finding the shortest distance of a bunch of points to a reference point,
	/// where it doesn't matter exactly what the distances are, you just want the
	/// shortest). It avoids the square root calculation that is ordinarily required
	/// to calculate a length.
	///
	/// \returns The distance squared as float
	/// \sa distance()
    float squareDistance( const ofVec2f& pnt ) const;
	
	/// \}

	//---------------------
	/// \name Interpolation
	/// \{

    /// \brief Linear interpolation
    /// 
	/// Perform a linear interpolation of this vector's position towards pnt
	/// and return the interpolated position without altering the original
	/// vector. 
	/// 
	/// `p` is normally between 0 and 1 and where 0 means stay the original position and 1
	/// means move all the way to pnt, but you can also have p greater than 1
	/// overshoot pnt, or less than 0 to move backwards away from pnt.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1( 0, 5 );
	/// ofVec2f v2( 10, 10 );
	/// ofVec3f v3 = v1.getInterpolated( v2, 0.5 ); // v3 is (5, 7.5)
	/// ofVec3f v4 = v1.getInterpolated( v2, 0.8 ); // v4 is (8, 9)
	/// ~~~~
	///
	/// \param pnt The point to move towards
	/// \param p The amount to move towards pnt
	/// \sa interpolate()
    ofVec2f   getInterpolated( const ofVec2f& pnt, float p ) const;
    
    /// \brief Linear interpolation
    /// 
	/// Perform a linear interpolation of this vector's position towards pnt. p
	/// controls the amount to move towards pnt. p is normally between 0 and 1 and
	/// where 0 means stay the original position and 1 means move all the way to pnt,
	/// but you can also have p greater than 1 overshoot pnt, or less than 0 to move
	/// backwards away from pnt.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1( 0, 5 );
	/// ofVec2f v2( 10, 10 );
	/// // go go gadget zeno
	/// v1.interpolate( v2, 0.5 ); // v1 is now (5, 7.5)
	/// v1.interpolate( v2, 0.5 ); // v1 is now (7.5, 8.75)
	/// v1.interpolate( v2, 0.5 ); // v1 is now (8.75, 9.375)
	/// v1.interpolate( v2, 0.5 ); // v1 is now (9.375, 9.6875)
	/// ~~~~
	/// 
	/// \sa getInterpolated()
    ofVec2f&  interpolate( const ofVec2f& pnt, float p );

	/// \brief Calculate and return the midpoint between this vector and pnt.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(5, 0);
	/// ofVec2f v2(10, 10);
	/// ofVec3f mid = v1.getMiddle(v2); // mid gets (7.5, 5)
	/// ~~~~
	///
	/// \param pnt The vector to find the middle to
	/// \returns The middle between this vector and `pnt`
	/// \sa middle()
    ofVec2f   getMiddle( const ofVec2f& pnt ) const;

	/// \brief Set this vector to the midpoint between itself and pnt.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1( 0, 5 );
	/// ofVec2f v2( 10, 10 );
	/// v1.middle( v2 ); // v1 is now (5, 7.5)
	/// v1.middle( v2 ); // v1 is now (7.5, 8.75)
	/// v1.middle( v2 ); // v1 is now (8.75, 9.375)
	/// v1.middle( v2 ); // v1 is now (9.375, 9.6875)
	/// ~~~~    
	/// 
	/// \sa getMiddle()
    ofVec2f&  middle( const ofVec2f& pnt );

	/// \brief Average vector over an array of points 
	///  
	/// Sets this vector to be the average (*centre of gravity* or *centroid*) 
	/// of a given array of ofVec2f. 
	/// 
	/// ~~~~{.cpp}
	/// int numPoints = 10;
	/// ofVec2f points[numPoints];
	/// for ( int i=0; i<numPoints; i++ ) {
	/// 	points[i].set( ofRandom(0,100), ofRandom(0,100) );
	/// }
	/// ofVec2f centroid;
	/// centroid.average( points, numPoints ); 
	/// // centroid now is the centre of gravity/average of all the random points
	/// ~~~~
	/// 
	/// \param points The array of ofVec2f to avarage over
	/// \param num specifies the number of ofVec2f in the array.
	/// \returns Vector that is the avarage of the points in the array
    ofVec2f&  average( const ofVec2f* points, int num );
    
    /// \}

    //---------------------
	/// \name Limit
	/// \{

	/// \brief Returns a normalized copy of this vector. 
	/// 	
	/// *Normalization* means to scale the vector so that its length
	/// (magnitude) is exactly 1, at which stage all that is left is the
	/// direction. A normalized vector is usually called a *unit vector*, and
	/// can be used to represent a pure direction (heading).	
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(5, 0);
	/// ofVec2f v1Normalized = v1.getNormalized(); // (1, 0)
	/// ofVec2f v2(5, 5);
	/// ofVec2f v2Normalized = v2.getNormalized(); // (√2, √2)
	/// ~~~~
    ofVec2f  getNormalized() const;

	/// \brief Normalize the vector.
	/// 
	/// *Normalizing* means to scale the vector so that its length (magnitude) is
	/// exactly 1, at which stage all that is left is the direction. A normalized
	/// vector is usually called a *unit vector*, and can be used to represent a pure
	/// direction (heading).
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(5, 0);
	/// v1.normalize(); // v2 is now (1, 0)
	/// ofVec2f v2(5, 5);
	/// v2.normalize(); // v2 is now (√2, √2)
	/// ~~~~
	/// \sa getNormalized()
    ofVec2f& normalize();
	
	
    
    /// \brief Get vector limited by length
	///     
	/// ~~~~{.cpp}
	/// ofVec2f v1(5, 1); // length is about 5.1
	/// ofVec2f v2(2, 1); // length is about 2.2
	/// ofVec2f v1Limited = v1.getLimited(3); 
	/// // v1Limited is (2.9417, 0.58835) which has length of 3 in the same direction as v1
	/// ofVec2f v2Limited = v2.getLimited(3);
	/// // v2Limited is (2, 1) (same as v2)
	/// ~~~~
	/// 
	/// \sa limit()
	/// \param max The maximum length of the vector to return
	/// \returns A copy of this vector with its length (magnitude) restricted to a
    /// maximum of max units by scaling down if necessary.
	ofVec2f  getLimited(float max) const;

    ofVec2f& getLimited(float max);

   	/// \brief Restrict the length (magnitude) of this vector to a maximum of max units by scaling down if necessary.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v1(5, 1); // length is about 5.1
	/// ofVec2f v2(2, 1); // length is about 2.2
	/// v1.limit(3); 
	/// // v1 is now (2.9417, 0.58835) which has length of 3 in the same direction as at initialization
	/// v2.limit(3);
	/// // v2 is unchanged
	/// ~~~~
	///
	/// \sa limit()
    ofVec2f& limit(float max);

	
	/// \}

	//---------------------
	/// \name Measurement
	/// \{

	
	/// \brief Return the length (magnitude) of this vector.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v(3, 4);
	/// float len = v.length(); // len is 5 (3,4,5 triangle)
	/// ~~~~
	/// 
	/// length() involves a square root calculation, which is one of the slowest things
	/// you can do in programming. If you don't need an exact number but rather just a
	/// rough idea of a length (for example when finding the shortest distance of a
	/// bunch of points to a reference point, where it doesn't matter exactly what the
	/// lengths are, you just want the shortest), you can use
	/// lengthSquared() instead.
	///
	/// \sa lengthSquared()
    float length() const;
    
	/// \brief Return the squared length (squared magnitude) of this vector.
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v(3, 4);
	/// float len = v.length(); // len is 5 (3,4,5 triangle)
	/// ~~~~
	/// 
	/// Use as a much faster alternative to length() if you don't
	/// need to know an accurate length but rather just a rough idea of a
	/// length (for example when finding the shortest distance of a bunch of
	/// points to a reference point, where it doesn't matter exactly what the
	/// lengths are, you just want the shortest). It avoids the square root
	/// calculation that is ordinarily required to calculate a length.
	///
	/// \sa length()
    float lengthSquared() const;

    /// \brief Calculate the angle to another vector in degrees
    ///
	/// ~~~~{.cpp}
	/// ofVec2f v1(1,0);
	/// ofVec2f v2(0,1);
	/// float angle = v1.angle(v2); // angle is 90
	/// ~~~~
	/// \param vec The vector to calculate the angle to
	/// \returns The angle in degrees (-180...180)
	float angle( const ofVec2f& vec ) const;

    /// \brief Calculate the angle to another vector in radians
    ///
	/// ~~~~{.cpp}
	/// ofVec2f v1(1,0);
	/// ofVec2f v2(0,1);
	/// float angle = v1.angleRad(v2); // angle is HALF_PI
	/// ~~~~
	/// \param vec The vector to calculate the angle to
	/// \returns The angle in radians (-PI...PI)
    float angleRad( const ofVec2f& vec ) const;
	
	/// \}

	//---------------------
	/// \name Perpendicular
	/// \{

	/// \brief Return the *normalized* ofVec2f that is perpendicular to this vector
	/// (ie rotated 90 degrees and normalized).
	/// 
	/// ![PERPENDICULAR](math/perpendicular.png)
	/// Image courtesy of Wikipedia
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v(2, 5);
	/// ofVec2f u = v.getPerpendicular(); // u is (0.928, -0.371)
	/// ~~~~
	/// 
	/// \sa perpendicular()
    ofVec2f  getPerpendicular() const;

	/// \brief Set this vector to its own **normalized** perpendicular (by
	/// rotating 90 degrees and normalizing).
	/// 
	/// ![PERPENDICULAR](math/perpendicular.png)
	/// Image courtesy of Wikipedia
	/// 
	/// ~~~~{.cpp}
	/// ofVec2f v(2, 5);
	/// v.perpendicular(); // v is (0.928, -0.371)
	/// ~~~~
	/// \sa getPerpendicular()
	ofVec2f& perpendicular();
	
	/// \brief Calculate and return the dot product of this vector with vec.
	/// 
	/// *Dot product* (less commonly known as *Euclidean inner product*) expresses 
	/// the angular relationship between two vectors. In other words it is a measure 
	/// of how *parallel* two vectors are. If they are completely perpendicular the dot 
	/// product is 0; if they are completely parallel their dot product is either 1 if 
	/// they are pointing in the same direction, or -1 if they are pointing in 
	/// opposite directions.
	///
	/// ![DOT](math/dotproduct.png)
	/// Image courtesy of Wikipedia
	/// 
	/// ~~~~{.cpp}
	/// ofvec2f a1(1, 0);
	/// ofVec2f b1(0, 1); // 90 degree angle to a1
	/// dot = a1.dot(b1); // dot is 0, ie cos(90)
	/// 
	/// ofVec2f a2(1, 0); 
	/// ofVec2f b2(1, 1); // 45 degree angle to a2
	/// b2.normalize(); // vectors should to be unit vectors (normalized)
	/// float dot = a2.dot(b2); // dot is 0.707, ie cos(45)
	/// 
	/// ofVec2f a3(1, 0);
	/// ofVec2f b3(-1, 0); // 180 degree angle to a3
	/// dot = a3.dot(b3); // dot is -1, ie cos(180)
	/// ~~~~
	/// 
	/// \param vec The vector to dotproduct
    float dot( const ofVec2f& vec ) const;
	
	
	/// \}



    //---------------------------------------------------
    // this methods are deprecated in 006 please dont use:
	/// \cond INTERNAL

    // getScaled
    OF_DEPRECATED_MSG("Use member method getScaled() instead.", ofVec2f rescaled( const float length ) const);
	
    // scale
    OF_DEPRECATED_MSG("Use member method scale() instead.", ofVec2f& rescale( const float length ));
	
    // getRotated
    OF_DEPRECATED_MSG("Use member method getRotated() instead.", ofVec2f rotated( float angle ) const);
	
    // getNormalized
    OF_DEPRECATED_MSG("Use member method getNormalized() instead.", ofVec2f normalized() const);
	
    // getLimited
    OF_DEPRECATED_MSG("Use member method getLimited() instead.", ofVec2f limited(float max) const);
	
    // getPerpendicular
    OF_DEPRECATED_MSG("Use member method getPerpendicular() instead.", ofVec2f perpendiculared() const);
	
    // getInterpolated
    OF_DEPRECATED_MSG("Use member method getInterpolated() instead.", ofVec2f interpolated( const ofVec2f& pnt, float p ) const);
    
    // getMiddled
    OF_DEPRECATED_MSG("Use member method getMiddled() instead.", ofVec2f middled( const ofVec2f& pnt ) const);
    
    // getMapped 
    OF_DEPRECATED_MSG("Use member method getMapped() instead.", ofVec2f mapped( const ofVec2f& origin, const ofVec2f& vx, const ofVec2f& vy ) const);
    
    // squareDistance
    OF_DEPRECATED_MSG("Use member method squareDistance() instead.", float distanceSquared( const ofVec2f& pnt ) const);
    
    // use getRotated
    OF_DEPRECATED_MSG("Use member method getRotated() instead.", ofVec2f rotated( float angle, const ofVec2f& pivot ) const);    
    
    // return all zero vector
    static ofVec2f zero() { return ofVec2f(0, 0); }

    // return all one vector
    static ofVec2f one() { return ofVec2f(1, 1); }

    /// \endcond
};



/// \cond INTERNAL

// Non-Member operators
//
ofVec2f operator+( float f, const ofVec2f& vec );
ofVec2f operator-( float f, const ofVec2f& vec );
ofVec2f operator*( float f, const ofVec2f& vec );
ofVec2f operator/( float f, const ofVec2f& vec );


/// \endcond




/////////////////
// Implementation
/////////////////
/// \cond INTERNAL


inline ofVec2f::ofVec2f(): x(0), y(0) {}
inline ofVec2f::ofVec2f( float _scalar ): x(_scalar), y(_scalar) {}
inline ofVec2f::ofVec2f( float _x, float _y ):x(_x), y(_y) {}

// Getters and Setters.
//
//
inline void ofVec2f::set( float _scalar ) {
	x = _scalar;
	y = _scalar;
}

inline void ofVec2f::set( float _x, float _y ) {
	x = _x;
	y = _y;
}

inline void ofVec2f::set( const ofVec2f& vec ) {
	x = vec.x;
	y = vec.y;
}


// Check similarity/equality.
//
//
inline bool ofVec2f::operator==( const ofVec2f& vec ) const {
	return (x == vec.x) && (y == vec.y);
}

inline bool ofVec2f::operator!=( const ofVec2f& vec ) const {
	return (x != vec.x) || (y != vec.y);
}

inline bool ofVec2f::match( const ofVec2f& vec, float tolerance ) const {
	return (fabs(x - vec.x) < tolerance)
	&& (fabs(y - vec.y) < tolerance);
}

//
// Checks if vectors look in the same direction.
// Tolerance is specified in degree.
 
inline bool ofVec2f::isAligned( const ofVec2f& vec, float tolerance ) const { 
	return  fabs( this->angle( vec ) ) < tolerance;
}
inline bool ofVec2f::align( const ofVec2f& vec, float tolerance ) const {
    return isAligned( vec, tolerance );
}

inline bool ofVec2f::isAlignedRad( const ofVec2f& vec, float tolerance ) const {
	return  fabs( this->angleRad( vec ) ) < tolerance;
}
inline bool ofVec2f::alignRad( const ofVec2f& vec, float tolerance ) const {
    return isAlignedRad( vec, tolerance );
}


// Overloading for any type to any type
//
//

inline ofVec2f ofVec2f::operator+( const ofVec2f& vec ) const {
	return ofVec2f( x+vec.x, y+vec.y);
}

inline ofVec2f& ofVec2f::operator+=( const ofVec2f& vec ) {
	x += vec.x;
	y += vec.y;
	return *this;
}

inline ofVec2f ofVec2f::operator-( const ofVec2f& vec ) const {
	return ofVec2f(x-vec.x, y-vec.y);
}

inline ofVec2f& ofVec2f::operator-=( const ofVec2f& vec ) {
	x -= vec.x;
	y -= vec.y;
	return *this;
}

inline ofVec2f ofVec2f::operator*( const ofVec2f& vec ) const {
	return ofVec2f(x*vec.x, y*vec.y);
}

inline ofVec2f& ofVec2f::operator*=( const ofVec2f& vec ) {
	x*=vec.x;
	y*=vec.y;
	return *this;
}

inline ofVec2f ofVec2f::operator/( const ofVec2f& vec ) const {
	return ofVec2f( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y);
}

inline ofVec2f& ofVec2f::operator/=( const ofVec2f& vec ) {
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	return *this;
}

inline ostream& operator<<(ostream& os, const ofVec2f& vec) {
	os << vec.x << ", " << vec.y;
	return os;
}

inline istream& operator>>(istream& is, ofVec2f& vec) {
	is >> vec.x;
	is.ignore(2);
	is >> vec.y;
	return is;
}

//operator overloading for float
//
//
//inline void ofVec2f::operator=( const float f){
//	x = f;
//	y = f;
//}

inline ofVec2f ofVec2f::operator+( const float f ) const {
	return ofVec2f( x+f, y+f);
}

inline ofVec2f& ofVec2f::operator+=( const float f ) {
	x += f;
	y += f;
	return *this;
}

inline ofVec2f ofVec2f::operator-( const float f ) const {
	return ofVec2f( x-f, y-f);
}

inline ofVec2f& ofVec2f::operator-=( const float f ) {
	x -= f;
	y -= f;
	return *this;
}

inline ofVec2f ofVec2f::operator-() const {
	return ofVec2f(-x, -y);
}

inline ofVec2f ofVec2f::operator*( const float f ) const {
	return ofVec2f(x*f, y*f);
}

inline ofVec2f& ofVec2f::operator*=( const float f ) {
	x*=f;
	y*=f;
	return *this;
}

inline ofVec2f ofVec2f::operator/( const float f ) const {
	if(f == 0) return ofVec2f(x, y);
	
	return ofVec2f(x/f, y/f);
}

inline ofVec2f& ofVec2f::operator/=( const float f ) {
	if(f == 0) return *this;
	
	x/=f;
	y/=f;
	return *this;
}

inline ofVec2f ofVec2f::rescaled( const float length ) const {
	return getScaled(length);
}

inline ofVec2f ofVec2f::getScaled( const float length ) const {
	float l = (float)sqrt(x*x + y*y);
	if( l > 0 )
		return ofVec2f( (x/l)*length, (y/l)*length );
	else
		return ofVec2f();
}

inline ofVec2f& ofVec2f::rescale( const float length ){
	return scale(length);
}

inline ofVec2f& ofVec2f::scale( const float length ) {
	float l = (float)sqrt(x*x + y*y);
	if (l > 0) {
		x = (x/l)*length;
		y = (y/l)*length;
	}
	return *this;
}



// Rotation
//
//
inline ofVec2f ofVec2f::rotated( float angle ) const {
	return getRotated(angle);
}

inline ofVec2f ofVec2f::getRotated( float angle ) const {
	float a = (float)(angle*DEG_TO_RAD);
	return ofVec2f( x*cos(a) - y*sin(a),
				   x*sin(a) + y*cos(a) );
}

inline ofVec2f ofVec2f::getRotatedRad( float angle ) const {
	float a = angle;
	return ofVec2f( x*cos(a) - y*sin(a),
				   x*sin(a) + y*cos(a) );
}

inline ofVec2f& ofVec2f::rotate( float angle ) {
	float a = (float)(angle * DEG_TO_RAD);
	float xrot = x*cos(a) - y*sin(a);
	y = x*sin(a) + y*cos(a);
	x = xrot;
	return *this;
}

inline ofVec2f& ofVec2f::rotateRad( float angle ) {
	float a = angle;
	float xrot = x*cos(a) - y*sin(a);
	y = x*sin(a) + y*cos(a);
	x = xrot;
	return *this;
}



// Rotate point by angle (deg) around pivot point.
//
//

// This method is deprecated in 006 please use getRotated instead
inline ofVec2f ofVec2f::rotated( float angle, const ofVec2f& pivot ) const {
	return getRotated(angle, pivot);
}

inline ofVec2f ofVec2f::getRotated( float angle, const ofVec2f& pivot ) const {
	float a = (float)(angle * DEG_TO_RAD);
	return ofVec2f( ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x,
				   ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y );
}

inline ofVec2f& ofVec2f::rotate( float angle, const ofVec2f& pivot ) {
	float a = (float)(angle * DEG_TO_RAD);
	float xrot = ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x;
	y = ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y;
	x = xrot;
	return *this;
}

inline ofVec2f ofVec2f::getRotatedRad( float angle, const ofVec2f& pivot ) const {
	float a = angle;
	return ofVec2f( ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x,
				   ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y );
}

inline ofVec2f& ofVec2f::rotateRad( float angle, const ofVec2f& pivot ) {
	float a = angle;
	float xrot = ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x;
	y = ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y;
	x = xrot;
	return *this;
}



// Map point to coordinate system defined by origin, vx, and vy.
//
//

// This method is deprecated in 006 please use getMapped instead
inline ofVec2f ofVec2f::mapped( const ofVec2f& origin,
							   const ofVec2f& vx,
							   const ofVec2f& vy ) const{
	return getMapped(origin, vx, vy);
}

inline ofVec2f ofVec2f::getMapped( const ofVec2f& origin,
								  const ofVec2f& vx,
								  const ofVec2f& vy ) const
{
	return ofVec2f( origin.x + x*vx.x + y*vy.x,
				   origin.y + x*vx.y + y*vy.y );
}

inline ofVec2f& ofVec2f::map( const ofVec2f& origin,
							 const ofVec2f& vx, const ofVec2f& vy )
{
	float xmap = origin.x + x*vx.x + y*vy.x;
	y = origin.y + x*vx.y + y*vy.y;
	x = xmap;
	return *this;
}


// Distance between two points.
//
//
inline float ofVec2f::distance( const ofVec2f& pnt) const {
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	return (float)sqrt(vx*vx + vy*vy);
}

//this method is deprecated in 006 please use squareDistance
inline float ofVec2f::distanceSquared( const ofVec2f& pnt ) const {
	return squareDistance(pnt);
}

inline float ofVec2f::squareDistance( const ofVec2f& pnt ) const {
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	return vx*vx + vy*vy;
}



// Linear interpolation.
//
//
//
// p==0.0 results in this point, p==0.5 results in the
// midpoint, and p==1.0 results in pnt being returned.
//

// this method is deprecated in 006 please use getInterpolated
inline ofVec2f ofVec2f::interpolated( const ofVec2f& pnt, float p ) const{
	return getInterpolated(pnt, p);
}

inline ofVec2f ofVec2f::getInterpolated( const ofVec2f& pnt, float p ) const {
	return ofVec2f( x*(1-p) + pnt.x*p, y*(1-p) + pnt.y*p );
}

inline ofVec2f& ofVec2f::interpolate( const ofVec2f& pnt, float p ) {
	x = x*(1-p) + pnt.x*p;
	y = y*(1-p) + pnt.y*p;
	return *this;
}

// this method is deprecated in 006 please use getMiddle
inline ofVec2f ofVec2f::middled( const ofVec2f& pnt ) const{
	return getMiddle(pnt);
}

inline ofVec2f ofVec2f::getMiddle( const ofVec2f& pnt ) const {
	return ofVec2f( (x+pnt.x)/2.0f, (y+pnt.y)/2.0f );
}

inline ofVec2f& ofVec2f::middle( const ofVec2f& pnt ) {
	x = (x+pnt.x)/2.0f;
	y = (y+pnt.y)/2.0f;
	return *this;
}





inline ofVec2f& ofVec2f::average( const ofVec2f* points, int num ) {
	x = 0.f;
	y = 0.f;
	for( int i=0; i<num; i++) {
		x += points[i].x;
		y += points[i].y;
	}
	x /= num;
	y /= num;
	return *this;
}



// Normalization
//
//
inline ofVec2f ofVec2f::normalized() const {
	return getNormalized();
}

inline ofVec2f ofVec2f::getNormalized() const {
	float length = (float)sqrt(x*x + y*y);
	if( length > 0 ) {
		return ofVec2f( x/length, y/length );
	} else {
		return ofVec2f();
	}
}

inline ofVec2f& ofVec2f::normalize() {
	float length = (float)sqrt(x*x + y*y);
	if( length > 0 ) {
		x /= length;
		y /= length;
	}
	return *this;
}



// Limit length.
//
//
inline ofVec2f ofVec2f::limited(float max) const{
	return getLimited(max);
}

inline ofVec2f ofVec2f::getLimited(float max) const {
    ofVec2f limited;
    float lengthSquared = (x*x + y*y);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        float ratio = max/(float)sqrt(lengthSquared);
        limited.set( x*ratio, y*ratio);
    } else {
        limited.set(x,y);
    }
    return limited;
}

inline ofVec2f& ofVec2f::limit(float max) {
    float lengthSquared = (x*x + y*y);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        float ratio = max/(float)sqrt(lengthSquared);
        x *= ratio;
        y *= ratio;
    }
    return *this;
}



// Perpendicular normalized vector.
//
//
inline ofVec2f ofVec2f::perpendiculared() const {
	return getPerpendicular();
}

inline ofVec2f ofVec2f::getPerpendicular() const {
	float length = (float)sqrt( x*x + y*y );
	if( length > 0 )
		return ofVec2f( -(y/length), x/length );
	else
		return ofVec2f();
}

inline ofVec2f& ofVec2f::perpendicular() {
	float length = (float)sqrt( x*x + y*y );
	if( length > 0 ) {
		float _x = x;
		x = -(y/length);
		y = _x/length;
	}
	return *this;
}


// Length
//
//
inline float ofVec2f::length() const {
	return (float)sqrt( x*x + y*y );
}

inline float ofVec2f::lengthSquared() const {
	return (float)(x*x + y*y);
}


inline float ofVec2f::angle( const ofVec2f& vec ) const {
	return (float)(atan2( x*vec.y-y*vec.x, x*vec.x + y*vec.y )*RAD_TO_DEG);
}

inline float ofVec2f::angleRad( const ofVec2f& vec ) const {
	return atan2( x*vec.y-y*vec.x, x*vec.x + y*vec.y );
}


inline float ofVec2f::dot( const ofVec2f& vec ) const {
	return x*vec.x + y*vec.y;
}







// Non-Member operators
//
//
inline ofVec2f operator+( float f, const ofVec2f& vec ) {
    return ofVec2f( f+vec.x, f+vec.y);
}

inline ofVec2f operator-( float f, const ofVec2f& vec ) {
    return ofVec2f( f-vec.x, f-vec.y);
}

inline ofVec2f operator*( float f, const ofVec2f& vec ) {
    return ofVec2f( f*vec.x, f*vec.y);
}

inline ofVec2f operator/( float f, const ofVec2f& vec ) {
    return ofVec2f( f/vec.x, f/vec.y);
}


/// \endcond