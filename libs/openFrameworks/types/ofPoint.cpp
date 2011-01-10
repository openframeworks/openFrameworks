#include "ofPoint.h"


ofPoint::ofPoint (){ 
	x = y = z = 0.0f; 
}

ofPoint::~ ofPoint (){}

ofPoint::ofPoint (float _x, float _y, float _z){
	x = _x;
	y = _y;
	z = _z;
}

ofPoint::ofPoint (ofPoint const & pnt){
	x = pnt.x;
	y = pnt.y;
	z = pnt.z;
}

void ofPoint::set (float _x, float _y, float _z){
	x = _x;
	y = _y;
	z = _z;
}

void ofPoint::set (ofPoint const & pnt){
	x = pnt.x;
	y = pnt.y;
	z = pnt.z;
}

ofPoint ofPoint::operator - () const{
	return ofPoint( -x, -y, -z );
}

bool ofPoint::operator == (ofPoint const & pnt){
	return (x == pnt.x) && (y == pnt.y) && (z == pnt.z);
}

bool ofPoint::operator != (ofPoint const & pnt){
	return (x != pnt.x) || (y != pnt.y) || (z != pnt.z);
}

ofPoint & ofPoint::operator = (ofPoint const & pnt){
	x = pnt.x;
	y = pnt.y;
	z = pnt.z;
	return *this;
}

ofPoint & ofPoint::operator = (float const & val){
	x = val;
	y = val;
	z = val;
	return *this;
}

ofPoint ofPoint::operator + (ofPoint const & pnt) const{
	return ofPoint( x+pnt.x, y+pnt.y, z+pnt.z );
}

ofPoint ofPoint::operator + (float const & val) const{
	return ofPoint( x+val, y+val, z+val );
}

ofPoint & ofPoint::operator += (ofPoint const & pnt){
	x+=pnt.x;
	y+=pnt.y;
	z+=pnt.z;
	return *this;
}

ofPoint & ofPoint::operator += (float const & val){
	x+=val;
	y+=val;
	z+=val;
	return *this;
}

ofPoint ofPoint::operator - (ofPoint const & pnt) const{
	return ofPoint( x-pnt.x, y-pnt.y, z-pnt.z );
}

ofPoint ofPoint::operator - (float const & val) const{
	return ofPoint( x-val, y-val, z-val);
}

ofPoint & ofPoint::operator -= (ofPoint const & pnt){
	x -= pnt.x;
	y -= pnt.y;
	z -= pnt.z;
	return *this;
}

ofPoint & ofPoint::operator -= (float const & val){
	x -= val;
	y -= val;
	z -= val;
	return *this;
}

ofPoint ofPoint::operator * (ofPoint const & pnt) const{
	return ofPoint( x*pnt.x, y*pnt.y, z*pnt.z );
}

ofPoint ofPoint::operator * (float const & val) const{
	return ofPoint( x*val, y*val, z*val);
}

ofPoint & ofPoint::operator *= (ofPoint const & pnt){
	x*=pnt.x;
	y*=pnt.y;
	z*=pnt.z;
	return *this;
}

ofPoint & ofPoint::operator *= (float const & val){
	x*=val;
	y*=val;
	z*=val;
	return *this;
}

ofPoint ofPoint::operator / (ofPoint const & pnt) const{
	return ofPoint( pnt.x!=0 ? x/pnt.x : x , pnt.y!=0 ? y/pnt.y : y, pnt.z!=0 ? z/pnt.z : z );
}

ofPoint ofPoint::operator / (float const & val) const{
	if( val != 0){
		return ofPoint( x/val, y/val, z/val );
	}
	return ofPoint(x, y, z );
}

ofPoint & ofPoint::operator /= (ofPoint const & pnt){
	pnt.x!=0 ? x/=pnt.x : x;
	pnt.y!=0 ? y/=pnt.y : y;
	pnt.z!=0 ? z/=pnt.z : z;
	return *this;
}

ofPoint & ofPoint::operator /= (float const & val){
	if( val != 0 ){
		x /= val;
		y /= val;
		z /= val;
	}
	return *this;
}
