#pragma once

template <typename VecType>
class ofVec3_;

template <typename VecType>
class ofVec4_;
#include "ofConstants.h"

// class ofVec2_ : public ofVec_<VecType, 2>
template <typename VecType>
class ofVec2_ {
public:
	VecType x, y;
		
    static const int DIM = 2;
     
    ofVec2_();
	explicit ofVec2_( VecType _scalar );
	ofVec2_( VecType _x, VecType _y );
	ofVec2_( const ofVec3_< VecType > & vec );
    ofVec2_( const ofVec4_< VecType >& vec );
		
    //-----------------------------------------------
	// Accessors
	//	
	VecType* getPtr() {
		return (VecType*)&x;
	}
	
	const VecType* getPtr() const {
		return (const VecType *)&x;
	}
	
	VecType& operator[]( int n ) {
		return getPtr()[ n ];
	}
	
	VecType operator[]( int n ) const {
		return getPtr()[n];
	}
	
    //-----------------------------------------------
	// Getters and Setters.
    //
	void set( VecType _scalar );
    void set( VecType _x, VecType _y );
    void set( const ofVec2_< VecType >& vec );
	
    //-----------------------------------------------
    // Check similarity/equality.
    //
    bool operator==( const ofVec2_< VecType >& vec ) const;
    bool operator!=( const ofVec2_< VecType >& vec ) const;
    bool match( const ofVec2_< VecType >& vec, VecType tolerance=0.0001 ) const;

    //-----------------------------------------------
	// Checks if vectors look in the same direction.
	// 
    bool    isAligned( const ofVec2_< VecType >& vec, VecType tolerance=0.0001 ) const;
    bool isAlignedRad( const ofVec2_< VecType >& vec, VecType tolerance=0.0001 ) const;
    bool        align( const ofVec2_< VecType >& vec, VecType tolerance=0.0001 ) const;
    bool     alignRad( const ofVec2_< VecType >& vec, VecType tolerance=0.0001 ) const;
	
    //-----------------------------------------------
    // Operator overloading
    //
    ofVec2_< VecType >  operator +  ( const ofVec2_< VecType >& vec ) const;
    ofVec2_< VecType >& operator += ( const ofVec2_< VecType >& vec );
    ofVec2_< VecType >  operator -  ( const ofVec2_< VecType >& vec ) const;
    ofVec2_< VecType >& operator -= ( const ofVec2_< VecType >& vec );
    ofVec2_< VecType >  operator *  ( const ofVec2_< VecType >& vec ) const;
    ofVec2_< VecType >& operator *= ( const ofVec2_< VecType >& vec );
    ofVec2_< VecType >  operator /  ( const ofVec2_< VecType >& vec ) const;
    ofVec2_< VecType >& operator /= ( const ofVec2_< VecType >& vec );
		
	ofVec2_< VecType >& operator = ( const ofVec2_< VecType >& vec) {
		x = vec.x;
		y = vec.y;
		return *this;
	}
	
    //-----------------------------------------------
    // Operator overloading for VecType
    //
	//    void 	  operator=( const VecType f);
    ofVec2_< VecType >  operator +  ( const VecType f ) const;
    ofVec2_< VecType >& operator += ( const VecType f );
    ofVec2_< VecType >  operator -  ( const VecType f ) const;
    ofVec2_< VecType >& operator -= ( const VecType f );
    ofVec2_< VecType >  operator *  ( const VecType f ) const;
    ofVec2_< VecType >& operator *= ( const VecType f );
    ofVec2_< VecType >  operator /  ( const VecType f ) const;
    ofVec2_< VecType >& operator /= ( const VecType f );
    ofVec2_< VecType >  operator -  () const;
	
	template <typename S>
	friend ostream& operator<<(ostream& os, const ofVec2_<S>& vec);
	template <typename S>
	friend istream& operator>>(istream& is, const ofVec2_<S>& vec);
	
    //-----------------------------------------------
    // Scale
    //
    ofVec2_< VecType >  getScaled( const VecType length ) const;
    ofVec2_< VecType >& scale( const VecType length );
	
    //-----------------------------------------------
    // Rotation
    // 
    ofVec2_< VecType >     getRotated( VecType angle ) const;
    ofVec2_< VecType >  getRotatedRad( VecType angle ) const;
    ofVec2_< VecType >&        rotate( VecType angle );
    ofVec2_< VecType >&     rotateRad( VecType angle );
	
    //-----------------------------------------------
    // Rotation - point around pivot
    //
    ofVec2_< VecType >     getRotated( VecType angle, const ofVec2_< VecType >& pivot ) const;
    ofVec2_< VecType >&        rotate( VecType angle, const ofVec2_< VecType >& pivot );
    ofVec2_< VecType >  getRotatedRad( VecType angle, const ofVec2_< VecType >& pivot ) const;
    ofVec2_< VecType >&     rotateRad( VecType angle, const ofVec2_< VecType >& pivot );
	
    //-----------------------------------------------
    // Map point to coordinate system defined by origin, vx, and vy.
    //
    ofVec2_< VecType > getMapped( const ofVec2_< VecType >& origin,
                                  const ofVec2_< VecType >& vx,
					              const ofVec2_< VecType >& vy ) const;
    ofVec2_< VecType >&      map( const ofVec2_< VecType >& origin,
				                  const ofVec2_< VecType >& vx, 
                                  const ofVec2_< VecType >& vy );
	
    //-----------------------------------------------
    // Distance between two points.
    //
    VecType       distance( const ofVec2_< VecType >& pnt) const;
    VecType squareDistance( const ofVec2_< VecType >& pnt ) const;
	
    //-----------------------------------------------
    // Linear interpolation.
    //
	// p==0.0 results in this point, p==0.5 results in the
	// midpoint, and p==1.0 results in pnt being returned.
	// 
    ofVec2_< VecType >   getInterpolated( const ofVec2_< VecType >& pnt, VecType p ) const;
    ofVec2_< VecType >&      interpolate( const ofVec2_< VecType >& pnt, VecType p );
    ofVec2_< VecType >   getMiddle( const ofVec2_< VecType >& pnt ) const;
    ofVec2_< VecType >&     middle( const ofVec2_< VecType >& pnt );
    ofVec2_< VecType >&  average( const ofVec2_< VecType >* points, int num );
    
    //-----------------------------------------------
    // Normalization
    //
    ofVec2_< VecType >  getNormalized() const;
    ofVec2_< VecType >&     normalize();
	
    //-----------------------------------------------
    // Limit length.
    //
	ofVec2_< VecType >  getLimited( VecType max ) const;
    ofVec2_< VecType >&      limit( VecType max );
	
    //-----------------------------------------------
    // Perpendicular normalized vector.
    //
    ofVec2_< VecType >  getPerpendicular() const;
    ofVec2_< VecType >&    perpendicular();
	
    //-----------------------------------------------
    // Length
    //
    VecType length() const;
    VecType lengthSquared() const;
	OF_DEPRECATED_MSG("Use ofVec2_<VecType>::lengthSquared() instead.", VecType squareLength() const);
	
    //-----------------------------------------------
    // Angle
    // 
    /**
	 * Angle (deg) between two vectors.
	 * VecTypehis is a signed relative angle between -180 and 180.
	 */
    VecType angle( const ofVec2_< VecType >& vec ) const;
    VecType angleRad( const ofVec2_< VecType >& vec ) const;
	
    //-----------------------------------------------
    // Dot Product
    //
    VecType dot( const ofVec2_< VecType >& vec ) const;
	
    //-----------------------------------------------
    // DEPRECATED 006, please use:
    // getScaled
    ofVec2_<VecType> rescaled( const VecType length ) const;
    // scale
    ofVec2_<VecType>& rescale( const VecType length );
    // getRotated
    ofVec2_<VecType> rotated( VecType angle ) const;
    // getNormalized
    ofVec2_<VecType> normalized() const;
    // getLimited
    ofVec2_<VecType> limited(VecType max) const;
    // getPerpendicular
    ofVec2_<VecType> perpendiculared() const;
    // getInterpolated
    ofVec2_<VecType> interpolated( const ofVec2_<VecType>& pnt, VecType p ) const;
    // getMiddled
    ofVec2_<VecType> middled( const ofVec2_<VecType>& pnt ) const;
    // getMapped 
    ofVec2_<VecType> mapped( const ofVec2_<VecType>& origin, const ofVec2_<VecType>& vx, const ofVec2_<VecType>& vy ) const;
    // squareDistance
    VecType distanceSquared( const ofVec2_<VecType>& pnt ) const;
    // use getRotated
    ofVec2_<VecType> rotated( VecType angle, const ofVec2_<VecType>& pivot ) const;    

    // return all zero vector
    static ofVec2_< VecType > zero() { return ofVec2_< VecType >( 0, 0 ); }
    // return all one vector
    static ofVec2_< VecType > one() { return ofVec2_< VecType >( 1, 1 ); }
};

//-----------------------------------------------
// Non-Member operators
//
template <typename VecType>
ofVec2_< VecType > operator+( VecType f, const ofVec2_< VecType >& vec );
template <typename VecType>
ofVec2_< VecType > operator-( VecType f, const ofVec2_< VecType >& vec );
template <typename VecType>
ofVec2_< VecType > operator*( VecType f, const ofVec2_< VecType >& vec );
template <typename VecType>
ofVec2_< VecType > operator/( VecType f, const ofVec2_< VecType >& vec );

//-----------------------------------------------
// Getters and Setters.
//
template <typename VecType>
inline void ofVec2_< VecType >::set( VecType _scalar ) {
	x = _scalar;
	y = _scalar;
}

template <typename VecType>
inline void ofVec2_< VecType >::set( VecType _x, VecType _y ) {
	x = _x;
	y = _y;
}

template <typename VecType>
inline void ofVec2_< VecType >::set( const ofVec2_< VecType >& vec ) {
	x = vec.x;
	y = vec.y;
}

//-----------------------------------------------
// Check similarity/equality.
//
template <typename VecType>
inline bool ofVec2_< VecType >::operator==( const ofVec2_< VecType >& vec ) const {
	return (x == vec.x) && (y == vec.y);
}

template <typename VecType>
inline bool ofVec2_< VecType >::operator!=( const ofVec2_< VecType >& vec ) const {
	return (x != vec.x) || (y != vec.y);
}

template <typename VecType>
inline bool ofVec2_< VecType >::match( const ofVec2_< VecType >& vec, VecType tolerance ) const {
	return (fabs(x - vec.x) < tolerance)
	&& (fabs(y - vec.y) < tolerance);
}

//-----------------------------------------------
// Align
// 
// Checks if vectors look in the same direction.
// Tolerance is specified in degree.
// 
template <typename VecType>
inline bool ofVec2_< VecType >::isAligned( const ofVec2_< VecType >& vec, VecType tolerance ) const { 
	return  fabs( this->angle( vec ) ) < tolerance;
}

template <typename VecType>
inline bool ofVec2_< VecType >::align( const ofVec2_< VecType >& vec, VecType tolerance ) const {
    return isAligned( vec, tolerance );
}

template <typename VecType>
inline bool ofVec2_< VecType >::isAlignedRad( const ofVec2_< VecType >& vec, VecType tolerance ) const {
	return  fabs( this->angleRad( vec ) ) < tolerance;
}

template <typename VecType>
inline bool ofVec2_< VecType >::alignRad( const ofVec2_< VecType >& vec, VecType tolerance ) const {
    return isAlignedRad( vec, tolerance );
}

//-----------------------------------------------
// Overloading for any type to any type
//
template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::operator+( const ofVec2_< VecType >& vec ) const {
	return ofVec2_< VecType >( x+vec.x, y+vec.y);
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::operator+=( const ofVec2_< VecType >& vec ) {
	x += vec.x;
	y += vec.y;
	return *this;
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::operator-( const ofVec2_< VecType >& vec ) const {
	return ofVec2_< VecType >(x-vec.x, y-vec.y);
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::operator-=( const ofVec2_< VecType >& vec ) {
	x -= vec.x;
	y -= vec.y;
	return *this;
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::operator*( const ofVec2_< VecType >& vec ) const {
	return ofVec2_< VecType >(x*vec.x, y*vec.y);
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::operator*=( const ofVec2_< VecType >& vec ) {
	x*=vec.x;
	y*=vec.y;
	return *this;
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::operator/( const ofVec2_< VecType >& vec ) const {
	return ofVec2_< VecType >( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y);
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::operator/=( const ofVec2_< VecType >& vec ) {
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	return *this;
}

template <typename S>
inline ostream& operator<<(ostream& os, const ofVec2_<S>& vec) {
	os << vec.x << ", " << vec.y;
	return os;
}

template <typename S>
inline istream& operator>>(istream& is, ofVec2_<S>& vec) {
	is >> vec.x;
	is.ignore(2);
	is >> vec.y;
	return is;
}

//-----------------------------------------------
// Operator overloading for VecType
//
template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::operator+( const VecType f ) const {
	return ofVec2_< VecType >( x+f, y+f);
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::operator+=( const VecType f ) {
	x += f;
	y += f;
	return *this;
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::operator-( const VecType f ) const {
	return ofVec2_< VecType >( x-f, y-f);
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::operator-=( const VecType f ) {
	x -= f;
	y -= f;
	return *this;
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::operator-() const {
	return ofVec2_< VecType >(-x, -y);
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::operator*( const VecType f ) const {
	return ofVec2_< VecType >(x*f, y*f);
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::operator*=( const VecType f ) {
	x*=f;
	y*=f;
	return *this;
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::operator/( const VecType f ) const {
	if(f == 0) return ofVec2_< VecType >(x, y);
	
	return ofVec2_< VecType >(x/f, y/f);
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::operator/=( const VecType f ) {
	if(f == 0) return *this;
	
	x/=f;
	y/=f;
	return *this;
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::rescaled( const VecType length ) const {
	return getScaled(length);
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::getScaled( const VecType length ) const {
	VecType l = (VecType)sqrt(x*x + y*y);
	if( l > 0 )
		return ofVec2_< VecType >( (x/l)*length, (y/l)*length );
	else
		return ofVec2_< VecType >();
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::rescale( const VecType length ){
	return scale(length);
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::scale( const VecType length ) {
	VecType l = (VecType)sqrt(x*x + y*y);
	if (l > 0) {
		x = (x/l)*length;
		y = (y/l)*length;
	}
	return *this;
}

//-----------------------------------------------
// Rotation
//
template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::rotated( VecType angle ) const {
	return getRotated(angle);
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::getRotated( VecType angle ) const {
	VecType a = (VecType)(angle*DEG_TO_RAD);
	return ofVec2_< VecType >( x*cos(a) - y*sin(a),
				   x*sin(a) + y*cos(a) );
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::getRotatedRad( VecType angle ) const {
	VecType a = angle;
	return ofVec2_< VecType >( x*cos(a) - y*sin(a),
				   x*sin(a) + y*cos(a) );
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::rotate( VecType angle ) {
	VecType a = (VecType)(angle * DEG_TO_RAD);
	VecType xrot = x*cos(a) - y*sin(a);
	y = x*sin(a) + y*cos(a);
	x = xrot;
	return *this;
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::rotateRad( VecType angle ) {
	VecType a = angle;
	VecType xrot = x*cos(a) - y*sin(a);
	y = x*sin(a) + y*cos(a);
	x = xrot;
	return *this;
}

//-----------------------------------------------
// Rotate point by angle (deg) around pivot point.
//
// This method is deprecated in 006 please use getRotated instead
template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::rotated( VecType angle, const ofVec2_< VecType >& pivot ) const {
	return getRotated(angle, pivot);
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::getRotated( VecType angle, const ofVec2_< VecType >& pivot ) const {
	VecType a = (VecType)(angle * DEG_TO_RAD);
	return ofVec2_< VecType >( ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x,
				   ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y );
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::rotate( VecType angle, const ofVec2_< VecType >& pivot ) {
	VecType a = (VecType)(angle * DEG_TO_RAD);
	VecType xrot = ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x;
	y = ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y;
	x = xrot;
	return *this;
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::getRotatedRad( VecType angle, const ofVec2_< VecType >& pivot ) const {
	VecType a = angle;
	return ofVec2_< VecType >( ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x,
				   ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y );
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::rotateRad( VecType angle, const ofVec2_< VecType >& pivot ) {
	VecType a = angle;
	VecType xrot = ((x-pivot.x)*cos(a) - (y-pivot.y)*sin(a)) + pivot.x;
	y = ((x-pivot.x)*sin(a) + (y-pivot.y)*cos(a)) + pivot.y;
	x = xrot;
	return *this;
}

//-----------------------------------------------
// Map point to coordinate system defined by origin, vx, and vy.
//
// This method is deprecated in 006 please use getMapped instead
template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::mapped( const ofVec2_< VecType >& origin,
							   const ofVec2_< VecType >& vx,
							   const ofVec2_< VecType >& vy ) const{
	return getMapped(origin, vx, vy);
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::getMapped( const ofVec2_< VecType >& origin,
								  const ofVec2_< VecType >& vx,
								  const ofVec2_< VecType >& vy ) const
{
	return ofVec2_< VecType >( origin.x + x*vx.x + y*vy.x,
				   origin.y + x*vx.y + y*vy.y );
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::map( const ofVec2_< VecType >& origin,
							 const ofVec2_< VecType >& vx, const ofVec2_< VecType >& vy )
{
	VecType xmap = origin.x + x*vx.x + y*vy.x;
	y = origin.y + x*vx.y + y*vy.y;
	x = xmap;
	return *this;
}

//-----------------------------------------------
// Distance between two points.
//
template <typename VecType>
inline VecType ofVec2_<VecType>::distance( const ofVec2_<VecType>& pnt) const {
	VecType vx = x-pnt.x;
	VecType vy = y-pnt.y;
	return (VecType)sqrt(vx*vx + vy*vy);
}

//this method is deprecated in 006 please use squareDistance
template <typename VecType>
inline VecType ofVec2_< VecType >::distanceSquared( const ofVec2_< VecType >& pnt ) const {
	return squareDistance(pnt);
}

template <typename VecType>
inline VecType ofVec2_< VecType >::squareDistance( const ofVec2_< VecType >& pnt ) const {
	VecType vx = x-pnt.x;
	VecType vy = y-pnt.y;
	return vx*vx + vy*vy;
}

//-----------------------------------------------
// Linear interpolation.
//
// p==0.0 results in this point, p==0.5 results in the
// midpoint, and p==1.0 results in pnt being returned.
// 
// this method is deprecated in 006 please use getInterpolated
template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::interpolated( const ofVec2_< VecType >& pnt, VecType p ) const{
	return getInterpolated(pnt, p);
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::getInterpolated( const ofVec2_< VecType >& pnt, VecType p ) const {
	return ofVec2_< VecType >( x*(1-p) + pnt.x*p, y*(1-p) + pnt.y*p );
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::interpolate( const ofVec2_< VecType >& pnt, VecType p ) {
	x = x*(1-p) + pnt.x*p;
	y = y*(1-p) + pnt.y*p;
	return *this;
}

// this method is deprecated in 006 please use getMiddle
template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::middled( const ofVec2_< VecType >& pnt ) const{
	return getMiddle(pnt);
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::getMiddle( const ofVec2_< VecType >& pnt ) const {
	return ofVec2_< VecType >( (x+pnt.x)/2.0f, (y+pnt.y)/2.0f );
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::middle( const ofVec2_< VecType >& pnt ) {
	x = (x+pnt.x)/2.0f;
	y = (y+pnt.y)/2.0f;
	return *this;
}

//-----------------------------------------------
// Average (centroid) among points.
// Addition is sometimes useful for calculating averages too.
//
template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::average( const ofVec2_< VecType >* points, int num ) {
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

//-----------------------------------------------
// Normalization
//
template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::normalized() const {
	return getNormalized();
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::getNormalized() const {
	VecType length = (VecType)sqrt(x*x + y*y);
	if( length > 0 ) {
		return ofVec2_< VecType >( x/length, y/length );
	} else {
		return ofVec2_< VecType >();
	}
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::normalize() {
	VecType length = (VecType)sqrt(x*x + y*y);
	if( length > 0 ) {
		x /= length;
		y /= length;
	}
	return *this;
}

//-----------------------------------------------
// Limit length.
//
template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::limited(VecType max) const{
	return getLimited(max);
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::getLimited(VecType max) const {
    ofVec2_< VecType > limited;
    VecType lengthSquared = (x*x + y*y);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        VecType ratio = max/(VecType)sqrt(lengthSquared);
        limited.set( x*ratio, y*ratio);
    } else {
        limited.set(x,y);
    }
    return limited;
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::limit(VecType max) {
    VecType lengthSquared = (x*x + y*y);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        VecType ratio = max/(VecType)sqrt(lengthSquared);
        x *= ratio;
        y *= ratio;
    }
    return *this;
}

//-----------------------------------------------
// Perpendicular normalized vector.
//
template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::perpendiculared() const {
	return getPerpendicular();
}

template <typename VecType>
inline ofVec2_< VecType > ofVec2_< VecType >::getPerpendicular() const {
	VecType length = (VecType)sqrt( x*x + y*y );
	if( length > 0 )
		return ofVec2_< VecType >( -(y/length), x/length );
	else
		return ofVec2_< VecType >();
}

template <typename VecType>
inline ofVec2_< VecType >& ofVec2_< VecType >::perpendicular() {
	VecType length = (VecType)sqrt( x*x + y*y );
	if( length > 0 ) {
		VecType _x = x;
		x = -(y/length);
		y = _x/length;
	}
	return *this;
}

//-----------------------------------------------
// Length
//
template <typename VecType>
inline VecType ofVec2_< VecType >::length() const {
	return (VecType)sqrt( x*x + y*y );
}

template <typename VecType>
inline VecType ofVec2_< VecType >::lengthSquared() const {
	return (VecType)(x*x + y*y);
}

template <typename VecType>
inline VecType ofVec2_< VecType >::squareLength() const {
	return lengthSquared();
}

//-----------------------------------------------
// Angle
//
/**
 * Angle (deg) between two vectors.
 * VecTypehis is a signed relative angle between -180 and 180.
 */
template <typename VecType>
inline VecType ofVec2_< VecType >::angle( const ofVec2_< VecType >& vec ) const {
	return (VecType)(atan2( x*vec.y-y*vec.x, x*vec.x + y*vec.y )*RAD_TO_DEG);
}

/**
 * Angle (deg) between two vectors.
 * VecTypehis is a signed relative angle between -180 and 180.
 */
template <typename VecType>
inline VecType ofVec2_< VecType >::angleRad( const ofVec2_< VecType >& vec ) const {
	return atan2( x*vec.y-y*vec.x, x*vec.x + y*vec.y );
}

//-----------------------------------------------
// Dot product
//
template <typename VecType>
inline VecType ofVec2_< VecType >::dot( const ofVec2_< VecType >& vec ) const {
	return x*vec.x + y*vec.y;
}

//-----------------------------------------------
// Non-Member operators
//
template <typename VecType>
inline ofVec2_< VecType > operator+( VecType f, const ofVec2_< VecType >& vec ) {
    return ofVec2_< VecType >( f+vec.x, f+vec.y);
}

template <typename VecType>
inline ofVec2_< VecType > operator-( VecType f, const ofVec2_< VecType >& vec ) {
    return ofVec2_< VecType >( f-vec.x, f-vec.y);
}

template <typename VecType>
inline ofVec2_< VecType > operator*( VecType f, const ofVec2_< VecType >& vec ) {
    return ofVec2_< VecType >( f*vec.x, f*vec.y);
}

template <typename VecType>
inline ofVec2_< VecType > operator/( VecType f, const ofVec2_< VecType >& vec ) {
    return ofVec2_< VecType >( f/vec.x, f/vec.y);
}

//-----------------------------------------------
// Expand templates
//
typedef ofVec2_< float > ofVec2f;
