#ifndef _OFX_VEC4f
#define _OFX_VEC4f

#include "ofConstants.h"
#include "ofTypes.h"


class ofxVec4f : public ofPoint {


  public:

    float w;


    ofxVec4f( float _x=0.0f,
              float _y=0.0f,
              float _z=0.0f,
              float _w=0.0f );


    // Getters and Setters.
    //
    void set( float _x, float _y, float _z, float _w );
    void set( const ofxVec4f& vec );
    float &operator[]( const int& i );



    // Check similarity/equality.
    //
    bool operator==( const ofxVec4f& vec );
    bool operator!=( const ofxVec4f& vec );
    bool match( const ofxVec4f& vec, float tollerance=0.0001);


    // Additions and Subtractions.
    //
    ofxVec4f  operator+( const ofxVec4f& vec ) const;
    ofxVec4f& operator+=( const ofxVec4f& vec );
    ofxVec4f  operator-( const float f ) const;
    ofxVec4f& operator-=( const float f );
    ofxVec4f  operator-( const ofxVec4f& vec ) const;
    ofxVec4f& operator-=( const ofxVec4f& vec );
    ofxVec4f  operator+( const float f ) const;
    ofxVec4f& operator+=( const float f );
    ofxVec4f  operator-() const;


    // Scalings
    //
    ofxVec4f  operator*( const ofxVec4f& vec ) const;
    ofxVec4f& operator*=( const ofxVec4f& vec );
    ofxVec4f  operator*( const float f ) const;
    ofxVec4f& operator*=( const float f );
    ofxVec4f  operator/( const ofxVec4f& vec ) const;
    ofxVec4f& operator/=( const ofxVec4f& vec );
    ofxVec4f  operator/( const float f ) const;
    ofxVec4f& operator/=( const float f );
    ofxVec4f  getScaled( const float length ) const;
    ofxVec4f& scale( const float length );


    // Distance between two points.
    //
    float distance( const ofxVec4f& pnt) const;
    float squareDistance( const ofxVec4f& pnt ) const;


    // Linear interpolation.
    //
    /**
    * p==0.0 results in this point, p==0.5 results in the
    * midpoint, and p==1.0 results in pnt being returned.
    */
    ofxVec4f   getInterpolated( const ofxVec4f& pnt, float p ) const;
    ofxVec4f&  interpolate( const ofxVec4f& pnt, float p );
    ofxVec4f   getMiddle( const ofxVec4f& pnt ) const;
    ofxVec4f&  middle( const ofxVec4f& pnt );
    ofxVec4f&  average( const ofxVec4f* points, int num );
    

    // Normalization
    //
    ofxVec4f  getNormalized() const;
    ofxVec4f& normalize();


    // Limit length.
    //
	ofxVec4f  getLimited(float max) const;
    ofxVec4f& limit(float max);


    // Length
    //
    float length() const;
    float squareLength() const;
    /**
    * Dot Product.
    */
    float dot( const ofxVec4f& vec ) const;




    //---------------------------------------
    // this methods are deprecated in 006 please use:

    // getScaled
    ofxVec4f rescaled( const float length ) const;

    // scale
    ofxVec4f& rescale( const float length );

    // getNormalized
    ofxVec4f normalized() const;

    // getLimited
    ofxVec4f limited(float max) const;

    // squareLength
    float lengthSquared() const;

    // use squareDistance
    float  distanceSquared( const ofxVec4f& pnt ) const;

    // use getInterpolated
    ofxVec4f 	interpolated( const ofxVec4f& pnt, float p ) const;

    // use getMiddle
    ofxVec4f 	middled( const ofxVec4f& pnt ) const;    
};




// Non-Member operators
//
//
ofxVec4f operator+( float f, const ofxVec4f& vec );
ofxVec4f operator-( float f, const ofxVec4f& vec );
ofxVec4f operator*( float f, const ofxVec4f& vec );
ofxVec4f operator/( float f, const ofxVec4f& vec );







/////////////////
// Implementation
/////////////////


inline ofxVec4f::ofxVec4f( float _x,
		  float _y,
		  float _z,
		  float _w )
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}




// Getters and Setters.
//
//
inline void ofxVec4f::set( float _x, float _y, float _z, float _w ) {
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

inline void ofxVec4f::set( const ofxVec4f& vec ) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}

inline float& ofxVec4f::operator[]( const int& i ) {
	switch(i) {
		case 0:  return x;
		case 1:  return y;
		case 2:  return z;
		case 3:  return w;
		default: return x;
	}
}



// Check similarity/equality.
//
//
inline bool ofxVec4f::operator==( const ofxVec4f& vec ) {
	return (x == vec.x) && (y == vec.y) && (z == vec.z) && (w == vec.w);
}

inline bool ofxVec4f::operator!=( const ofxVec4f& vec ) {
	return (x != vec.x) || (y != vec.y) || (z != vec.z) || (w != vec.w);
}

inline bool ofxVec4f::match( const ofxVec4f& vec, float tollerance) {
	return (fabs(x - vec.x) < tollerance)
		&& (fabs(y - vec.y) < tollerance)
		&& (fabs(z - vec.z) < tollerance)
		&& (fabs(w - vec.w) < tollerance);
}




// Additions and Subtractions.
//
//
inline ofxVec4f ofxVec4f::operator+( const ofxVec4f& vec ) const {
	return ofxVec4f( x+vec.x, y+vec.y, z+vec.z, w+vec.w);
}

inline ofxVec4f& ofxVec4f::operator+=( const ofxVec4f& vec ) {
	x += vec.x;
	y += vec.y;
	z += vec.z;
	w += vec.w;
	return *this;
}

inline ofxVec4f ofxVec4f::operator-( const float f ) const {
	return ofxVec4f( x-f, y-f, z-f, w-f );
}

inline ofxVec4f& ofxVec4f::operator-=( const float f ) {
	x -= f;
	y -= f;
	z -= f;
	w -= f;
	return *this;
}

inline ofxVec4f ofxVec4f::operator-( const ofxVec4f& vec ) const {
	return ofxVec4f( x-vec.x, y-vec.y, z-vec.z, w-vec.w );
}

inline ofxVec4f& ofxVec4f::operator-=( const ofxVec4f& vec ) {
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	w -= vec.w;
	return *this;
}

inline ofxVec4f ofxVec4f::operator+( const float f ) const {
	return ofxVec4f( x+f, y+f, z+f, w+f );
}

inline ofxVec4f& ofxVec4f::operator+=( const float f ) {
	x += f;
	y += f;
	z += f;
	w += f;
	return *this;
}

inline ofxVec4f ofxVec4f::operator-() const {
	return ofxVec4f( -x, -y, -z, -w );
}


// Scalings
//
//
inline ofxVec4f ofxVec4f::operator*( const ofxVec4f& vec ) const {
	return ofxVec4f( x*vec.x, y*vec.y, z*vec.z, w*vec.w );
}

inline ofxVec4f& ofxVec4f::operator*=( const ofxVec4f& vec ) {
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
	w *= vec.w;
	return *this;
}

inline ofxVec4f ofxVec4f::operator*( const float f ) const {
	return ofxVec4f( x*f, y*f, z*f, w*f );
}

inline ofxVec4f& ofxVec4f::operator*=( const float f ) {
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

inline ofxVec4f ofxVec4f::operator/( const ofxVec4f& vec ) const {
	return ofxVec4f( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y, vec.z!=0 ? z/vec.z : z, vec.w!=0 ? w/vec.w : w  );
}

inline ofxVec4f& ofxVec4f::operator/=( const ofxVec4f& vec ) {
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	vec.z!=0 ? z/=vec.z : z;
	vec.w!=0 ? w/=vec.w : w;
	return *this;
}

inline ofxVec4f ofxVec4f::operator/( const float f ) const {
	if(f == 0) return ofxVec4f(x, y, z, w);

	return ofxVec4f( x/f, y/f, z/f, w/f );
}

inline ofxVec4f& ofxVec4f::operator/=( const float f ) {
	if(f == 0)return *this;

	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this;
}

inline ofxVec4f ofxVec4f::rescaled( const float length ) const {
	return getScaled(length);
}

inline ofxVec4f ofxVec4f::getScaled( const float length ) const {
	float l = (float)sqrt(x*x + y*y + z*z + w*w);
	if( l > 0 )
		return ofxVec4f( (x/l)*length, (y/l)*length,
						(z/l)*length, (w/l)*length );
	else
		return ofxVec4f();
}

inline ofxVec4f& ofxVec4f::rescale( const float length ) {
	return scale(length);
}

inline ofxVec4f& ofxVec4f::scale( const float length ) {
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
inline float ofxVec4f::distance( const ofxVec4f& pnt) const {
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	float vz = z-pnt.z;
	float vw = w-pnt.w;
	return (float)sqrt( vx*vx + vy*vy + vz*vz + vw*vw );
}

inline float ofxVec4f::distanceSquared( const ofxVec4f& pnt ) const {
	return squareDistance(pnt);
}

inline float ofxVec4f::squareDistance( const ofxVec4f& pnt ) const {
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
inline ofxVec4f ofxVec4f::interpolated( const ofxVec4f& pnt, float p ) const{
	return getInterpolated(pnt,p);
}

inline ofxVec4f ofxVec4f::getInterpolated( const ofxVec4f& pnt, float p ) const {
	return ofxVec4f( x*(1-p) + pnt.x*p,
                     y*(1-p) + pnt.y*p,
                     z*(1-p) + pnt.z*p,
                     w*(1-p) + pnt.w*p );
}

inline ofxVec4f& ofxVec4f::interpolate( const ofxVec4f& pnt, float p ) {
	x = x*(1-p) + pnt.x*p;
	y = y*(1-p) + pnt.y*p;
	z = z*(1-p) + pnt.z*p;
	w = w*(1-p) + pnt.w*p;
	return *this;
}

inline ofxVec4f ofxVec4f::middled( const ofxVec4f& pnt ) const {
	return getMiddle(pnt);
}

inline ofxVec4f ofxVec4f::getMiddle( const ofxVec4f& pnt ) const {
	return ofxVec4f( (x+pnt.x)/2.0f, (y+pnt.y)/2.0f,
                      (z+pnt.z)/2.0f, (w+pnt.w)/2.0f );
}

inline ofxVec4f& ofxVec4f::middle( const ofxVec4f& pnt ) {
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
inline ofxVec4f& ofxVec4f::average( const ofxVec4f* points, int num ) {
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
inline ofxVec4f ofxVec4f::normalized() const {
	return getNormalized();
}

inline ofxVec4f ofxVec4f::getNormalized() const {
	float length = (float)sqrt(x*x + y*y + z*z + w*w);
	if( length > 0 ) {
		return ofxVec4f( x/length, y/length, z/length, w/length );
	} else {
		return ofxVec4f();
	}
}

inline ofxVec4f& ofxVec4f::normalize() {
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
inline ofxVec4f ofxVec4f::limited(float max) const {
	return getLimited(max);
}

inline ofxVec4f ofxVec4f::getLimited(float max) const {
    ofxVec4f limited;
    float lengthSquared = (x*x + y*y + z*z + w*w);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        float ratio = max/(float)sqrt(lengthSquared);
        limited.set( x*ratio, y*ratio, z*ratio, w*ratio );
    } else {
        limited.set(x,y,z,w);
    }
    return limited;
}

inline ofxVec4f& ofxVec4f::limit(float max) {
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
inline float ofxVec4f::length() const {
	return (float)sqrt( x*x + y*y + z*z + w*w );
}

inline float ofxVec4f::lengthSquared() const {
	return squareLength();
}

inline float ofxVec4f::squareLength() const {
	return (float)(x*x + y*y + z*z + w*w);
}



/**
* Dot Product.
*/
inline float ofxVec4f::dot( const ofxVec4f& vec ) const {
	return x*vec.x + y*vec.y + z*vec.z + w*vec.w;
}





// Non-Member operators
//
//
inline ofxVec4f operator+( float f, const ofxVec4f& vec ) {
    return ofxVec4f( f+vec.x, f+vec.y, f+vec.z, f+vec.w );
}

inline ofxVec4f operator-( float f, const ofxVec4f& vec ) {
    return ofxVec4f( f-vec.x, f-vec.y, f-vec.z, f-vec.w );
}

inline ofxVec4f operator*( float f, const ofxVec4f& vec ) {
    return ofxVec4f( f*vec.x, f*vec.y, f*vec.z, f*vec.w );
}

inline ofxVec4f operator/( float f, const ofxVec4f& vec ) {
    return ofxVec4f( f/vec.x, f/vec.y, f/vec.z, f/vec.w);
}





#endif
