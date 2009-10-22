#ifndef _OFX_VEC2f
#define _OFX_VEC2f

#include "ofConstants.h"
#include "ofTypes.h"



class ofxVec2f : public ofPoint {


  public:

    ofxVec2f( float _x=0.0f, float _y=0.0f );
    ofxVec2f( const ofPoint& pnt );


    // Getters and Setters.
    //
    void set( float _x, float _y );
    void set( const ofPoint& vec );
    float &operator[]( const int& i );


    // Check similarity/equality.
    //
    bool operator==( const ofPoint& vec );
    bool operator!=( const ofPoint& vec );
    bool match( const ofPoint& vec, float tollerance=0.0001 );
    /**
    * Checks if vectors look in the same direction.
    * Tollerance is specified in degree.
    */
    bool align( const ofxVec2f& vec, float tollerance=0.0001 ) const;
    bool alignRad( const ofxVec2f& vec, float tollerance=0.0001 ) const;


    // Overloading for any type to any type
    //
    void 	  operator=( const ofPoint& vec );
    ofxVec2f  operator+( const ofPoint& vec ) const;
    ofxVec2f& operator+=( const ofPoint& vec );
    ofxVec2f  operator-( const ofPoint& vec ) const;
    ofxVec2f& operator-=( const ofPoint& vec );
    ofxVec2f  operator*( const ofPoint& vec ) const;
    ofxVec2f& operator*=( const ofPoint& vec );
    ofxVec2f  operator/( const ofPoint& vec ) const;
    ofxVec2f& operator/=( const ofPoint& vec );


    //operator overloading for float
    //
    void 	  operator=( const float f);
    ofxVec2f  operator+( const float f ) const;
    ofxVec2f& operator+=( const float f );
    ofxVec2f  operator-( const float f ) const;
    ofxVec2f& operator-=( const float f );
    ofxVec2f  operator-() const;
    ofxVec2f  operator*( const float f ) const;
    ofxVec2f& operator*=( const float f );
    ofxVec2f  operator/( const float f ) const;
    ofxVec2f& operator/=( const float f );


    // Scaling
    //
    ofxVec2f  getScaled( const float length ) const;
    ofxVec2f& scale( const float length );


    // Rotation
    //
    ofxVec2f  getRotated( float angle ) const;
    ofxVec2f  getRotatedRad( float angle ) const;
    ofxVec2f& rotate( float angle );
    ofxVec2f& rotateRad( float angle );

    
    // Rotation - point around pivot
    //
    ofxVec2f  getRotated( float angle, const ofPoint& pivot ) const;
    ofxVec2f& rotate( float angle, const ofPoint& pivot );
    ofxVec2f  getRotatedRad( float angle, const ofPoint& pivot ) const;
    ofxVec2f& rotateRad( float angle, const ofPoint& pivot );
        

    // Map point to coordinate system defined by origin, vx, and vy.
    //
    ofxVec2f getMapped( const ofPoint& origin,
                        const ofxVec2f& vx,
                        const ofxVec2f& vy ) const;
    ofxVec2f& map( const ofPoint& origin,
                   const ofxVec2f& vx, const ofxVec2f& vy );


    // Distance between two points.
    //
    float distance( const ofPoint& pnt) const;
    float squareDistance( const ofPoint& pnt ) const;


    // Linear interpolation.
    //
    //
    /**
    * p==0.0 results in this point, p==0.5 results in the
    * midpoint, and p==1.0 results in pnt being returned.
    */
    ofxVec2f   getInterpolated( const ofPoint& pnt, float p ) const;
    ofxVec2f&  interpolate( const ofPoint& pnt, float p );
    ofxVec2f   getMiddle( const ofPoint& pnt ) const;
    ofxVec2f&  middle( const ofPoint& pnt );
    ofxVec2f&  average( const ofPoint* points, int num );
    
    
    // Normalization
    //
    ofxVec2f  getNormalized() const;
    ofxVec2f& normalize();


    // Limit length.
    //
	ofxVec2f  getLimited(float max) const;
    ofxVec2f& limit(float max);


    // Perpendicular normalized vector.
    //
    ofxVec2f  getPerpendicular() const;
    ofxVec2f& perpendicular();


    // Length
    //
    float length() const;
    float squareLength() const;


    /**
    * Angle (deg) between two vectors.
    * This is a signed relative angle between -180 and 180.
    */
    float angle( const ofxVec2f& vec ) const;
    float angleRad( const ofxVec2f& vec ) const;


    /**
    * Dot Product.
    */
    float dot( const ofxVec2f& vec ) const;



    //---------------------------------------------------
    // this methods are deprecated in 006 please use:

    // getScaled
    ofxVec2f rescaled( const float length ) const;

    // scale
    ofxVec2f& rescale( const float length );

    // getRotated
    ofxVec2f rotated( float angle ) const;

    // getNormalized
    ofxVec2f normalized() const;

    // getLimited
    ofxVec2f limited(float max) const;

    // getPerpendicular
    ofxVec2f perpendiculared() const;

    // squareLength
    float lengthSquared() const;

    // getInterpolated
    ofxVec2f interpolated( const ofPoint& pnt, float p ) const;
    
    // getMiddled
    ofxVec2f middled( const ofPoint& pnt ) const;
    
    // getMapped 
    ofxVec2f mapped( const ofPoint& origin, const ofxVec2f& vx, const ofxVec2f& vy ) const;
    
    // squareDistance
    float distanceSquared( const ofPoint& pnt ) const;
    
    // use getRotated
    ofxVec2f rotated( float angle, const ofPoint& pivot ) const;    
};





// Non-Member operators
//
ofxVec2f operator+( float f, const ofxVec2f& vec );
ofxVec2f operator-( float f, const ofxVec2f& vec );
ofxVec2f operator*( float f, const ofxVec2f& vec );
ofxVec2f operator/( float f, const ofxVec2f& vec );







/////////////////
// Implementation
/////////////////


inline ofxVec2f::ofxVec2f( float _x, float _y ) {
	x = _x;
	y = _y;
}

inline ofxVec2f::ofxVec2f( const ofPoint& pnt ) {
	x = pnt.x;
	y = pnt.y;
}



// Getters and Setters.
//
//
inline void ofxVec2f::set( float _x, float _y ) {
	x = _x;
	y = _y;
}

inline void ofxVec2f::set( const ofPoint& vec ) {
	x = vec.x;
	y = vec.y;
}

inline float& ofxVec2f::operator[]( const int& i ) {
	switch(i) {
		case 0:  return x;
		case 1:  return y;
		default: return x;
	}
}



// Check similarity/equality.
//
//
inline bool ofxVec2f::operator==( const ofPoint& vec ) {
	return (x == vec.x) && (y == vec.y);
}

inline bool ofxVec2f::operator!=( const ofPoint& vec ) {
	return (x != vec.x) || (y != vec.y);
}

inline bool ofxVec2f::match( const ofPoint& vec, float tollerance ) {
	return (fabs(x - vec.x) < tollerance)
		&& (fabs(y - vec.y) < tollerance);
}

/**
* Checks if vectors look in the same direction.
* Tollerance is specified in degree.
*/
inline bool ofxVec2f::align( const ofxVec2f& vec, float tollerance ) const {
	return  fabs( this->angle( vec ) ) < tollerance;
}

inline bool ofxVec2f::alignRad( const ofxVec2f& vec, float tollerance ) const {
	return  fabs( this->angleRad( vec ) ) < tollerance;
}


// Overloading for any type to any type
//
//
inline void ofxVec2f::operator=( const ofPoint& vec ){
	x = vec.x;
	y = vec.y;
}

inline ofxVec2f ofxVec2f::operator+( const ofPoint& vec ) const {
	return ofxVec2f( x+vec.x, y+vec.y);
}

inline ofxVec2f& ofxVec2f::operator+=( const ofPoint& vec ) {
	x += vec.x;
	y += vec.y;
	return *this;
}

inline ofxVec2f ofxVec2f::operator-( const ofPoint& vec ) const {
	return ofxVec2f(x-vec.x, y-vec.y);
}

inline ofxVec2f& ofxVec2f::operator-=( const ofPoint& vec ) {
	x -= vec.x;
	y -= vec.y;
	return *this;
}

inline ofxVec2f ofxVec2f::operator*( const ofPoint& vec ) const {
	return ofxVec2f(x*vec.x, y*vec.y);
}

inline ofxVec2f& ofxVec2f::operator*=( const ofPoint& vec ) {
	x*=vec.x;
	y*=vec.y;
	return *this;
}

inline ofxVec2f ofxVec2f::operator/( const ofPoint& vec ) const {
	return ofxVec2f( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y);
}

inline ofxVec2f& ofxVec2f::operator/=( const ofPoint& vec ) {
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	return *this;
}


//operator overloading for float
//
//
inline void ofxVec2f::operator=( const float f){
	x = f;
	y = f;
}

inline ofxVec2f ofxVec2f::operator+( const float f ) const {
	return ofxVec2f( x+f, y+f);
}

inline ofxVec2f& ofxVec2f::operator+=( const float f ) {
	x += f;
	y += f;
	return *this;
}

inline ofxVec2f ofxVec2f::operator-( const float f ) const {
	return ofxVec2f( x-f, y-f);
}

inline ofxVec2f& ofxVec2f::operator-=( const float f ) {
	x -= f;
	y -= f;
	return *this;
}

inline ofxVec2f ofxVec2f::operator-() const {
	return ofxVec2f(-x, -y);
}

inline ofxVec2f ofxVec2f::operator*( const float f ) const {
	return ofxVec2f(x*f, y*f);
}

inline ofxVec2f& ofxVec2f::operator*=( const float f ) {
	x*=f;
	y*=f;
	return *this;
}

inline ofxVec2f ofxVec2f::operator/( const float f ) const {
	if(f == 0) return ofxVec2f(x, y);

	return ofxVec2f(x/f, y/f);
}

inline ofxVec2f& ofxVec2f::operator/=( const float f ) {
	if(f == 0) return *this;

	x/=f;
	y/=f;
	return *this;
}

inline ofxVec2f ofxVec2f::rescaled( const float length ) const {
	return getScaled(length);
}

inline ofxVec2f ofxVec2f::getScaled( const float length ) const {
	float l = (float)sqrt(x*x + y*y);
	if( l > 0 )
		return ofxVec2f( (x/l)*length, (y/l)*length );
	else
		return ofxVec2f();
}

inline ofxVec2f& ofxVec2f::rescale( const float length ){
	return scale(length);
}

inline ofxVec2f& ofxVec2f::scale( const float length ) {
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
inline ofxVec2f ofxVec2f::rotated( float angle ) const {
	return getRotated(angle);
}

inline ofxVec2f ofxVec2f::getRotated( float angle ) const {
	float a = (float)(angle*DEG_TO_RAD);
	return ofxVec2f( x*cos(a) - y*sin(a),
					x*sin(a) + y*cos(a) );
}

inline ofxVec2f ofxVec2f::getRotatedRad( float angle ) const {
	float a = angle;
	return ofxVec2f( x*cos(a) - y*sin(a),
					x*sin(a) + y*cos(a) );
}

inline ofxVec2f& ofxVec2f::rotate( float angle ) {
	float a = (float)(angle * DEG_TO_RAD);
	float xrot = x*cos(a) - y*sin(a);
	y = x*sin(a) + y*cos(a);
	x = xrot;
	return *this;
}

inline ofxVec2f& ofxVec2f::rotateRad( float angle ) {
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
inline ofxVec2f ofxVec2f::rotated( float angle, const ofPoint& pivot ) const {
	return getRotated(angle, pivot);
}

inline ofxVec2f ofxVec2f::getRotated( float angle, const ofPoint& pivot ) const {
	float a = (float)(angle * DEG_TO_RAD);
	return ofxVec2f( ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x,
                     ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y );
}

inline ofxVec2f& ofxVec2f::rotate( float angle, const ofPoint& pivot ) {
	float a = (float)(angle * DEG_TO_RAD);
	float xrot = ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x;
	y = ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y;
	x = xrot;
	return *this;
}

inline ofxVec2f ofxVec2f::getRotatedRad( float angle, const ofPoint& pivot ) const {
	float a = angle;
	return ofxVec2f( ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x,
                     ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y );
}

inline ofxVec2f& ofxVec2f::rotateRad( float angle, const ofPoint& pivot ) {
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
inline ofxVec2f ofxVec2f::mapped( const ofPoint& origin,
					              const ofxVec2f& vx,
					              const ofxVec2f& vy ) const{
	return getMapped(origin, vx, vy);
}

inline ofxVec2f ofxVec2f::getMapped( const ofPoint& origin,
				                     const ofxVec2f& vx,
				                     const ofxVec2f& vy ) const
{
	return ofxVec2f( origin.x + x*vx.x + y*vy.x,
                     origin.y + x*vx.y + y*vy.y );
}

inline ofxVec2f& ofxVec2f::map( const ofPoint& origin,
				                const ofxVec2f& vx, const ofxVec2f& vy )
{
	float xmap = origin.x + x*vx.x + y*vy.x;
	y = origin.y + x*vx.y + y*vy.y;
	x = xmap;
	return *this;
}


// Distance between two points.
//
//
inline float ofxVec2f::distance( const ofPoint& pnt) const {
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	return (float)sqrt(vx*vx + vy*vy);
}

//this method is deprecated in 006 please use squareDistance
inline float ofxVec2f::distanceSquared( const ofPoint& pnt ) const {
	return squareDistance(pnt);
}

inline float ofxVec2f::squareDistance( const ofPoint& pnt ) const {
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	return vx*vx + vy*vy;
}



// Linear interpolation.
//
//
/**
* p==0.0 results in this point, p==0.5 results in the
* midpoint, and p==1.0 results in pnt being returned.
*/

// this method is deprecated in 006 please use getInterpolated
inline ofxVec2f ofxVec2f::interpolated( const ofPoint& pnt, float p ) const{
	return getInterpolated(pnt, p);
}

inline ofxVec2f ofxVec2f::getInterpolated( const ofPoint& pnt, float p ) const {
	return ofxVec2f( x*(1-p) + pnt.x*p, y*(1-p) + pnt.y*p );
}

inline ofxVec2f& ofxVec2f::interpolate( const ofPoint& pnt, float p ) {
	x = x*(1-p) + pnt.x*p;
	y = y*(1-p) + pnt.y*p;
	return *this;
}

// this method is deprecated in 006 please use getMiddle
inline ofxVec2f ofxVec2f::middled( const ofPoint& pnt ) const{
	return getMiddle(pnt);
}

inline ofxVec2f ofxVec2f::getMiddle( const ofPoint& pnt ) const {
	return ofxVec2f( (x+pnt.x)/2.0f, (y+pnt.y)/2.0f );
}

inline ofxVec2f& ofxVec2f::middle( const ofPoint& pnt ) {
	x = (x+pnt.x)/2.0f;
	y = (y+pnt.y)/2.0f;
	return *this;
}



// Average (centroid) among points.
// Addition is sometimes useful for calculating averages too.
//
//
inline ofxVec2f& ofxVec2f::average( const ofPoint* points, int num ) {
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
inline ofxVec2f ofxVec2f::normalized() const {
	return getNormalized();
}

inline ofxVec2f ofxVec2f::getNormalized() const {
	float length = (float)sqrt(x*x + y*y);
	if( length > 0 ) {
		return ofxVec2f( x/length, y/length );
	} else {
		return ofxVec2f();
	}
}

inline ofxVec2f& ofxVec2f::normalize() {
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
inline ofxVec2f ofxVec2f::limited(float max) const{
	return getLimited(max);
}

inline ofxVec2f ofxVec2f::getLimited(float max) const {
    ofxVec2f limited;
    float lengthSquared = (x*x + y*y);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        float ratio = max/(float)sqrt(lengthSquared);
        limited.set( x*ratio, y*ratio);
    } else {
        limited.set(x,y);
    }
    return limited;
}

inline ofxVec2f& ofxVec2f::limit(float max) {
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
inline ofxVec2f ofxVec2f::perpendiculared() const {
	return getPerpendicular();
}

inline ofxVec2f ofxVec2f::getPerpendicular() const {
	float length = (float)sqrt( x*x + y*y );
	if( length > 0 )
		return ofxVec2f( -(y/length), x/length );
	else
		return ofxVec2f();
}

inline ofxVec2f& ofxVec2f::perpendicular() {
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
inline float ofxVec2f::length() const {
	return (float)sqrt( x*x + y*y );
}

inline float ofxVec2f::lengthSquared() const {
	return squareLength();
}

inline float ofxVec2f::squareLength() const {
	return (float)(x*x + y*y);
}



/**
* Angle (deg) between two vectors.
* This is a signed relative angle between -180 and 180.
*/
inline float ofxVec2f::angle( const ofxVec2f& vec ) const {
	return (float)(atan2( x*vec.y-y*vec.x, x*vec.x + y*vec.y )*RAD_TO_DEG);
}

/**
* Angle (deg) between two vectors.
* This is a signed relative angle between -180 and 180.
*/
inline float ofxVec2f::angleRad( const ofxVec2f& vec ) const {
	return atan2( x*vec.y-y*vec.x, x*vec.x + y*vec.y );
}


/**
* Dot Product.
*/
inline float ofxVec2f::dot( const ofxVec2f& vec ) const {
	return x*vec.x + y*vec.y;
}







// Non-Member operators
//
//
inline ofxVec2f operator+( float f, const ofxVec2f& vec ) {
    return ofxVec2f( f+vec.x, f+vec.y);
}

inline ofxVec2f operator-( float f, const ofxVec2f& vec ) {
    return ofxVec2f( f-vec.x, f-vec.y);
}

inline ofxVec2f operator*( float f, const ofxVec2f& vec ) {
    return ofxVec2f( f*vec.x, f*vec.y);
}

inline ofxVec2f operator/( float f, const ofxVec2f& vec ) {
    return ofxVec2f( f/vec.x, f/vec.y);
}




#endif
