#include "ofxVec2f.h"


ofxVec2f::ofxVec2f( float _x, float _y ) {
	x = _x;
	y = _y;
}

ofxVec2f::ofxVec2f( const ofPoint& pnt ) {
	x = pnt.x;
	y = pnt.y;
}



// Getters and Setters.
//
//
void ofxVec2f::set( float _x, float _y ) {
	x = _x;
	y = _y;
}

void ofxVec2f::set( const ofPoint& vec ) {
	x = vec.x;
	y = vec.y;
}

float& ofxVec2f::operator[]( const int& i ) {
	switch(i) {
		case 0:  return x;
		case 1:  return y;
		default: return x;
	}
}



// Check similarity/equality.
//
//
bool ofxVec2f::operator==( const ofPoint& vec ) {
	return (x == vec.x) && (y == vec.y);
}

bool ofxVec2f::operator!=( const ofPoint& vec ) {
	return (x != vec.x) || (y != vec.y);
}

bool ofxVec2f::match( const ofPoint& vec, float tollerance ) {
	return (fabs(x - vec.x) < tollerance)
		&& (fabs(y - vec.y) < tollerance);
}

/**
* Checks if vectors look in the same direction.
* Tollerance is specified in degree.
*/
bool ofxVec2f::align( const ofxVec2f& vec, float tollerance ) const {
	return  fabs( this->angle( vec ) ) < tollerance;
}

bool ofxVec2f::alignRad( const ofxVec2f& vec, float tollerance ) const {
	return  fabs( this->angleRad( vec ) ) < tollerance;
}


// Overloading for any type to any type
//
//
void ofxVec2f::operator=( const ofPoint& vec ){
	x = vec.x;
	y = vec.y;
}

ofxVec2f ofxVec2f::operator+( const ofPoint& vec ) const {
	return ofxVec2f( x+vec.x, y+vec.y);
}

ofxVec2f& ofxVec2f::operator+=( const ofPoint& vec ) {
	x += vec.x;
	y += vec.y;
	return *this;
}

ofxVec2f ofxVec2f::operator-( const ofPoint& vec ) const {
	return ofxVec2f(x-vec.x, y-vec.y);
}

ofxVec2f& ofxVec2f::operator-=( const ofPoint& vec ) {
	x -= vec.x;
	y -= vec.y;
	return *this;
}

ofxVec2f ofxVec2f::operator*( const ofPoint& vec ) const {
	return ofxVec2f(x*vec.x, y*vec.y);
}

ofxVec2f& ofxVec2f::operator*=( const ofPoint& vec ) {
	x*=vec.x;
	y*=vec.y;
	return *this;
}

ofxVec2f ofxVec2f::operator/( const ofPoint& vec ) const {
	return ofxVec2f( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y);
}

ofxVec2f& ofxVec2f::operator/=( const ofPoint& vec ) {
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	return *this;
}


//operator overloading for float
//
//
void ofxVec2f::operator=( const float f){
	x = f;
	y = f;
}

ofxVec2f ofxVec2f::operator+( const float f ) const {
	return ofxVec2f( x+f, y+f);
}

ofxVec2f& ofxVec2f::operator+=( const float f ) {
	x += f;
	y += f;
	return *this;
}

ofxVec2f ofxVec2f::operator-( const float f ) const {
	return ofxVec2f( x-f, y-f);
}

ofxVec2f& ofxVec2f::operator-=( const float f ) {
	x -= f;
	y -= f;
	return *this;
}

ofxVec2f ofxVec2f::operator-() const {
	return ofxVec2f(-x, -y);
}

ofxVec2f ofxVec2f::operator*( const float f ) const {
	return ofxVec2f(x*f, y*f);
}

ofxVec2f& ofxVec2f::operator*=( const float f ) {
	x*=f;
	y*=f;
	return *this;
}

ofxVec2f ofxVec2f::operator/( const float f ) const {
	if(f == 0) return ofxVec2f(x, y);

	return ofxVec2f(x/f, y/f);
}

ofxVec2f& ofxVec2f::operator/=( const float f ) {
	if(f == 0) return *this;

	x/=f;
	y/=f;
	return *this;
}

ofxVec2f ofxVec2f::rescaled( const float length ) const {
	return getScaled(length);
}

ofxVec2f ofxVec2f::getScaled( const float length ) const {
	float l = (float)sqrt(x*x + y*y);
	if( l > 0 )
		return ofxVec2f( (x/l)*length, (y/l)*length );
	else
		return ofxVec2f();
}

ofxVec2f& ofxVec2f::rescale( const float length ){
	return scale(length);
}

ofxVec2f& ofxVec2f::scale( const float length ) {
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
ofxVec2f ofxVec2f::rotated( float angle ) const {
	return getRotated(angle);
}

ofxVec2f ofxVec2f::getRotated( float angle ) const {
	float a = (float)(angle*DEG_TO_RAD);
	return ofxVec2f( x*cos(a) - y*sin(a),
					x*sin(a) + y*cos(a) );
}

ofxVec2f ofxVec2f::getRotatedRad( float angle ) const {
	float a = angle;
	return ofxVec2f( x*cos(a) - y*sin(a),
					x*sin(a) + y*cos(a) );
}

ofxVec2f& ofxVec2f::rotate( float angle ) {
	float a = (float)(angle * DEG_TO_RAD);
	float xrot = x*cos(a) - y*sin(a);
	y = x*sin(a) + y*cos(a);
	x = xrot;
	return *this;
}

ofxVec2f& ofxVec2f::rotateRad( float angle ) {
	float a = angle;
	float xrot = x*cos(a) - y*sin(a);
	y = x*sin(a) + y*cos(a);
	x = xrot;
	return *this;
}



// Normalization
//
//
ofxVec2f ofxVec2f::normalized() const {
	return getNormalized();
}

ofxVec2f ofxVec2f::getNormalized() const {
	float length = (float)sqrt(x*x + y*y);
	if( length > 0 ) {
		return ofxVec2f( x/length, y/length );
	} else {
		return ofxVec2f();
	}
}

ofxVec2f& ofxVec2f::normalize() {
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
ofxVec2f ofxVec2f::limited(float max) const{
	return getLimited(max);
}

ofxVec2f ofxVec2f::getLimited(float max) const {
	float length = (float)sqrt(x*x + y*y);
	if( length > max && length > 0 ) {
		return ofxVec2f( (x/length)*max, (y/length)*max );
	} else {
		return ofxVec2f( x, y );
	}
}

ofxVec2f& ofxVec2f::limit(float max) {
	float length = (float)sqrt(x*x + y*y);
	if( length > max && length > 0 ) {
		x = (x/length)*max;
		y = (y/length)*max;
	}
	return *this;
}



// Perpendicular normalized vector.
//
//
ofxVec2f ofxVec2f::perpendiculared() const {
	return getPerpendicular();
}

ofxVec2f ofxVec2f::getPerpendicular() const {
	float length = (float)sqrt( x*x + y*y );
	if( length > 0 )
		return ofxVec2f( -(y/length), x/length );
	else
		return ofxVec2f();
}

ofxVec2f& ofxVec2f::perpendicular() {
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
float ofxVec2f::length() const {
	return (float)sqrt( x*x + y*y );
}

float ofxVec2f::lengthSquared() const {
	return squareLength();
}

float ofxVec2f::squareLength() const {
	return (float)(x*x + y*y);
}



/**
* Angle (deg) between two vectors.
* This is a signed relative angle between -180 and 180.
*/
float ofxVec2f::angle( const ofxVec2f& vec ) const {
	return (float)(atan2( x*vec.y-y*vec.x, x*vec.x + y*vec.y )*RAD_TO_DEG);
}

/**
* Angle (deg) between two vectors.
* This is a signed relative angle between -180 and 180.
*/
float ofxVec2f::angleRad( const ofxVec2f& vec ) const {
	return atan2( x*vec.y-y*vec.x, x*vec.x + y*vec.y );
}


/**
* Dot Product.
*/
float ofxVec2f::dot( const ofxVec2f& vec ) const {
	return x*vec.x + y*vec.y;
}







// Non-Member operators
//
//
ofxVec2f operator+( float f, const ofxVec2f& vec ) {
    return ofxVec2f( f+vec.x, f+vec.y);
}

ofxVec2f operator-( float f, const ofxVec2f& vec ) {
    return ofxVec2f( f-vec.x, f-vec.y);
}

ofxVec2f operator*( float f, const ofxVec2f& vec ) {
    return ofxVec2f( f*vec.x, f*vec.y);
}

ofxVec2f operator/( float f, const ofxVec2f& vec ) {
    return ofxVec2f( f/vec.x, f/vec.y);
}

