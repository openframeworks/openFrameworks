#include "ofxPoint4f.h"




ofxPoint4f::ofxPoint4f( const float _x,
			const float _y,
			const float _z,
			const float _w )
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}




// Getters and Setters.
//
//
void ofxPoint4f::set( float _x, float _y, float _z, float _w ) {
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

void ofxPoint4f::set( const ofxPoint4f& pnt ) {
	x = pnt.x;
	y = pnt.y;
	z = pnt.z;
	w = pnt.w;
}

float& ofxPoint4f::operator[]( const int& i ) {
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
bool ofxPoint4f::operator==( const ofxPoint4f& pnt ) {
	return (x == pnt.x) && (y == pnt.y) && (z == pnt.z) && (w == pnt.w);
}

bool ofxPoint4f::operator!=( const ofxPoint4f& pnt ) {
	return (x != pnt.x) || (y != pnt.y) || (z != pnt.z) || (w != pnt.w);
}

bool ofxPoint4f::match( const ofxPoint4f& pnt, float tollerance) {
	return (fabs(x - pnt.x) < tollerance)
		&& (fabs(y - pnt.y) < tollerance)
		&& (fabs(z - pnt.z) < tollerance)
		&& (fabs(w - pnt.w) < tollerance);
}



// Translate point by vector.
//
//
ofxPoint4f ofxPoint4f::operator+( const ofxVec4f& vec ) const {
	return ofxPoint4f( x+vec.x, y+vec.y, z+vec.z, w+vec.w);
}

ofxPoint4f& ofxPoint4f::operator+=( const ofxVec4f& vec ) {
	x += vec.x;
	y += vec.y;
	z += vec.z;
	w += vec.w;
	return *this;
}

ofxPoint4f ofxPoint4f::operator+( const float f ) const {
	return ofxPoint4f( x+f, y+f, z+f, w+f );
}

ofxPoint4f& ofxPoint4f::operator+=( const float f ) {
	x+=f;
	y+=f;
	z+=f;
	w+=f;
	return *this;
}

ofxPoint4f ofxPoint4f::operator-( const ofxVec4f& vec ) const {
	return ofxPoint4f( x-vec.x, y-vec.y, z-vec.z, w-vec.w );
}

ofxPoint4f& ofxPoint4f::operator-=( const ofxVec4f& vec ) {
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	w -= vec.w;
	return *this;
}

ofxPoint4f ofxPoint4f::operator-( const float f ) const {
	return ofxPoint4f( x-f, y-f, z-f, w-f );
}

ofxPoint4f& ofxPoint4f::operator-=( const float f ) {
	x-=f;
	y-=f;
	z-=f;
	w-=f;
	return *this;
}

ofxPoint4f ofxPoint4f::operator-() const {
	return ofxPoint4f( -x, -y, -z, -w );
}



// Scale point (by scalar or vector).
//
//
ofxPoint4f ofxPoint4f::operator*( const float f ) const {
	return ofxPoint4f( x*f, y*f, z*f, w*f );
}

ofxPoint4f& ofxPoint4f::operator*=( const float f ) {
	x*=f;
	y*=f;
	z*=f;
	w*=f;
	return *this;
}

ofxPoint4f ofxPoint4f::operator*( const ofxVec4f& vec ) const {
	return ofxPoint4f( x*vec.x, y*vec.y, z*vec.z, w*vec.w );
}

ofxPoint4f& ofxPoint4f::operator*=( const ofxVec4f& vec ) {
	x*=vec.x;
	y*=vec.y;
	z*=vec.z;
	w*=vec.w;
	return *this;
}

ofxPoint4f ofxPoint4f::operator/( const float f ) const {
	if(f == 0) return ofxPoint4f(x, y, z, w);

	return ofxPoint4f( x/f, y/f, z/f, w/f );
}

ofxPoint4f& ofxPoint4f::operator/=( const float f ) {
	if(f == 0)return *this;

	x/=f;
	y/=f;
	z/=f;
	w/=f;
	return *this;
}

ofxPoint4f ofxPoint4f::operator/( const ofxVec4f& vec ) const
{
	return ofxPoint4f( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y, vec.z!=0 ? z/vec.z : z, vec.w!=0 ? w/vec.w : w );
}

ofxPoint4f& ofxPoint4f::operator/=( const ofxVec4f& vec ) {
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	vec.z!=0 ? z/=vec.z : z;
	vec.w!=0 ? w/=vec.w : w;
	return *this;
}


/**
* Return the difference vector between two points.
*/
ofxVec4f ofxPoint4f::operator-( const ofxPoint4f& pnt ) const {
	return ofxVec4f( x-pnt.x, y-pnt.y, z-pnt.z, w-pnt.w );
}



// Distance between two points.
//
//
float ofxPoint4f::distance( const ofxPoint4f& pnt) const {
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	float vz = z-pnt.z;
	float vw = w-pnt.w;
	return (float)sqrt( vx*vx + vy*vy + vz*vz + vw*vw );
}

float ofxPoint4f::distanceSquared( const ofxPoint4f& pnt ) const {
	return squareDistance(pnt);
}

float ofxPoint4f::squareDistance( const ofxPoint4f& pnt ) const {
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
ofxPoint4f ofxPoint4f::interpolated( const ofxPoint4f& pnt, float p ) const{
	return getInterpolated(pnt,p);
}

ofxPoint4f ofxPoint4f::getInterpolated( const ofxPoint4f& pnt, float p ) const {
	return ofxPoint4f( x*(1-p) + pnt.x*p,
					  y*(1-p) + pnt.y*p,
					  z*(1-p) + pnt.z*p,
					  w*(1-p) + pnt.w*p );
}

ofxPoint4f& ofxPoint4f::interpolate( const ofxPoint4f& pnt, float p ) {
	x = x*(1-p) + pnt.x*p;
	y = y*(1-p) + pnt.y*p;
	z = z*(1-p) + pnt.z*p;
	w = w*(1-p) + pnt.w*p;
	return *this;
}

ofxPoint4f ofxPoint4f::middled( const ofxPoint4f& pnt ) const {
	return getMiddle(pnt);
}

ofxPoint4f ofxPoint4f::getMiddle( const ofxPoint4f& pnt ) const {
	return ofxPoint4f( (x+pnt.x)/2.0f, (y+pnt.y)/2.0f,
					   (z+pnt.z)/2.0f, (w+pnt.w)/2.0f );
}

ofxPoint4f& ofxPoint4f::middle( const ofxPoint4f& pnt ) {
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
ofxPoint4f& ofxPoint4f::average( const ofxPoint4f* points, int num ) {
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

ofxPoint4f ofxPoint4f::operator+( const ofxPoint4f& pnt ) const {
	return ofxPoint4f( x+pnt.x, y+pnt.y, z+pnt.z,  w+pnt.w );
}

ofxPoint4f& ofxPoint4f::operator+=( const ofxPoint4f& pnt ) {
	x+=pnt.x;
	y+=pnt.y;
	z+=pnt.z;
	w+=pnt.w;
	return *this;
}






// Non-Member operators
//
//
ofxPoint4f operator+( float f, const ofxPoint4f& pnt ) {
    return ofxPoint4f( f+pnt.x, f+pnt.y, f+pnt.z, f+pnt.w );
}

ofxPoint4f operator-( float f, const ofxPoint4f& pnt ) {
    return ofxPoint4f( f-pnt.x, f-pnt.y, f-pnt.z, f-pnt.w );
}

ofxPoint4f operator*( float f, const ofxPoint4f& pnt ) {
    return ofxPoint4f( f*pnt.x, f*pnt.y, f*pnt.z, f*pnt.w );
}

ofxPoint4f operator/( float f, const ofxPoint4f& pnt ) {
    return ofxPoint4f( f/pnt.x, f/pnt.y, f/pnt.z, f/pnt.w);
}

