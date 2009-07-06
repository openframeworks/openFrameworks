#ifndef _OFX_VEC3f
#define _OFX_VEC3f

#include "ofConstants.h"
#include "ofTypes.h"



class ofxVec3f : public ofPoint {


  public:

    ofxVec3f( float _x=0.0f,
              float _y=0.0f,
              float _z=0.0f );
              
    ofxVec3f( const ofPoint& pnt );


    // Getters and Setters.
    //
    void set( float _x, float _y, float _z );
    void set( const ofPoint& vec );
    float &operator[]( const int& i );


    // Check similarity/equality.
    //
    bool operator==( const ofPoint& vec );
    bool operator!=( const ofPoint& vec );
    bool match( const ofPoint& vec, float tollerance=0.0001 );
    /**
    * Checks if vectors look in the same direction.
    */
    bool align( const ofxVec3f& vec, float tollerance=0.0001 ) const;
    bool alignRad( const ofxVec3f& vec, float tollerance=0.0001 ) const;


    // Operator overloading for ofPoint
    //
    void 	  operator=( const ofPoint& vec );
    ofxVec3f  operator+( const ofPoint& pnt ) const;
    ofxVec3f& operator+=( const ofPoint& pnt );
    ofxVec3f  operator-( const ofPoint& vec ) const;
    ofxVec3f& operator-=( const ofPoint& vec );
    ofxVec3f  operator*( const ofPoint& vec ) const;
    ofxVec3f& operator*=( const ofPoint& vec );
    ofxVec3f  operator/( const ofPoint& vec ) const;
    ofxVec3f& operator/=( const ofPoint& vec );
    ofxVec3f  operator-() const;


    //operator overloading for float
    //
    void 	  operator=( const float f);
    ofxVec3f  operator+( const float f ) const;
    ofxVec3f& operator+=( const float f );
 	ofxVec3f  operator-( const float f ) const;
    ofxVec3f& operator-=( const float f );
    ofxVec3f  operator*( const float f ) const;
    ofxVec3f& operator*=( const float f );
    ofxVec3f  operator/( const float f ) const;
    ofxVec3f& operator/=( const float f );


    //Scale
    //
    ofxVec3f  getScaled( const float length ) const;
    ofxVec3f& scale( const float length );
    

    // Rotation
    //
    ofxVec3f  getRotated( float angle, const ofxVec3f& axis ) const;
    ofxVec3f  getRotatedRad( float angle, const ofxVec3f& axis ) const;
    ofxVec3f& rotate( float angle, const ofxVec3f& axis );
    ofxVec3f& rotateRad( float angle, const ofxVec3f& axis );
    ofxVec3f  getRotated(float ax, float ay, float az) const;
    ofxVec3f  getRotatedRad(float ax, float ay, float az) const;
    ofxVec3f& rotate(float ax, float ay, float az);
    ofxVec3f& rotateRad(float ax, float ay, float az);
    
    
    // Rotation - point around pivot
    //    
    ofxVec3f   getRotated( float angle, const ofPoint& pivot, const ofxVec3f& axis ) const;
    ofxVec3f&  rotate( float angle, const ofPoint& pivot, const ofxVec3f& axis );
    ofxVec3f   getRotatedRad( float angle, const ofPoint& pivot, const ofxVec3f& axis ) const;
    ofxVec3f&  rotateRad( float angle, const ofPoint& pivot, const ofxVec3f& axis );    


    // Map point to coordinate system defined by origin, vx, vy, and vz.
    //
    ofxVec3f getMapped( const ofPoint& origin,
                        const ofxVec3f& vx,
                        const ofxVec3f& vy,
                        const ofxVec3f& vz ) const;
    ofxVec3f& map( const ofPoint& origin,
                   const ofxVec3f& vx,
                   const ofxVec3f& vy,
                   const ofxVec3f& vz );


    // Distance between two points.
    //
    float distance( const ofPoint& pnt) const;
    float squareDistance( const ofPoint& pnt ) const;


    // Linear interpolation.
    //
    /**
    * p==0.0 results in this point, p==0.5 results in the
    * midpoint, and p==1.0 results in pnt being returned.
    */
    ofxVec3f   getInterpolated( const ofPoint& pnt, float p ) const;
    ofxVec3f&  interpolate( const ofPoint& pnt, float p );
    ofxVec3f   getMiddle( const ofPoint& pnt ) const;
    ofxVec3f&  middle( const ofPoint& pnt );
    ofxVec3f&  average( const ofPoint* points, int num );
    

    // Normalization
    //
    ofxVec3f  getNormalized() const;
    ofxVec3f& normalize();


    // Limit length.
    //
    ofxVec3f  getLimited(float max) const;
    ofxVec3f& limit(float max);


    // Perpendicular vector.
    //
    ofxVec3f  getCrossed( const ofxVec3f& vec ) const;
    ofxVec3f& cross( const ofxVec3f& vec );
    /**
    * Normalized perpendicular.
    */
    ofxVec3f  getPerpendicular( const ofxVec3f& vec ) const;
    ofxVec3f& perpendicular( const ofxVec3f& vec );


    // Length
    //
    float length() const;
    float squareLength() const;
    /**
    * Angle (deg) between two vectors.
    * This is an unsigned relative angle from 0 to 180.
    * http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
    */
    float angle( const ofxVec3f& vec ) const;
    float angleRad( const ofxVec3f& vec ) const;


    // Dot Product
    //
    float dot( const ofxVec3f& vec ) const;



    //-----------------------------------------------
    // this methods are deprecated in 006 please use:

    // getScaled
    ofxVec3f rescaled( const float length ) const;

    // scale
    ofxVec3f& rescale( const float length );

    // getRotated
    ofxVec3f rotated( float angle, const ofxVec3f& axis ) const;

    // getRotated should this be const???
    ofxVec3f rotated(float ax, float ay, float az);

    // getNormalized
    ofxVec3f normalized() const;

    // getLimited
    ofxVec3f limited(float max) const;

    // getCrossed
    ofxVec3f crossed( const ofxVec3f& vec ) const;

    // getPerpendicular
    ofxVec3f perpendiculared( const ofxVec3f& vec ) const;

    // squareLength
    float lengthSquared() const;
    
    // use getMapped
    ofxVec3f  mapped( const ofPoint& origin,
                      const ofxVec3f& vx,
                      const ofxVec3f& vy,
                      const ofxVec3f& vz ) const;

    // use squareDistance
    float  distanceSquared( const ofPoint& pnt ) const;

    // use getInterpolated
    ofxVec3f 	interpolated( const ofPoint& pnt, float p ) const;

    // use getMiddle
    ofxVec3f 	middled( const ofPoint& pnt ) const;
    
    // use getRotated
    ofxVec3f 	rotated( float angle,
                         const ofPoint& pivot,
                         const ofxVec3f& axis ) const;    
};




// Non-Member operators
//
//
ofxVec3f operator+( float f, const ofxVec3f& vec );
ofxVec3f operator-( float f, const ofxVec3f& vec );
ofxVec3f operator*( float f, const ofxVec3f& vec );
ofxVec3f operator/( float f, const ofxVec3f& vec );





/////////////////
// Implementation
/////////////////


inline ofxVec3f::ofxVec3f( float _x,
		  float _y,
		  float _z )
{
	x = _x;
	y = _y;
	z = _z;
}

inline ofxVec3f::ofxVec3f( const ofPoint& pnt ) {
	x = pnt.x;
	y = pnt.y;
	z = pnt.z;
}



// Getters and Setters.
//
//
inline void ofxVec3f::set( float _x, float _y, float _z ) {
	x = _x;
	y = _y;
	z = _z;
}

inline void ofxVec3f::set( const ofPoint& vec ) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

inline float& ofxVec3f::operator[]( const int& i ) {
	switch(i) {
		case 0:  return x;
		case 1:  return y;
		case 2:  return z;
		default: return x;
	}
}



// Check similarity/equality.
//
//
inline bool ofxVec3f::operator==( const ofPoint& vec ) {
	return (x == vec.x) && (y == vec.y) && (z == vec.z);
}

inline bool ofxVec3f::operator!=( const ofPoint& vec ) {
	return (x != vec.x) || (y != vec.y) || (z != vec.z);
}

inline bool ofxVec3f::match( const ofPoint& vec, float tollerance ) {
	return (fabs(x - vec.x) < tollerance)
		&& (fabs(y - vec.y) < tollerance)
		&& (fabs(z - vec.z) < tollerance);
}

/**
* Checks if vectors look in the same direction.
*/
inline bool ofxVec3f::align( const ofxVec3f& vec, float tollerance ) const {
	float angle = this->angle( vec );
	return  angle < tollerance;
}

inline bool ofxVec3f::alignRad( const ofxVec3f& vec, float tollerance ) const {
	float angle = this->angleRad( vec );
	return  angle < tollerance;
}


// Operator overloading for ofPoint
//
//

inline void ofxVec3f::operator=( const ofPoint& vec ){
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

inline ofxVec3f ofxVec3f::operator+( const ofPoint& pnt ) const {
	return ofxVec3f( x+pnt.x, y+pnt.y, z+pnt.z );
}

inline ofxVec3f& ofxVec3f::operator+=( const ofPoint& pnt ) {
	x+=pnt.x;
	y+=pnt.y;
	z+=pnt.z;
	return *this;
}

inline ofxVec3f ofxVec3f::operator-( const ofPoint& vec ) const {
	return ofxVec3f( x-vec.x, y-vec.y, z-vec.z );
}

inline ofxVec3f& ofxVec3f::operator-=( const ofPoint& vec ) {
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

inline ofxVec3f ofxVec3f::operator*( const ofPoint& vec ) const {
	return ofxVec3f( x*vec.x, y*vec.y, z*vec.z );
}

inline ofxVec3f& ofxVec3f::operator*=( const ofPoint& vec ) {
	x*=vec.x;
	y*=vec.y;
	z*=vec.z;
	return *this;
}

inline ofxVec3f ofxVec3f::operator/( const ofPoint& vec ) const {
	return ofxVec3f( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y, vec.z!=0 ? z/vec.z : z );
}

inline ofxVec3f& ofxVec3f::operator/=( const ofPoint& vec ) {
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	vec.z!=0 ? z/=vec.z : z;
	return *this;
}

inline ofxVec3f ofxVec3f::operator-() const {
	return ofxVec3f( -x, -y, -z );
}


//operator overloading for float
//
//
inline void ofxVec3f::operator=( const float f){
	x = f;
	y = f;
	z = f;
}

inline ofxVec3f ofxVec3f::operator+( const float f ) const {
	return ofxVec3f( x+f, y+f, z+f);
}

inline ofxVec3f& ofxVec3f::operator+=( const float f ) {
	x += f;
	y += f;
	z += f;
	return *this;
}

inline ofxVec3f ofxVec3f::operator-( const float f ) const {
	return ofxVec3f( x-f, y-f, z-f);
}

inline ofxVec3f& ofxVec3f::operator-=( const float f ) {
	x -= f;
	y -= f;
	z -= f;
	return *this;
}

inline ofxVec3f ofxVec3f::operator*( const float f ) const {
	return ofxVec3f( x*f, y*f, z*f );
}

inline ofxVec3f& ofxVec3f::operator*=( const float f ) {
	x*=f;
	y*=f;
	z*=f;
	return *this;
}

inline ofxVec3f ofxVec3f::operator/( const float f ) const {
	 if(f == 0) return ofxVec3f( x, y, z);

	return ofxVec3f( x/f, y/f, z/f );
}

inline ofxVec3f& ofxVec3f::operator/=( const float f ) {
	if(f == 0) return *this;

	x/=f;
	y/=f;
	z/=f;
	return *this;
}


//Scale
//
//
inline ofxVec3f ofxVec3f::rescaled( const float length ) const {
	return getScaled(length);
}
inline ofxVec3f ofxVec3f::getScaled( const float length ) const {
	float l = (float)sqrt(x*x + y*y + z*z);
	if( l > 0 )
		return ofxVec3f( (x/l)*length, (y/l)*length, (z/l)*length );
	else
		return ofxVec3f();
}
inline ofxVec3f& ofxVec3f::rescale( const float length ) {
	return scale(length);
}
inline ofxVec3f& ofxVec3f::scale( const float length ) {
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
inline ofxVec3f ofxVec3f::rotated( float angle, const ofxVec3f& axis ) const {
	return getRotated(angle, axis);
}
inline ofxVec3f ofxVec3f::getRotated( float angle, const ofxVec3f& axis ) const {
	ofxVec3f ax = axis.normalized();
	float a = (float)(angle*DEG_TO_RAD);
	float sina = sin( a );
	float cosa = cos( a );
	float cosb = 1.0f - cosa;

	return ofxVec3f( x*(ax.x*ax.x*cosb + cosa)
				  + y*(ax.x*ax.y*cosb - ax.z*sina)
				  + z*(ax.x*ax.z*cosb + ax.y*sina),
					x*(ax.y*ax.x*cosb + ax.z*sina)
				  + y*(ax.y*ax.y*cosb + cosa)
				  + z*(ax.y*ax.z*cosb - ax.x*sina),
					x*(ax.z*ax.x*cosb - ax.y*sina)
				  + y*(ax.z*ax.y*cosb + ax.x*sina)
				  + z*(ax.z*ax.z*cosb + cosa) );
}

inline ofxVec3f ofxVec3f::getRotatedRad( float angle, const ofxVec3f& axis ) const {
	ofxVec3f ax = axis.normalized();
	float a = angle;
	float sina = sin( a );
	float cosa = cos( a );
	float cosb = 1.0f - cosa;

	return ofxVec3f( x*(ax.x*ax.x*cosb + cosa)
				  + y*(ax.x*ax.y*cosb - ax.z*sina)
				  + z*(ax.x*ax.z*cosb + ax.y*sina),
					x*(ax.y*ax.x*cosb + ax.z*sina)
				  + y*(ax.y*ax.y*cosb + cosa)
				  + z*(ax.y*ax.z*cosb - ax.x*sina),
					x*(ax.z*ax.x*cosb - ax.y*sina)
				  + y*(ax.z*ax.y*cosb + ax.x*sina)
				  + z*(ax.z*ax.z*cosb + cosa) );
}

inline ofxVec3f& ofxVec3f::rotate( float angle, const ofxVec3f& axis ) {
	ofxVec3f ax = axis.normalized();
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


inline ofxVec3f& ofxVec3f::rotateRad(float angle, const ofxVec3f& axis ) {
	ofxVec3f ax = axis.normalized();
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
inline ofxVec3f ofxVec3f::rotated(float ax, float ay, float az) {
	return getRotated(ax,ay,az);
}

inline ofxVec3f ofxVec3f::getRotated(float ax, float ay, float az) const {
	float a = (float)cos(DEG_TO_RAD*(ax));
	float b = (float)sin(DEG_TO_RAD*(ax));
	float c = (float)cos(DEG_TO_RAD*(ay));
	float d = (float)sin(DEG_TO_RAD*(ay));
	float e = (float)cos(DEG_TO_RAD*(az));
	float f = (float)sin(DEG_TO_RAD*(az));

	float nx = c * e * x - c * f * y + d * z;
	float ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	float nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;

	return ofxVec3f( nx, ny, nz );
}

inline ofxVec3f ofxVec3f::getRotatedRad(float ax, float ay, float az) const {
	float a = cos(ax);
	float b = sin(ax);
	float c = cos(ay);
	float d = sin(ay);
	float e = cos(az);
	float f = sin(az);

	float nx = c * e * x - c * f * y + d * z;
	float ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	float nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;

	return ofxVec3f( nx, ny, nz );
}


inline ofxVec3f& ofxVec3f::rotate(float ax, float ay, float az) {
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


inline ofxVec3f& ofxVec3f::rotateRad(float ax, float ay, float az) {
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
inline ofxVec3f ofxVec3f::rotated( float angle,
                                   const ofPoint& pivot,
                                   const ofxVec3f& axis ) const{
	return getRotated(angle, pivot, axis);
}

inline ofxVec3f ofxVec3f::getRotated( float angle,
                                      const ofPoint& pivot,
                                      const ofxVec3f& axis ) const
{
	ofxVec3f ax = axis.normalized();
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


	return ofxVec3f( xrot+pivot.x, yrot+pivot.y, zrot+pivot.z );
}


inline ofxVec3f ofxVec3f::getRotatedRad( float angle,
                                         const ofPoint& pivot,
                                         const ofxVec3f& axis ) const
{
	ofxVec3f ax = axis.normalized();
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


	return ofxVec3f( xrot+pivot.x, yrot+pivot.y, zrot+pivot.z );
}


inline ofxVec3f& ofxVec3f::rotate( float angle,
                                   const ofPoint& pivot,
                                   const ofxVec3f& axis )
{
	ofxVec3f ax = axis.normalized();
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


inline ofxVec3f& ofxVec3f::rotateRad( float angle,
                                      const ofPoint& pivot,
                                      const ofxVec3f& axis )
{
	ofxVec3f ax = axis.normalized();
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
inline ofxVec3f ofxVec3f::mapped( const ofPoint& origin,
                                  const ofxVec3f& vx,
                                  const ofxVec3f& vy,
                                  const ofxVec3f& vz ) const{
	return getMapped(origin, vx, vy, vz);
}

inline ofxVec3f ofxVec3f::getMapped( const ofPoint& origin,
                                     const ofxVec3f& vx,
                                     const ofxVec3f& vy,
                                     const ofxVec3f& vz ) const
{
	return ofxVec3f( origin.x + x*vx.x + y*vy.x + z*vz.x,
                     origin.y + x*vx.y + y*vy.y + z*vz.y,
                     origin.z + x*vx.z + y*vy.z + z*vz.z );
}

inline ofxVec3f& ofxVec3f::map( const ofPoint& origin,
                                const ofxVec3f& vx,
                                const ofxVec3f& vy,
                                const ofxVec3f& vz )
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
inline float ofxVec3f::distance( const ofPoint& pnt) const {
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	float vz = z-pnt.z;
	return (float)sqrt(vx*vx + vy*vy + vz*vz);
}

inline float  ofxVec3f::distanceSquared( const ofPoint& pnt ) const{
	return squareDistance(pnt);
}

inline float  ofxVec3f::squareDistance( const ofPoint& pnt ) const {
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

inline ofxVec3f ofxVec3f::interpolated( const ofPoint& pnt, float p ) const {
	return getInterpolated(pnt,p);
}

inline ofxVec3f ofxVec3f::getInterpolated( const ofPoint& pnt, float p ) const {
	return ofxVec3f( x*(1-p) + pnt.x*p,
                     y*(1-p) + pnt.y*p,
                     z*(1-p) + pnt.z*p );
}

inline ofxVec3f& ofxVec3f::interpolate( const ofPoint& pnt, float p ) {
	x = x*(1-p) + pnt.x*p;
	y = y*(1-p) + pnt.y*p;
	z = z*(1-p) + pnt.z*p;
	return *this;
}


inline ofxVec3f ofxVec3f::middled( const ofPoint& pnt ) const {
	return getMiddle(pnt);
}

inline ofxVec3f ofxVec3f::getMiddle( const ofPoint& pnt ) const {
	return ofxVec3f( (x+pnt.x)/2.0f, (y+pnt.y)/2.0f, (z+pnt.z)/2.0f );
}

inline ofxVec3f& ofxVec3f::middle( const ofPoint& pnt ) {
	x = (x+pnt.x)/2.0f;
	y = (y+pnt.y)/2.0f;
	z = (z+pnt.z)/2.0f;
	return *this;
}


// Average (centroid) among points.
// Addition is sometimes useful for calculating averages too.
//
//
inline ofxVec3f& ofxVec3f::average( const ofPoint* points, int num ) {
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
inline ofxVec3f ofxVec3f::normalized() const {
	return getNormalized();
}

inline ofxVec3f ofxVec3f::getNormalized() const {
	float length = (float)sqrt(x*x + y*y + z*z);
	if( length > 0 ) {
		return ofxVec3f( x/length, y/length, z/length );
	} else {
		return ofxVec3f();
	}
}

inline ofxVec3f& ofxVec3f::normalize() {
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

inline ofxVec3f ofxVec3f::limited(float max) const {
	return getLimited(max);
}

inline ofxVec3f ofxVec3f::getLimited(float max) const {
    ofxVec3f limited;
    float lengthSquared = (x*x + y*y + z*z);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        float ratio = max/(float)sqrt(lengthSquared);
        limited.set( x*ratio, y*ratio, z*ratio);
    } else {
        limited.set(x,y,z);
    }
    return limited;
}
 
inline ofxVec3f& ofxVec3f::limit(float max) {
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
inline ofxVec3f ofxVec3f::crossed( const ofxVec3f& vec ) const {
	return getCrossed(vec);
}
inline ofxVec3f ofxVec3f::getCrossed( const ofxVec3f& vec ) const {
	return ofxVec3f( y*vec.z - z*vec.y,
					 z*vec.x - x*vec.z,
					 x*vec.y - y*vec.x );
}

inline ofxVec3f& ofxVec3f::cross( const ofxVec3f& vec ) {
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
inline ofxVec3f ofxVec3f::perpendiculared( const ofxVec3f& vec ) const {
	return getPerpendicular(vec);
}

inline ofxVec3f ofxVec3f::getPerpendicular( const ofxVec3f& vec ) const {
	float crossX = y*vec.z - z*vec.y;
	float crossY = z*vec.x - x*vec.z;
	float crossZ = x*vec.y - y*vec.x;

	float length = (float)sqrt(crossX*crossX +
							   crossY*crossY +
							   crossZ*crossZ);

	if( length > 0 )
		return ofxVec3f( crossX/length, crossY/length, crossZ/length );
	else
		return ofxVec3f();
}

inline ofxVec3f& ofxVec3f::perpendicular( const ofxVec3f& vec ) {
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
inline float ofxVec3f::length() const {
	return (float)sqrt( x*x + y*y + z*z );
}

inline float ofxVec3f::lengthSquared() const {
	return squareLength();
}

inline float ofxVec3f::squareLength() const {
	return (float)(x*x + y*y + z*z);
}



/**
* Angle (deg) between two vectors.
* This is an unsigned relative angle from 0 to 180.
* http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
*/
inline float ofxVec3f::angle( const ofxVec3f& vec ) const {
	ofxVec3f n1 = this->normalized();
	ofxVec3f n2 = vec.normalized();
	return (float)(acos( n1.dot(n2) )*RAD_TO_DEG);
}

inline float ofxVec3f::angleRad( const ofxVec3f& vec ) const {
	ofxVec3f n1 = this->normalized();
	ofxVec3f n2 = vec.normalized();
	return (float)acos( n1.dot(n2) );
}



/**
* Dot Product.
*/
inline float ofxVec3f::dot( const ofxVec3f& vec ) const {
	return x*vec.x + y*vec.y + z*vec.z;
}





// Non-Member operators
//
//
inline ofxVec3f operator+( float f, const ofxVec3f& vec ) {
    return ofxVec3f( f+vec.x, f+vec.y, f+vec.z );
}

inline ofxVec3f operator-( float f, const ofxVec3f& vec ) {
    return ofxVec3f( f-vec.x, f-vec.y, f-vec.z );
}

inline ofxVec3f operator*( float f, const ofxVec3f& vec ) {
    return ofxVec3f( f*vec.x, f*vec.y, f*vec.z );
}

inline ofxVec3f operator/( float f, const ofxVec3f& vec ) {
    return ofxVec3f( f/vec.x, f/vec.y, f/vec.z);
}




#endif
