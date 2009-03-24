#include "ofxVec3f.h"


ofxVec3f::ofxVec3f( float _x,
		  float _y,
		  float _z )
{
	x = _x;
	y = _y;
	z = _z;
}

ofxVec3f::ofxVec3f( const ofPoint& pnt ) {
	x = pnt.x;
	y = pnt.y;
	z = pnt.z;
}



// Getters and Setters.
//
//
void ofxVec3f::set( float _x, float _y, float _z ) {
	x = _x;
	y = _y;
	z = _z;
}

void ofxVec3f::set( const ofPoint& vec ) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

float& ofxVec3f::operator[]( const int& i ) {
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
bool ofxVec3f::operator==( const ofPoint& vec ) {
	return (x == vec.x) && (y == vec.y) && (z == vec.z);
}

bool ofxVec3f::operator!=( const ofPoint& vec ) {
	return (x != vec.x) || (y != vec.y) || (z != vec.z);
}

bool ofxVec3f::match( const ofPoint& vec, float tollerance ) {
	return (fabs(x - vec.x) < tollerance)
		&& (fabs(y - vec.y) < tollerance)
		&& (fabs(z - vec.z) < tollerance);
}

/**
* Checks if vectors look in the same direction.
*/
bool ofxVec3f::align( const ofxVec3f& vec, float tollerance ) const {
	float angle = this->angle( vec );
	return  angle < tollerance;
}

bool ofxVec3f::alignRad( const ofxVec3f& vec, float tollerance ) const {
	float angle = this->angleRad( vec );
	return  angle < tollerance;
}


// Operator overloading for ofPoint
//
//

void ofxVec3f::operator=( const ofPoint& vec ){
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

ofxVec3f ofxVec3f::operator+( const ofPoint& pnt ) const {
	return ofxVec3f( x+pnt.x, y+pnt.y, z+pnt.z );
}

ofxVec3f& ofxVec3f::operator+=( const ofPoint& pnt ) {
	x+=pnt.x;
	y+=pnt.y;
	z+=pnt.z;
	return *this;
}

ofxVec3f ofxVec3f::operator-( const ofPoint& vec ) const {
	return ofxVec3f( x-vec.x, y-vec.y, z-vec.z );
}

ofxVec3f& ofxVec3f::operator-=( const ofPoint& vec ) {
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

ofxVec3f ofxVec3f::operator*( const ofPoint& vec ) const {
	return ofxVec3f( x*vec.x, y*vec.y, z*vec.z );
}

ofxVec3f& ofxVec3f::operator*=( const ofPoint& vec ) {
	x*=vec.x;
	y*=vec.y;
	z*=vec.z;
	return *this;
}

ofxVec3f ofxVec3f::operator/( const ofPoint& vec ) const {
	return ofxVec3f( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y, vec.z!=0 ? z/vec.z : z );
}

ofxVec3f& ofxVec3f::operator/=( const ofPoint& vec ) {
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	vec.z!=0 ? z/=vec.z : z;
	return *this;
}

ofxVec3f ofxVec3f::operator-() const {
	return ofxVec3f( -x, -y, -z );
}


//operator overloading for float
//
//
void ofxVec3f::operator=( const float f){
	x = f;
	y = f;
	z = f;
}

ofxVec3f ofxVec3f::operator+( const float f ) const {
	return ofxVec3f( x+f, y+f, z+f);
}

ofxVec3f& ofxVec3f::operator+=( const float f ) {
	x += f;
	y += f;
	z += f;
	return *this;
}

ofxVec3f ofxVec3f::operator-( const float f ) const {
	return ofxVec3f( x-f, y-f, z-f);
}

ofxVec3f& ofxVec3f::operator-=( const float f ) {
	x -= f;
	y -= f;
	z -= f;
	return *this;
}

ofxVec3f ofxVec3f::operator*( const float f ) const {
	return ofxVec3f( x*f, y*f, z*f );
}

ofxVec3f& ofxVec3f::operator*=( const float f ) {
	x*=f;
	y*=f;
	z*=f;
	return *this;
}

ofxVec3f ofxVec3f::operator/( const float f ) const {
	 if(f == 0) return ofxVec3f( x, y, z);

	return ofxVec3f( x/f, y/f, z/f );
}

ofxVec3f& ofxVec3f::operator/=( const float f ) {
	if(f == 0) return *this;

	x/=f;
	y/=f;
	z/=f;
	return *this;
}


//Scale
//
//
ofxVec3f ofxVec3f::rescaled( const float length ) const {
	return getScaled(length);
}
ofxVec3f ofxVec3f::getScaled( const float length ) const {
	float l = (float)sqrt(x*x + y*y + z*z);
	if( l > 0 )
		return ofxVec3f( (x/l)*length, (y/l)*length, (z/l)*length );
	else
		return ofxVec3f();
}
ofxVec3f& ofxVec3f::rescale( const float length ) {
	return scale(length);
}
ofxVec3f& ofxVec3f::scale( const float length ) {
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
ofxVec3f ofxVec3f::rotated( float angle, const ofxVec3f& axis ) const {
	return getRotated(angle, axis);
}
ofxVec3f ofxVec3f::getRotated( float angle, const ofxVec3f& axis ) const {
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

ofxVec3f ofxVec3f::getRotatedRad( float angle, const ofxVec3f& axis ) const {
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

ofxVec3f& ofxVec3f::rotate( float angle, const ofxVec3f& axis ) {
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


ofxVec3f& ofxVec3f::rotateRad(float angle, const ofxVec3f& axis ) {
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
ofxVec3f ofxVec3f::rotated(float ax, float ay, float az) {
	return getRotated(ax,ay,az);
}

ofxVec3f ofxVec3f::getRotated(float ax, float ay, float az) const {
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

ofxVec3f ofxVec3f::getRotatedRad(float ax, float ay, float az) const {
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


ofxVec3f& ofxVec3f::rotate(float ax, float ay, float az) {
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


ofxVec3f& ofxVec3f::rotateRad(float ax, float ay, float az) {
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


// Normalization
//
//
ofxVec3f ofxVec3f::normalized() const {
	return getNormalized();
}

ofxVec3f ofxVec3f::getNormalized() const {
	float length = (float)sqrt(x*x + y*y + z*z);
	if( length > 0 ) {
		return ofxVec3f( x/length, y/length, z/length );
	} else {
		return ofxVec3f();
	}
}

ofxVec3f& ofxVec3f::normalize() {
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

ofxVec3f ofxVec3f::limited(float max) const {
	return getLimited(max);
}

ofxVec3f ofxVec3f::getLimited(float max) const {
	float length = (float)sqrt(x*x + y*y + z*z);
	if( length > max && length > 0 ) {
		return ofxVec3f( (x/length)*max, (y/length)*max, (z/length)*max );
	} else {
		return ofxVec3f( x, y, z );
	}
}

ofxVec3f& ofxVec3f::limit(float max) {
	float length = (float)sqrt(x*x + y*y + z*z);
	if( length > max && length > 0 ) {
		x = (x/length)*max;
		y = (y/length)*max;
		z = (z/length)*max;
	}
	return *this;
}


// Perpendicular vector.
//
//
ofxVec3f ofxVec3f::crossed( const ofxVec3f& vec ) const {
	return getCrossed(vec);
}
ofxVec3f ofxVec3f::getCrossed( const ofxVec3f& vec ) const {
	return ofxVec3f( y*vec.z - z*vec.y,
					 z*vec.x - x*vec.z,
					 x*vec.y - y*vec.x );
}

ofxVec3f& ofxVec3f::cross( const ofxVec3f& vec ) {
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
ofxVec3f ofxVec3f::perpendiculared( const ofxVec3f& vec ) const {
	return getPerpendicular(vec);
}

ofxVec3f ofxVec3f::getPerpendicular( const ofxVec3f& vec ) const {
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

ofxVec3f& ofxVec3f::perpendicular( const ofxVec3f& vec ) {
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
float ofxVec3f::length() const {
	return (float)sqrt( x*x + y*y + z*z );
}

float ofxVec3f::lengthSquared() const {
	return squareLength();
}

float ofxVec3f::squareLength() const {
	return (float)(x*x + y*y + z*z);
}



/**
* Angle (deg) between two vectors.
* This is an unsigned relative angle from 0 to 180.
* http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
*/
float ofxVec3f::angle( const ofxVec3f& vec ) const {
	ofxVec3f n1 = this->normalized();
	ofxVec3f n2 = vec.normalized();
	return (float)(acos( n1.dot(n2) )*RAD_TO_DEG);
}

float ofxVec3f::angleRad( const ofxVec3f& vec ) const {
	ofxVec3f n1 = this->normalized();
	ofxVec3f n2 = vec.normalized();
	return (float)acos( n1.dot(n2) );
}



/**
* Dot Product.
*/
float ofxVec3f::dot( const ofxVec3f& vec ) const {
	return x*vec.x + y*vec.y + z*vec.z;
}





// Non-Member operators
//
//
ofxVec3f operator+( float f, const ofxVec3f& vec ) {
    return ofxVec3f( f+vec.x, f+vec.y, f+vec.z );
}

ofxVec3f operator-( float f, const ofxVec3f& vec ) {
    return ofxVec3f( f-vec.x, f-vec.y, f-vec.z );
}

ofxVec3f operator*( float f, const ofxVec3f& vec ) {
    return ofxVec3f( f*vec.x, f*vec.y, f*vec.z );
}

ofxVec3f operator/( float f, const ofxVec3f& vec ) {
    return ofxVec3f( f/vec.x, f/vec.y, f/vec.z);
}


