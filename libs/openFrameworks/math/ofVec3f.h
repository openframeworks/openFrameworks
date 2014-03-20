#pragma once

#include "ofVec2f.h"
#include "ofVec4f.h"
#include "ofConstants.h"

#include <cmath>
#include <iostream>

class ofVec3f {
public:
	float x,y,z;
    
    static const int DIM = 3;
	
	ofVec3f();
	ofVec3f( float _x, float _y, float _z=0 );
	/// assigns scalar to x, y, and z
	explicit ofVec3f( float _scalar );
	
    ofVec3f( const ofVec2f& vec );
    ofVec3f( const ofVec4f& vec );
	
	float * getPtr() {
		return (float*)&x;
	}
	const float * getPtr() const {
		return (const float *)&x;
	}
	
	float& operator[]( int n ){
		return getPtr()[n];
	}
	
	float operator[]( int n ) const {
		return getPtr()[n];
	}
	
    
	// Getters and Setters.
    //
    void set( float _x, float _y, float _z = 0 );
    void set( const ofVec3f& vec );
	void set( float _scalar );
	
    // Check similarity/equality.
    //
    bool operator==( const ofVec3f& vec ) const;
    bool operator!=( const ofVec3f& vec ) const;
    bool match( const ofVec3f& vec, float tolerance=0.0001 ) const;
    /**
	 * Checks if vectors look in the same direction.
	 */
    bool isAligned( const ofVec3f& vec, float tolerance=0.0001 ) const;
    bool align( const ofVec3f& vec, float tolerance=0.0001 ) const;
    bool isAlignedRad( const ofVec3f& vec, float tolerance=0.0001 ) const;
    bool alignRad( const ofVec3f& vec, float tolerance=0.0001 ) const;
	
	
    // Operator overloading for ofVec3f
    //
    ofVec3f  operator+( const ofVec3f& pnt ) const;
    ofVec3f& operator+=( const ofVec3f& pnt );
    ofVec3f  operator-( const ofVec3f& vec ) const;
    ofVec3f& operator-=( const ofVec3f& vec );
    ofVec3f  operator*( const ofVec3f& vec ) const;
    ofVec3f& operator*=( const ofVec3f& vec );
    ofVec3f  operator/( const ofVec3f& vec ) const;
    ofVec3f& operator/=( const ofVec3f& vec );
    ofVec3f  operator-() const;
	
    //operator overloading for float
    //
	//    void 	  operator=( const float f);
    ofVec3f  operator+( const float f ) const;
    ofVec3f& operator+=( const float f );
 	ofVec3f  operator-( const float f ) const;
    ofVec3f& operator-=( const float f );
    ofVec3f  operator*( const float f ) const;
    ofVec3f& operator*=( const float f );
    ofVec3f  operator/( const float f ) const;
    ofVec3f& operator/=( const float f );
	
	friend ostream& operator<<(ostream& os, const ofVec3f& vec);
	friend istream& operator>>(istream& is, ofVec3f& vec);
	
    //Scale
    //
    ofVec3f  getScaled( const float length ) const;
    ofVec3f& scale( const float length );
    
	
    // Rotation
    //
    ofVec3f  getRotated( float angle, const ofVec3f& axis ) const;
    ofVec3f  getRotatedRad( float angle, const ofVec3f& axis ) const;
    ofVec3f& rotate( float angle, const ofVec3f& axis );
    ofVec3f& rotateRad( float angle, const ofVec3f& axis );
    ofVec3f  getRotated(float ax, float ay, float az) const;
    ofVec3f  getRotatedRad(float ax, float ay, float az) const;
    ofVec3f& rotate(float ax, float ay, float az);
    ofVec3f& rotateRad(float ax, float ay, float az);
    
    
    // Rotation - point around pivot
    //    
    ofVec3f   getRotated( float angle, const ofVec3f& pivot, const ofVec3f& axis ) const;
    ofVec3f&  rotate( float angle, const ofVec3f& pivot, const ofVec3f& axis );
    ofVec3f   getRotatedRad( float angle, const ofVec3f& pivot, const ofVec3f& axis ) const;
    ofVec3f&  rotateRad( float angle, const ofVec3f& pivot, const ofVec3f& axis );    
	
	
    // Map point to coordinate system defined by origin, vx, vy, and vz.
    //
    ofVec3f getMapped( const ofVec3f& origin,
					  const ofVec3f& vx,
					  const ofVec3f& vy,
					  const ofVec3f& vz ) const;
    ofVec3f& map( const ofVec3f& origin,
				 const ofVec3f& vx,
				 const ofVec3f& vy,
				 const ofVec3f& vz );
	
	
    // Distance between two points.
    //
    float distance( const ofVec3f& pnt) const;
    float squareDistance( const ofVec3f& pnt ) const;
	
	
    // Linear interpolation.
    //
    /**
	 * p==0.0 results in this point, p==0.5 results in the
	 * midpoint, and p==1.0 results in pnt being returned.
	 */
    ofVec3f   getInterpolated( const ofVec3f& pnt, float p ) const;
    ofVec3f&  interpolate( const ofVec3f& pnt, float p );
    ofVec3f   getMiddle( const ofVec3f& pnt ) const;
    ofVec3f&  middle( const ofVec3f& pnt );
    ofVec3f&  average( const ofVec3f* points, int num );
    
	
    // Normalization
    //
    ofVec3f  getNormalized() const;
    ofVec3f& normalize();
	
	
    // Limit length.
    //
    ofVec3f  getLimited(float max) const;
    ofVec3f& limit(float max);
	
	
    // Perpendicular vector.
    //
    ofVec3f  getCrossed( const ofVec3f& vec ) const;
    ofVec3f& cross( const ofVec3f& vec );
    /**
	 * Normalized perpendicular.
	 */
    ofVec3f  getPerpendicular( const ofVec3f& vec ) const;
    ofVec3f& perpendicular( const ofVec3f& vec );
	
	
    // Length
    //
    float length() const;
    float lengthSquared() const;

    /**
	 * Angle (deg) between two vectors.
	 * This is an unsigned relative angle from 0 to 180.
	 * http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
	 */
    float angle( const ofVec3f& vec ) const;
    float angleRad( const ofVec3f& vec ) const;
	
	
    // Dot Product
    //
    float dot( const ofVec3f& vec ) const;
	
	
	
    //-----------------------------------------------
    // this methods are deprecated in 006 please use:
	
    // getScaled
    ofVec3f rescaled( const float length ) const;
	
    // scale
    ofVec3f& rescale( const float length );
	
    // getRotated
    ofVec3f rotated( float angle, const ofVec3f& axis ) const;
	
    // getRotated should this be const???
    ofVec3f rotated(float ax, float ay, float az);
	
    // getNormalized
    ofVec3f normalized() const;
	
    // getLimited
    ofVec3f limited(float max) const;
	
    // getCrossed
    ofVec3f crossed( const ofVec3f& vec ) const;
	
    // getPerpendicular
    ofVec3f perpendiculared( const ofVec3f& vec ) const;
    
    // use getMapped
    ofVec3f  mapped( const ofVec3f& origin,
					const ofVec3f& vx,
					const ofVec3f& vy,
					const ofVec3f& vz ) const;
	
    // use squareDistance
    float  distanceSquared( const ofVec3f& pnt ) const;
	
    // use getInterpolated
    ofVec3f 	interpolated( const ofVec3f& pnt, float p ) const;
	
    // use getMiddle
    ofVec3f 	middled( const ofVec3f& pnt ) const;
    
    // use getRotated
    ofVec3f 	rotated( float angle,
						const ofVec3f& pivot,
						const ofVec3f& axis ) const;    

    // return all zero vector
    static ofVec3f zero() { return ofVec3f(0, 0, 0); }
    
    // return all one vector
    static ofVec3f one() { return ofVec3f(1, 1, 1); }
};




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
inline ofVec3f::ofVec3f(): x(0), y(0), z(0) {};
inline ofVec3f::ofVec3f( float _all ): x(_all), y(_all), z(_all) {};
inline ofVec3f::ofVec3f( float _x, float _y, float _z ):x(_x), y(_y), z(_z) {}


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

inline ostream& operator<<(ostream& os, const ofVec3f& vec) {
	os << vec.x << ", " << vec.y << ", " << vec.z;
	return os;
}

inline istream& operator>>(istream& is, ofVec3f& vec) {
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
	ofVec3f ax = axis.normalized();
	float a = (float)(angle*DEG_TO_RAD);
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
	ofVec3f ax = axis.normalized();
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
	ofVec3f ax = axis.normalized();
	float a = (float)(angle*DEG_TO_RAD);
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
	ofVec3f ax = axis.normalized();
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
	float a = (float)cos(DEG_TO_RAD*(ax));
	float b = (float)sin(DEG_TO_RAD*(ax));
	float c = (float)cos(DEG_TO_RAD*(ay));
	float d = (float)sin(DEG_TO_RAD*(ay));
	float e = (float)cos(DEG_TO_RAD*(az));
	float f = (float)sin(DEG_TO_RAD*(az));
	
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
	float a = (float)cos(DEG_TO_RAD*(ax));
	float b = (float)sin(DEG_TO_RAD*(ax));
	float c = (float)cos(DEG_TO_RAD*(ay));
	float d = (float)sin(DEG_TO_RAD*(ay));
	float e = (float)cos(DEG_TO_RAD*(az));
	float f = (float)sin(DEG_TO_RAD*(az));
	
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
	ofVec3f ax = axis.normalized();
	float tx = x - pivot.x;
	float ty = y - pivot.y;
	float tz = z - pivot.z;
	
	float a = (float)(angle*DEG_TO_RAD);
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
	ofVec3f ax = axis.normalized();
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
	ofVec3f ax = axis.normalized();
	x -= pivot.x;
	y -= pivot.y;
	z -= pivot.z;
	
	float a = (float)(angle*DEG_TO_RAD);
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
	ofVec3f ax = axis.normalized();
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
	ofVec3f n1 = this->normalized();
	ofVec3f n2 = vec.normalized();
	return (float)(acos( n1.dot(n2) )*RAD_TO_DEG);
}

inline float ofVec3f::angleRad( const ofVec3f& vec ) const {
	ofVec3f n1 = this->normalized();
	ofVec3f n2 = vec.normalized();
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
