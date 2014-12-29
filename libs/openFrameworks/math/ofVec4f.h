#pragma once

class ofVec2f;
class ofVec3f;
#include "ofConstants.h"

class ofVec4f {
public:
    /// \cond INTERNAL
    static const int DIM = 4;
    /// \endcond
	
	float x;
	float y;
	float z;
	float w;

	//---------------------
	/// \name Construct a 4D vector
	/// \{
    
	ofVec4f();
	explicit ofVec4f( float _scalar );
	ofVec4f( float _x, float _y, float _z, float _w );
	ofVec4f( const ofVec2f& vec);
	ofVec4f( const ofVec3f& vec);

    /// \}

	//---------------------
	/// \name Access components
	/// \{

	
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

	void set( float _scalar );
    void set( float _x, float _y, float _z, float _w );
    void set( const ofVec4f& vec );
	
	
	/// \}

    //---------------------
	/// \name Comparison 
	/// \{


    bool operator==( const ofVec4f& vec ) const;
    bool operator!=( const ofVec4f& vec ) const;
    bool match( const ofVec4f& vec, float tolerance=0.0001) const;
	
	/// \}

	//---------------------
	/// \name Operators
	/// \{

    ofVec4f  operator+( const ofVec4f& vec ) const;
    ofVec4f  operator+( const float f ) const;
    ofVec4f& operator+=( const ofVec4f& vec );
    ofVec4f& operator+=( const float f );
    ofVec4f  operator-( const float f ) const;
    ofVec4f  operator-( const ofVec4f& vec ) const;
    ofVec4f  operator-() const;
    ofVec4f& operator-=( const float f );
    ofVec4f& operator-=( const ofVec4f& vec );
	
	
    ofVec4f  operator*( const ofVec4f& vec ) const;
    ofVec4f  operator*( const float f ) const;
    ofVec4f& operator*=( const ofVec4f& vec );
    ofVec4f& operator*=( const float f );
    ofVec4f  operator/( const ofVec4f& vec ) const;
    ofVec4f  operator/( const float f ) const;
    ofVec4f& operator/=( const ofVec4f& vec );
    ofVec4f& operator/=( const float f );
	    
	/// \cond INTERNAL
	friend ostream& operator<<(ostream& os, const ofVec4f& vec);
	friend istream& operator>>(istream& is, const ofVec4f& vec);
	/// \endcond

    /// \}
	
	//---------------------
	/// \name Simple manipulations
	/// \{

    /// \brief Returns a new ofVec4f that is the result of scaling this vector up or down so that it has the requested length.
    ///
    /// \param length The desired length of the new ofVec4f object.
    /// \returns The result of scaling the this vector up or down.
    ofVec4f  getScaled( const float length ) const;
    
    /// \brief Scales this vector up or down so that it has the requested length.
    ///
    /// \param length The desired length of the vector.
    ofVec4f& scale( const float length );
	
	/// \}
	
	
	//---------------------
	/// \name Distance
	/// \{

    /// \brief Treats this vector and 'pnt' as points in 4D space and calculates the distance between them.
    ///
    /// \param pnt The vector used in the distance calculation with the current vector.
    /// \returns The distance between the two vectors in 4D space.
    float distance( const ofVec4f& pnt) const;
    float squareDistance( const ofVec4f& pnt ) const;
	
	/// \}

	//---------------------
	/// \name Interpolation
	/// \{


    /// \brief Performs a linear interpolation of this vector towards 'pnt'.
    ///
    /// \param pnt The vector the interpolation will be performed on. 
    /// \param p The amount to move towards 'pnt'; 'p' is normally between 0 and 1 and where 0 means stay the original position and 1 means move all the way to 'pnt', but you can also have 'p' greater than 1 overshoot 'pnt', or less than 0 to move backwards away from 'pnt'.
    /// \returns The interpolation as an ofVec4f. 
    ofVec4f   getInterpolated( const ofVec4f& pnt, float p ) const;
    
    /// \brief Performs a linear interpolation of this vector towards 'pnt'. This modifies the current vector to the interpolated value.
    ///
    /// \param pnt The vector the interpolation will be performed on. 
    /// \param p The amount to move towards 'pnt'; 'p' is normally between 0 and 1 and where 0 means stay the original position and 1 means move all the way to 'pnt', but you can also have 'p' greater than 1 overshoot 'pnt', or less than 0 to move backwards away from 'pnt'.
    ofVec4f&  interpolate( const ofVec4f& pnt, float p );
    
    /// \brief Calculates and returns the midpoint (as a vector) between this vector and 'pnt'.
    ///
    /// \param pnt The vector used in the midpoint calculation with this vector.
    /// \returns The midpoint between this vector and 'pnt' as an ofVec4f.
    ofVec4f   getMiddle( const ofVec4f& pnt ) const;
    
    /// \brief Calculates and returns the midpoint (as a vector) between this vector and 'pnt'. This modifies the current vector to the midpoint value.
    ///
    /// \param pnt The vector used in the midpoint calculation with this vector.
    /// \returns The midpoint between this vector and 'pnt' as an ofVec4f.
    ofVec4f&  middle( const ofVec4f& pnt );
    
    /// \brief Sets this vector to be the average (center of gravity or centroid) of a given array of 'ofVec4f's.
    /// 
    /// \param points The array of 'ofVec4f's used in the average calculation.
    /// \param num The number of ofVec4f objects in the array.
    ofVec4f&  average( const ofVec4f* points, int num );

    /// \}

    //---------------------
	/// \name Limit
	/// \{
    
    /// \brief Returns a normalized copy of this vector.
    ///
    /// Normalization means to scale the vector so that its length (magnitude) is exactly 1, 
    /// at which stage all that is left is the direction. A normalized vector is usually called 
    /// a unit vector, and can be used to represent a pure direction (heading).
    ///
    /// \returns The normalized copy of the current vector.
    ofVec4f  getNormalized() const;
    
    /// \brief Normalizes the vector. This changes the current vector to its normalized value. 
    ///
    /// Normalization means to scale the vector so that its length (magnitude) is exactly 1, 
    /// at which stage all that is left is the direction. A normalized vector is usually called 
    /// a unit vector, and can be used to represent a pure direction (heading).
    ofVec4f& normalize();
	
	
    /// \brief Returns a copy of this vector with its length (magnitude) restricted to a maximum of 'max' units by scaling down if necessary.
    ///
    /// \param max The maximum length of the new vector. 
    /// \returns A copy of the current vector that is at most 'max' units long. 
    ofVec4f  getLimited(float max) const;
    /// \brief Restrict the length (magnitude) of this vector to a maximum of 'max' units by scaling down if necessary.
    ///
    /// \param max The maximum length of the current vector.
    ofVec4f& limit(float max);
	
	/// \}

	//---------------------
	/// \name Measurement
	/// \{
	
    /// \brief Returns the length (magnitude) of this vector.
    ///
    /// \returns The magnitude of the current vector. 
    float length() const;
    float lengthSquared() const;


  	/// \}

	//---------------------
	/// \name Calculations
	/// \{


    /// \brief Calculates and returns the dot product of this vector with 'vec'.
    ///
    /// Dot product (less commonly known as Euclidean inner product) expresses the angular 
    /// relationship between two vectors. In other words it is a measure of how parallel two vectors 
    /// are. If they are completely perpendicular the dot product is 0; if they are completely parallel 
    /// their dot product is either 1 if they are pointing in the same direction, or -1 if they are pointing 
    /// in opposite directions.
    /// 
    /// \param vec The vector used in the dot product calculation with this vector.
    /// \returns The dot product of this vector with 'vec'. 
    float dot( const ofVec4f& vec ) const;
		
	/// \}

	
	
	
    //---------------------------------------
    // this methods are deprecated in 006 please use:
	/// \cond INTERNAL
	
    // getScaled
    OF_DEPRECATED_MSG("Use member method getScaled() instead.", ofVec4f rescaled( const float length ) const);
	
    // scale
    OF_DEPRECATED_MSG("Use member method scale() instead.", ofVec4f& rescale( const float length ));
	
    // getNormalized
    OF_DEPRECATED_MSG("Use member method getNormalized() instead.", ofVec4f normalized() const);
	
    // getLimited
    OF_DEPRECATED_MSG("Use member method getLimited() instead.", ofVec4f limited(float max) const);
	
    // use squareDistance
    OF_DEPRECATED_MSG("Use member method squareDistance() instead.", float  distanceSquared( const ofVec4f& pnt ) const);
	
    // use getInterpolated
    OF_DEPRECATED_MSG("Use member method getInterpolated() instead.", ofVec4f interpolated( const ofVec4f& pnt, float p ) const);
	
    // use getMiddle
    OF_DEPRECATED_MSG("Use member method getMiddle() instead.", ofVec4f middled( const ofVec4f& pnt ) const);
    
    // return all zero vector
    static ofVec4f zero() { return ofVec4f(0, 0, 0, 0); }
    
    // return all one vector
    static ofVec4f one() { return ofVec4f(1, 1, 1, 1); }
    /// \endcond
};


/// \cond INTERNAL


// Non-Member operators
//
//
ofVec4f operator+( float f, const ofVec4f& vec );
ofVec4f operator-( float f, const ofVec4f& vec );
ofVec4f operator*( float f, const ofVec4f& vec );
ofVec4f operator/( float f, const ofVec4f& vec );







/////////////////
// Implementation
/////////////////

inline ofVec4f::ofVec4f(): x(0), y(0), z(0), w(0) {}
inline ofVec4f::ofVec4f(float _s): x(_s), y(_s), z(_s), w(_s) {}
inline ofVec4f::ofVec4f( float _x,
						float _y,
						float _z,
						float _w ):x(_x), y(_y), z(_z), w(_w) {}

// Getters and Setters.
//
//
inline void ofVec4f::set( float _scalar) {
	x = _scalar;
	y = _scalar;
	z = _scalar;
	w = _scalar;
}

inline void ofVec4f::set( float _x, float _y, float _z, float _w ) {
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

inline void ofVec4f::set( const ofVec4f& vec ) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}


// Check similarity/equality.
//
//
inline bool ofVec4f::operator==( const ofVec4f& vec ) const {
	return (x == vec.x) && (y == vec.y) && (z == vec.z) && (w == vec.w);
}

inline bool ofVec4f::operator!=( const ofVec4f& vec ) const {
	return (x != vec.x) || (y != vec.y) || (z != vec.z) || (w != vec.w);
}

inline bool ofVec4f::match( const ofVec4f& vec, float tolerance) const {
	return (fabs(x - vec.x) < tolerance)
	&& (fabs(y - vec.y) < tolerance)
	&& (fabs(z - vec.z) < tolerance)
	&& (fabs(w - vec.w) < tolerance);
}




// Additions and Subtractions.
//
//
inline ofVec4f ofVec4f::operator+( const ofVec4f& vec ) const {
	return ofVec4f( x+vec.x, y+vec.y, z+vec.z, w+vec.w);
}

inline ofVec4f& ofVec4f::operator+=( const ofVec4f& vec ) {
	x += vec.x;
	y += vec.y;
	z += vec.z;
	w += vec.w;
	return *this;
}

inline ofVec4f ofVec4f::operator-( const float f ) const {
	return ofVec4f( x-f, y-f, z-f, w-f );
}

inline ofVec4f& ofVec4f::operator-=( const float f ) {
	x -= f;
	y -= f;
	z -= f;
	w -= f;
	return *this;
}

inline ofVec4f ofVec4f::operator-( const ofVec4f& vec ) const {
	return ofVec4f( x-vec.x, y-vec.y, z-vec.z, w-vec.w );
}

inline ofVec4f& ofVec4f::operator-=( const ofVec4f& vec ) {
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	w -= vec.w;
	return *this;
}

inline ofVec4f ofVec4f::operator+( const float f ) const {
	return ofVec4f( x+f, y+f, z+f, w+f );
}

inline ofVec4f& ofVec4f::operator+=( const float f ) {
	x += f;
	y += f;
	z += f;
	w += f;
	return *this;
}

inline ofVec4f ofVec4f::operator-() const {
	return ofVec4f( -x, -y, -z, -w );
}


// Scalings
//
//
inline ofVec4f ofVec4f::operator*( const ofVec4f& vec ) const {
	return ofVec4f( x*vec.x, y*vec.y, z*vec.z, w*vec.w );
}

inline ofVec4f& ofVec4f::operator*=( const ofVec4f& vec ) {
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
	w *= vec.w;
	return *this;
}

inline ofVec4f ofVec4f::operator*( const float f ) const {
	return ofVec4f( x*f, y*f, z*f, w*f );
}

inline ofVec4f& ofVec4f::operator*=( const float f ) {
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

inline ofVec4f ofVec4f::operator/( const ofVec4f& vec ) const {
	return ofVec4f( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y, vec.z!=0 ? z/vec.z : z, vec.w!=0 ? w/vec.w : w  );
}

inline ofVec4f& ofVec4f::operator/=( const ofVec4f& vec ) {
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	vec.z!=0 ? z/=vec.z : z;
	vec.w!=0 ? w/=vec.w : w;
	return *this;
}

inline ofVec4f ofVec4f::operator/( const float f ) const {
	if(f == 0) return ofVec4f(x, y, z, w);
	
	return ofVec4f( x/f, y/f, z/f, w/f );
}

inline ofVec4f& ofVec4f::operator/=( const float f ) {
	if(f == 0)return *this;
	
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this;
}


inline ostream& operator<<(ostream& os, const ofVec4f& vec) {
	os << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
	return os;
}

inline istream& operator>>(istream& is, ofVec4f& vec) {
	is >> vec.x;
	is.ignore(2);
	is >> vec.y;
	is.ignore(2);
	is >> vec.z;
	is.ignore(2);
	is >> vec.w;
	return is;
}


inline ofVec4f ofVec4f::rescaled( const float length ) const {
	return getScaled(length);
}

inline ofVec4f ofVec4f::getScaled( const float length ) const {
	float l = (float)sqrt(x*x + y*y + z*z + w*w);
	if( l > 0 )
		return ofVec4f( (x/l)*length, (y/l)*length,
					   (z/l)*length, (w/l)*length );
	else
		return ofVec4f();
}

inline ofVec4f& ofVec4f::rescale( const float length ) {
	return scale(length);
}

inline ofVec4f& ofVec4f::scale( const float length ) {
	float l = (float)sqrt(x*x + y*y + z*z + w*w);
	if (l > 0) {
		x = (x/l)*length;
		y = (y/l)*length;
		z = (z/l)*length;
		w = (w/l)*length;
	}
	return *this;
}



// Distance between two points.
//
//
inline float ofVec4f::distance( const ofVec4f& pnt) const {
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	float vz = z-pnt.z;
	float vw = w-pnt.w;
	return (float)sqrt( vx*vx + vy*vy + vz*vz + vw*vw );
}

inline float ofVec4f::distanceSquared( const ofVec4f& pnt ) const {
	return squareDistance(pnt);
}

inline float ofVec4f::squareDistance( const ofVec4f& pnt ) const {
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	float vz = z-pnt.z;
	float vw = w-pnt.w;
	return vx*vx + vy*vy + vz*vz + vw*vw;
}



// Linear interpolation.
//
//
/**
 * p==0.0 results in this point, p==0.5 results in the
 * midpoint, and p==1.0 results in pnt being returned.
 */
inline ofVec4f ofVec4f::interpolated( const ofVec4f& pnt, float p ) const{
	return getInterpolated(pnt,p);
}

inline ofVec4f ofVec4f::getInterpolated( const ofVec4f& pnt, float p ) const {
	return ofVec4f( x*(1-p) + pnt.x*p,
				   y*(1-p) + pnt.y*p,
				   z*(1-p) + pnt.z*p,
				   w*(1-p) + pnt.w*p );
}

inline ofVec4f& ofVec4f::interpolate( const ofVec4f& pnt, float p ) {
	x = x*(1-p) + pnt.x*p;
	y = y*(1-p) + pnt.y*p;
	z = z*(1-p) + pnt.z*p;
	w = w*(1-p) + pnt.w*p;
	return *this;
}

inline ofVec4f ofVec4f::middled( const ofVec4f& pnt ) const {
	return getMiddle(pnt);
}

inline ofVec4f ofVec4f::getMiddle( const ofVec4f& pnt ) const {
	return ofVec4f( (x+pnt.x)/2.0f, (y+pnt.y)/2.0f,
				   (z+pnt.z)/2.0f, (w+pnt.w)/2.0f );
}

inline ofVec4f& ofVec4f::middle( const ofVec4f& pnt ) {
	x = (x+pnt.x)/2.0f;
	y = (y+pnt.y)/2.0f;
	z = (z+pnt.z)/2.0f;
	w = (w+pnt.w)/2.0f;
	return *this;
}


// Average (centroid) among points.
// (Addition is sometimes useful for calculating averages too)
//
//
inline ofVec4f& ofVec4f::average( const ofVec4f* points, int num ) {
	x = 0.f;
	y = 0.f;
	z = 0.f;
	w = 0.f;
	for( int i=0; i<num; i++) {
		x += points[i].x;
		y += points[i].y;
		z += points[i].z;
		w += points[i].w;
	}
	x /= num;
	y /= num;
	z /= num;
	w /= num;
	return *this;
}



// Normalization
//
//
inline ofVec4f ofVec4f::normalized() const {
	return getNormalized();
}

inline ofVec4f ofVec4f::getNormalized() const {
	float length = (float)sqrt(x*x + y*y + z*z + w*w);
	if( length > 0 ) {
		return ofVec4f( x/length, y/length, z/length, w/length );
	} else {
		return ofVec4f();
	}
}

inline ofVec4f& ofVec4f::normalize() {
	float lenght = (float)sqrt(x*x + y*y + z*z + w*w);
	if( lenght > 0 ) {
		x /= lenght;
		y /= lenght;
		z /= lenght;
		w /= lenght;
	}
	return *this;
}



// Limit length.
//
//
inline ofVec4f ofVec4f::limited(float max) const {
	return getLimited(max);
}

inline ofVec4f ofVec4f::getLimited(float max) const {
    ofVec4f limited;
    float lengthSquared = (x*x + y*y + z*z + w*w);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        float ratio = max/(float)sqrt(lengthSquared);
        limited.set( x*ratio, y*ratio, z*ratio, w*ratio );
    } else {
        limited.set(x,y,z,w);
    }
    return limited;
}

inline ofVec4f& ofVec4f::limit(float max) {
    float lengthSquared = (x*x + y*y + z*z + w*w);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        float ratio = max/(float)sqrt(lengthSquared);
        x *= ratio;
        y *= ratio;
        z *= ratio;
        w *= ratio;
    }
    return *this;
}



// Length
//
//
inline float ofVec4f::length() const {
	return (float)sqrt( x*x + y*y + z*z + w*w );
}

inline float ofVec4f::lengthSquared() const {
	return (float)(x*x + y*y + z*z + w*w);
}




/**
 * Dot Product.
 */
inline float ofVec4f::dot( const ofVec4f& vec ) const {
	return x*vec.x + y*vec.y + z*vec.z + w*vec.w;
}





// Non-Member operators
//
//
inline ofVec4f operator+( float f, const ofVec4f& vec ) {
    return ofVec4f( f+vec.x, f+vec.y, f+vec.z, f+vec.w );
}

inline ofVec4f operator-( float f, const ofVec4f& vec ) {
    return ofVec4f( f-vec.x, f-vec.y, f-vec.z, f-vec.w );
}

inline ofVec4f operator*( float f, const ofVec4f& vec ) {
    return ofVec4f( f*vec.x, f*vec.y, f*vec.z, f*vec.w );
}

inline ofVec4f operator/( float f, const ofVec4f& vec ) {
    return ofVec4f( f/vec.x, f/vec.y, f/vec.z, f/vec.w);
}


/// \endcond

