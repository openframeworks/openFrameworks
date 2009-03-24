#include "ofxPoint2f.h"



ofxPoint2f::ofxPoint2f( float _x, float _y ) {
	x = _x;
	y = _y;
}

ofxPoint2f::ofxPoint2f( const ofPoint& pnt ) {
	x = pnt.x;
	y = pnt.y;
}



// Getters and Setters.
//
//
void ofxPoint2f::set( float _x, float _y ) {
	x = _x;
	y = _y;
}

void ofxPoint2f::set( const ofPoint& pnt ) {
	x = pnt.x;
	y = pnt.y;
}

float& ofxPoint2f::operator[]( const int& i ) {
	switch(i) {
		case 0:  return x;
		case 1:  return y;
		default: return x;
	}
}



// Check similarity/equality.
//
//
bool ofxPoint2f::operator==( const ofPoint& pnt ) {
	return (x == pnt.x) && (y == pnt.y);
}

bool ofxPoint2f::operator!=( const ofPoint& pnt ) {
	return (x != pnt.x) || (y != pnt.y);
}

bool ofxPoint2f::match( const ofPoint& pnt, float tollerance) {
	return (fabs(x - pnt.x) < tollerance)
		&& (fabs(y - pnt.y) < tollerance);
}



// Overloading for any type to any type
//
//
void ofxPoint2f::operator=( const ofPoint& vec ){
	x = vec.x;
	y = vec.y;
}

ofxPoint2f ofxPoint2f::operator+( const ofPoint& vec ) const {
	return ofxPoint2f( x+vec.x, y+vec.y);
}

ofxPoint2f& ofxPoint2f::operator+=( const ofPoint& vec ) {
	x += vec.x;
	y += vec.y;
	return *this;
}

ofxPoint2f ofxPoint2f::operator-( const ofPoint& vec ) const {
	return ofxPoint2f(x-vec.x, y-vec.y);
}

ofxPoint2f& ofxPoint2f::operator-=( const ofPoint& vec ) {
	x -= vec.x;
	y -= vec.y;
	return *this;
}

ofxPoint2f ofxPoint2f::operator*( const ofPoint& vec ) const {
	return ofxPoint2f(x*vec.x, y*vec.y);
}

ofxPoint2f& ofxPoint2f::operator*=( const ofPoint& vec ) {
	x*=vec.x;
	y*=vec.y;
	return *this;
}

ofxPoint2f ofxPoint2f::operator/( const ofPoint& vec ) const {
	return ofxPoint2f( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y);
}

ofxPoint2f& ofxPoint2f::operator/=( const ofPoint& vec ) {
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	return *this;
}


//operator overloading for float
//
//
void ofxPoint2f::operator=( const float f){
	x = f;
	y = f;
}

ofxPoint2f ofxPoint2f::operator+( const float f ) const {
	return ofxPoint2f( x+f, y+f);
}

ofxPoint2f& ofxPoint2f::operator+=( const float f ) {
	x += f;
	y += f;
	return *this;
}

ofxPoint2f ofxPoint2f::operator-( const float f ) const {
	return ofxPoint2f( x-f, y-f);
}

ofxPoint2f& ofxPoint2f::operator-=( const float f ) {
	x -= f;
	y -= f;
	return *this;
}

ofxPoint2f ofxPoint2f::operator-() const {
	return ofxPoint2f(-x, -y);
}

ofxPoint2f ofxPoint2f::operator*( const float f ) const {
	return ofxPoint2f(x*f, y*f);
}

ofxPoint2f& ofxPoint2f::operator*=( const float f ) {
	x*=f;
	y*=f;
	return *this;
}

ofxPoint2f ofxPoint2f::operator/( const float f ) const {
	if(f == 0) return ofxPoint2f(x, y);

	return ofxPoint2f(x/f, y/f);
}

ofxPoint2f& ofxPoint2f::operator/=( const float f ) {
	if(f == 0) return *this;

	x/=f;
	y/=f;
	return *this;
}


// Rotate point by angle (deg) around pivot point.
//
//

// This method is deprecated in 006 please use getRotated instead
ofxPoint2f ofxPoint2f::rotated( float angle, const ofxPoint2f& pivot ) const {
	return getRotated(angle, pivot);
}

ofxPoint2f ofxPoint2f::getRotated( float angle, const ofxPoint2f& pivot ) const {
	float a = (float)(angle * DEG_TO_RAD);
	return ofxPoint2f( ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x,
					  ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y );
}

ofxPoint2f& ofxPoint2f::rotate( float angle, const ofxPoint2f& pivot ) {
	float a = (float)(angle * DEG_TO_RAD);
	float xrot = ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x;
	y = ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y;
	x = xrot;
	return *this;
}

ofxPoint2f ofxPoint2f::getRotatedRad( float angle, const ofxPoint2f& pivot ) const {
	float a = angle;
	return ofxPoint2f( ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x,
					  ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y );
}

ofxPoint2f& ofxPoint2f::rotateRad( float angle, const ofxPoint2f& pivot ) {
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
ofxPoint2f ofxPoint2f::mapped( const ofxPoint2f& origin,
					  const ofxVec2f& vx,
					  const ofxVec2f& vy ) const{
	return getMapped(origin, vx, vy);
}

ofxPoint2f ofxPoint2f::getMapped( const ofxPoint2f& origin,
				  const ofxVec2f& vx,
				  const ofxVec2f& vy ) const
{
	return ofxPoint2f( origin.x + x*vx.x + y*vy.x,
					  origin.y + x*vx.y + y*vy.y );
}

ofxPoint2f& ofxPoint2f::map( const ofxPoint2f& origin,
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
float ofxPoint2f::distance( const ofxPoint2f& pnt) const {
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	return (float)sqrt(vx*vx + vy*vy);
}

//this method is deprecated in 006 please use squareDistance
float ofxPoint2f::distanceSquared( const ofxPoint2f& pnt ) const {
	return squareDistance(pnt);
}

float ofxPoint2f::squareDistance( const ofxPoint2f& pnt ) const {
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
ofxPoint2f ofxPoint2f::interpolated( const ofxPoint2f& pnt, float p ) const{
	return getInterpolated(pnt, p);
}

ofxPoint2f ofxPoint2f::getInterpolated( const ofxPoint2f& pnt, float p ) const {
	return ofxPoint2f( x*(1-p) + pnt.x*p, y*(1-p) + pnt.y*p );
}

ofxPoint2f& ofxPoint2f::interpolate( const ofxPoint2f& pnt, float p ) {
	x = x*(1-p) + pnt.x*p;
	y = y*(1-p) + pnt.y*p;
	return *this;
}

// this method is deprecated in 006 please use getMiddle
ofxPoint2f ofxPoint2f::middled( const ofxPoint2f& pnt ) const{
	return getMiddle(pnt);
}

ofxPoint2f ofxPoint2f::getMiddle( const ofxPoint2f& pnt ) const {
	return ofxPoint2f( (x+pnt.x)/2.0f, (y+pnt.y)/2.0f );
}

ofxPoint2f& ofxPoint2f::middle( const ofxPoint2f& pnt ) {
	x = (x+pnt.x)/2.0f;
	y = (y+pnt.y)/2.0f;
	return *this;
}



// Average (centroid) among points.
// Addition is sometimes useful for calculating averages too.
//
//
ofxPoint2f& ofxPoint2f::average( const ofxPoint2f* points, int num ) {
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


// Perpendicular normalized vector from two points.
//
//

// this method is deprecated in 006 use getPerpendicular
ofxVec2f ofxPoint2f::perpendicular( const ofxPoint2f& pnt ) const{
	return getPerpendicular(pnt);
}

ofxVec2f ofxPoint2f::getPerpendicular( const ofxPoint2f& pnt ) const {
	//float a = -atan2( pnt.y - y, pnt.x - x );
	//return ofxVec2f( sin(a), cos(a) );

	//the following seems simpler and about 3x faster

	float vy = -(x - pnt.x);
	float vx = y - pnt.y;
	float length = (float)sqrt(vx*vx + vy*vy);
	if( length > 0 )
		return ofxVec2f( vx/length, vy/length );
	else
		return ofxVec2f(x, y);
}



// Non-Member operators
//
//
ofxPoint2f operator+( float f, const ofxPoint2f& pnt ) {
    return ofxPoint2f( f+pnt.x, f+pnt.y);
}

ofxPoint2f operator-( float f, const ofxPoint2f& pnt ) {
    return ofxPoint2f( f-pnt.x, f-pnt.y);
}

ofxPoint2f operator*( float f, const ofxPoint2f& pnt ) {
    return ofxPoint2f( f*pnt.x, f*pnt.y);
}

ofxPoint2f operator/( float f, const ofxPoint2f& pnt ) {
    return ofxPoint2f( f/pnt.x, f/pnt.y);
}

