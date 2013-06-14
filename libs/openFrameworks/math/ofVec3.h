#pragma once

#include "ofVec2.h"
#include "ofVec4.h"
#include "ofConstants.h"

#include <cmath>
#include <iostream>

// class ofVec3_ : public ofVec_<VecType, 2>
template <typename VecType>
class ofVec3_ {
public:
	VecType x, y, z;
	
    static const int DIM = 3;
	
	ofVec3_();
	ofVec3_( VecType _x, VecType _y, VecType _z=0 );
	explicit ofVec3_( VecType _scalar );
    ofVec3_( const ofVec2_< VecType >& vec );
    ofVec3_( const ofVec4_< VecType >& vec );
	
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
    void set( VecType _x, VecType _y, VecType _z = 0 );
    void set( const ofVec3_< VecType >& vec );
	void set( VecType _scalar );
	
    //-----------------------------------------------
    // Check similarity/equality.
    //
    bool operator == ( const ofVec3_< VecType >& vec ) const;
    bool operator != ( const ofVec3_< VecType >& vec ) const;
    bool match( const ofVec3_< VecType >& vec, VecType tolerance=0.0001 ) const;
    
    //-----------------------------------------------
	// Checks if vectors look in the same direction.
	// 
    bool    isAligned( const ofVec3_< VecType >& vec, VecType tolerance=0.0001 ) const;
    bool        align( const ofVec3_< VecType >& vec, VecType tolerance=0.0001 ) const;
    bool isAlignedRad( const ofVec3_< VecType >& vec, VecType tolerance=0.0001 ) const;
    bool     alignRad( const ofVec3_< VecType >& vec, VecType tolerance=0.0001 ) const;
	
    //-----------------------------------------------
    // Operator overloading
    //
    ofVec3_<VecType>  operator +  ( const ofVec3_< VecType >& pnt ) const;
    ofVec3_<VecType>& operator += ( const ofVec3_< VecType >& pnt );
    ofVec3_<VecType>  operator -  ( const ofVec3_< VecType >& vec ) const;
    ofVec3_<VecType>& operator -= ( const ofVec3_< VecType >& vec );
    ofVec3_<VecType>  operator *  ( const ofVec3_< VecType >& vec ) const;
    ofVec3_<VecType>& operator *= ( const ofVec3_< VecType >& vec );
    ofVec3_<VecType>  operator /  ( const ofVec3_< VecType >& vec ) const;
    ofVec3_<VecType>& operator /= ( const ofVec3_< VecType >& vec );
    ofVec3_<VecType>  operator -  () const;
		
	ofVec3_<VecType>& operator =  (const ofVec3_< VecType >& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}
	
    //-----------------------------------------------
    // Operator overloading for VecType
    //
	//  void 	  operator=( const VecType f);
    ofVec3_< VecType >  operator +  ( const VecType f ) const;
    ofVec3_< VecType >& operator += ( const VecType f );
 	ofVec3_< VecType >  operator -  ( const VecType f ) const;
    ofVec3_< VecType >& operator -= ( const VecType f );
    ofVec3_< VecType >  operator *  ( const VecType f ) const;
    ofVec3_< VecType >& operator *= ( const VecType f );
    ofVec3_< VecType >  operator /  ( const VecType f ) const;
    ofVec3_< VecType >& operator /= ( const VecType f );
	
    template <typename S>
	friend ostream& operator << ( ostream& os, const ofVec3_< S >& vec );
    template <typename S>
	friend istream& operator >> ( istream& is, const ofVec3_< S >& vec );

    //-----------------------------------------------
    // Scale
    //
    ofVec3_< VecType >  getScaled( const VecType length ) const;
    ofVec3_< VecType >&     scale( const VecType length );
    
    //-----------------------------------------------
    // Rotation
    // 
    ofVec3_< VecType >     getRotated( VecType angle, const ofVec3_< VecType >& axis ) const;
    ofVec3_< VecType >  getRotatedRad( VecType angle, const ofVec3_< VecType >& axis ) const;
    ofVec3_< VecType >&        rotate( VecType angle, const ofVec3_< VecType >& axis );
    ofVec3_< VecType >&     rotateRad( VecType angle, const ofVec3_< VecType >& axis );

    ofVec3_< VecType >     getRotated( VecType ax, VecType ay, VecType az ) const;
    ofVec3_< VecType >  getRotatedRad( VecType ax, VecType ay, VecType az ) const;
    ofVec3_< VecType >&        rotate( VecType ax, VecType ay, VecType az );
    ofVec3_< VecType >&     rotateRad( VecType ax, VecType ay, VecType az );
    
    //-----------------------------------------------
    // Rotation - point around pivot
    //    
    ofVec3_< VecType >      getRotated( VecType angle, 
                                        const ofVec3_< VecType >& pivot, 
                                        const ofVec3_< VecType >& axis ) const;
    ofVec3_< VecType >&         rotate( VecType angle, 
                                        const ofVec3_< VecType >& pivot, 
                                        const ofVec3_< VecType >& axis );
    ofVec3_< VecType >   getRotatedRad( VecType angle, 
                                        const ofVec3_< VecType >& pivot, 
                                        const ofVec3_< VecType >& axis ) const;
    ofVec3_< VecType >&      rotateRad( VecType angle, 
                                        const ofVec3_< VecType >& pivot, 
                                        const ofVec3_< VecType >& axis );    
	
    //-----------------------------------------------
    // Map point to coordinate system defined by origin, vx, vy, and vz.
    //
    ofVec3_< VecType > getMapped( const ofVec3_< VecType >& origin,
					              const ofVec3_< VecType >& vx,
					              const ofVec3_< VecType >& vy,
					              const ofVec3_< VecType >& vz ) const;
    ofVec3_< VecType >&      map( const ofVec3_< VecType >& origin,
				                  const ofVec3_< VecType >& vx,
				                  const ofVec3_< VecType >& vy,
				                  const ofVec3_< VecType >& vz );
	
    //-----------------------------------------------
    // Distance between two points.
    //
    VecType       distance( const ofVec3_< VecType >& pnt ) const;
    VecType squareDistance( const ofVec3_< VecType >& pnt ) const;
	
    //-----------------------------------------------
    // Linear interpolation.
	// 
    ofVec3_< VecType >   getInterpolated( const ofVec3_<VecType>& pnt, VecType p ) const;
    ofVec3_< VecType >&      interpolate( const ofVec3_<VecType>& pnt, VecType p );

    ofVec3_< VecType >   getMiddle( const ofVec3_<VecType>& pnt ) const;
    ofVec3_< VecType >&     middle( const ofVec3_<VecType>& pnt );

    ofVec3_< VecType >&  average( const ofVec3_<VecType>* points, int num );
    
    //-----------------------------------------------
    // Normalization
    //
    ofVec3_< VecType >  getNormalized() const;
    ofVec3_< VecType >&     normalize();
	
    //-----------------------------------------------
    // Limit length.
    //
    ofVec3_<VecType>  getLimited( VecType max ) const;
    ofVec3_<VecType>&      limit( VecType max );
	
    //-----------------------------------------------
    // Perpendicular vector.
    //
    ofVec3_<VecType>  getCrossed( const ofVec3_< VecType >& vec ) const;
    ofVec3_<VecType>&      cross( const ofVec3_< VecType >& vec );

	// Normalized perpendicular.
	// 
    ofVec3_<VecType>  getPerpendicular( const ofVec3_< VecType >& vec ) const;
    ofVec3_<VecType>&    perpendicular( const ofVec3_< VecType >& vec );
	
    //-----------------------------------------------
    // Length
    //
    VecType length() const;
    VecType lengthSquared() const;
	OF_DEPRECATED_MSG("Use ofVec3_<VecType>::lengthSquared() instead.", VecType squareLength() const);

    //-----------------------------------------------
    // Angle
    // 
    VecType    angle( const ofVec3_< VecType >& vec ) const;
    VecType angleRad( const ofVec3_< VecType >& vec ) const;
	
    //-----------------------------------------------
    // Dot Product
    //
    VecType dot( const ofVec3_< VecType >& vec ) const;
	
    //-----------------------------------------------
    // DEPRECATED 006, please use:
    // getScaled
    ofVec3_<VecType> rescaled( const VecType length ) const;
    // scale
    ofVec3_<VecType>& rescale( const VecType length );
    // getRotated
    ofVec3_<VecType> rotated( VecType angle, const ofVec3_<VecType>& axis ) const;
    // getRotated should this be const???
    ofVec3_<VecType> rotated(VecType ax, VecType ay, VecType az);
    // getNormalized
    ofVec3_<VecType> normalized() const;
    // getLimited
    ofVec3_<VecType> limited(VecType max) const;
    // getCrossed
    ofVec3_<VecType> crossed( const ofVec3_<VecType>& vec ) const;
    // getPerpendicular
    ofVec3_<VecType> perpendiculared( const ofVec3_<VecType>& vec ) const;
    // use getMapped
    ofVec3_<VecType>  mapped( const ofVec3_<VecType>& origin,
					          const ofVec3_<VecType>& vx,
					          const ofVec3_<VecType>& vy,
					          const ofVec3_<VecType>& vz ) const;
    // use squareDistance
    VecType  distanceSquared( const ofVec3_<VecType>& pnt ) const;
	
    // use getInterpolated
    ofVec3_<VecType> interpolated( const ofVec3_<VecType>& pnt, VecType p ) const;
	
    // use getMiddle
    ofVec3_<VecType> middled( const ofVec3_<VecType>& pnt ) const;
    
    // use getRotated
    ofVec3_<VecType> rotated( VecType angle,
				              const ofVec3_<VecType>& pivot,
						      const ofVec3_<VecType>& axis ) const;    

    // return all zero vector
    static ofVec3_<VecType> zero() { return ofVec3_<VecType>(0, 0, 0); }
    
    // return all one vector
    static ofVec3_<VecType> one() { return ofVec3_<VecType>(1, 1, 1); }
};

//-----------------------------------------------
// Non-Member operators
//
template <typename VecType>
ofVec3_< VecType > operator+( VecType f, const ofVec3_< VecType >& vec );
template <typename VecType>
ofVec3_< VecType > operator-( VecType f, const ofVec3_< VecType >& vec );
template <typename VecType>
ofVec3_< VecType > operator*( VecType f, const ofVec3_< VecType >& vec );
template <typename VecType>
ofVec3_< VecType > operator/( VecType f, const ofVec3_< VecType >& vec );

//-----------------------------------------------
// Implementation
//
template <typename VecType>
inline ofVec3_< VecType >::ofVec3_()
//    : VEC3_REFS(),
//	  VEC_(3)
	{
	x = 0;
	y = 0;
	z = 0;
}

template <typename VecType>
inline ofVec3_< VecType >::ofVec3_( VecType _all )
//    : VEC3_REFS(),/
//	  VEC_WITH( 3, _all )
    {
	x = _all;
	y = _all;
	z = _all;
}

template <typename VecType>
inline ofVec3_< VecType >::ofVec3_( const ofVec2_< VecType >& vec )
//    : VEC3_REFS(),
//	  VEC_WITH(3, vec)
	{
	x = vec.x;
	y = vec.y;
	z = 0;
}

template <typename VecType>
inline ofVec3_< VecType >::ofVec3_( const ofVec4_< VecType >& vec )
//    : VEC3_REFS(),
//     VEC_WITH(3, vec)
	{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

template <typename VecType>
inline ofVec3_< VecType >::ofVec3_( VecType _x, VecType _y, VecType _z )
//    : VEC3_REFS(),
//	  VEC_(3)
	{
	x = _x;
	y = _y;
	z = _z;
}

//-----------------------------------------------
// Getters and Setters.
//
template <typename VecType>
inline void ofVec3_< VecType >::set( VecType _scalar ) {
	x = _scalar;
	y = _scalar;
	z = _scalar;
}

template <typename VecType>
inline void ofVec3_< VecType >::set( VecType _x, VecType _y, VecType _z ) {
	x = _x;
	y = _y;
	z = _z;
}

template <typename VecType>
inline void ofVec3_< VecType >::set( const ofVec3_< VecType >& vec ) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

//-----------------------------------------------
// Check similarity/equality.
//
template <typename VecType>
inline bool ofVec3_< VecType >::operator==( const ofVec3_< VecType >& vec ) const {
	return (x == vec.x) && (y == vec.y) && (z == vec.z);
}

template <typename VecType>
inline bool ofVec3_< VecType >::operator!=( const ofVec3_< VecType >& vec ) const {
	return (x != vec.x) || (y != vec.y) || (z != vec.z);
}

template <typename VecType>
inline bool ofVec3_< VecType >::match( const ofVec3_< VecType >& vec, VecType tolerance ) const{
	return (fabs(x - vec.x) < tolerance)
	&& (fabs(y - vec.y) < tolerance)
	&& (fabs(z - vec.z) < tolerance);
}

//-----------------------------------------------
// Align
/**
 * Checks if vectors look in the same direction.
 */
template <typename VecType>
inline bool ofVec3_< VecType >::isAligned( const ofVec3_< VecType >& vec, VecType tolerance ) const {
	VecType angle = this->angle( vec );
	return  angle < tolerance;
}

template <typename VecType>
inline bool ofVec3_< VecType >::align( const ofVec3_< VecType >& vec, VecType tolerance ) const {
    return isAligned( vec, tolerance );
}

template <typename VecType>
inline bool ofVec3_< VecType >::isAlignedRad( const ofVec3_< VecType >& vec, VecType tolerance ) const {
	VecType angle = this->angleRad( vec );
	return  angle < tolerance;
}

template <typename VecType>
inline bool ofVec3_< VecType >::alignRad( const ofVec3_< VecType >& vec, VecType tolerance ) const {
    return isAlignedRad( vec, tolerance );
}

//-----------------------------------------------
// Operator overloading for ofVec3_< VecType >
//
template <typename S>
inline ostream& operator<<(ostream& os, const ofVec3_<S>& vec) {
	os << vec.x << ", " << vec.y << ", " << vec.z;
	return os;
}

template <typename S>
inline istream& operator>>(istream& is, ofVec3_<S>& vec) {
	is >> vec.x;
	is.ignore(2);
	is >> vec.y;
	is.ignore(2);
	is >> vec.z;
	return is;
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::operator+( const ofVec3_< VecType >& pnt ) const {
	return ofVec3_< VecType >( x+pnt.x, y+pnt.y, z+pnt.z );
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::operator+=( const ofVec3_< VecType >& pnt ) {
	x+=pnt.x;
	y+=pnt.y;
	z+=pnt.z;
	return *this;
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::operator-( const ofVec3_< VecType >& vec ) const {
	return ofVec3_< VecType >( x-vec.x, y-vec.y, z-vec.z );
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::operator-=( const ofVec3_< VecType >& vec ) {
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::operator*( const ofVec3_< VecType >& vec ) const {
	return ofVec3_< VecType >( x*vec.x, y*vec.y, z*vec.z );
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::operator*=( const ofVec3_< VecType >& vec ) {
	x*=vec.x;
	y*=vec.y;
	z*=vec.z;
	return *this;
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::operator/( const ofVec3_< VecType >& vec ) const {
	return ofVec3_< VecType >( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y, vec.z!=0 ? z/vec.z : z );
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::operator/=( const ofVec3_< VecType >& vec ) {
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	vec.z!=0 ? z/=vec.z : z;
	return *this;
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::operator-() const {
	return ofVec3_< VecType >( -x, -y, -z );
}

//-----------------------------------------------
//operator overloading for VecType
//
//template <typename VecType>
//inline void ofVec3_< VecType >::operator=( const VecType f){
//	x = f;
//	y = f;
//	z = f;
//}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::operator+( const VecType f ) const {
	return ofVec3_< VecType >( x+f, y+f, z+f);
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::operator+=( const VecType f ) {
	x += f;
	y += f;
	z += f;
	return *this;
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::operator-( const VecType f ) const {
	return ofVec3_< VecType >( x-f, y-f, z-f);
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::operator-=( const VecType f ) {
	x -= f;
	y -= f;
	z -= f;
	return *this;
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::operator*( const VecType f ) const {
	return ofVec3_< VecType >( x*f, y*f, z*f );
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::operator*=( const VecType f ) {
	x*=f;
	y*=f;
	z*=f;
	return *this;
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::operator/( const VecType f ) const {
	if(f == 0) return ofVec3_< VecType >( x, y, z);
	
	return ofVec3_< VecType >( x/f, y/f, z/f );
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::operator/=( const VecType f ) {
	if(f == 0) return *this;
	
	x/=f;
	y/=f;
	z/=f;
	return *this;
}

//-----------------------------------------------
// Scale
//
template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::rescaled( const VecType length ) const {
	return getScaled(length);
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::getScaled( const VecType length ) const {
	VecType l = (VecType)sqrt(x*x + y*y + z*z);
	if( l > 0 )
		return ofVec3_< VecType >( (x/l)*length, (y/l)*length, (z/l)*length );
	else
		return ofVec3_< VecType >();
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::rescale( const VecType length ) {
	return scale(length);
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::scale( const VecType length ) {
	VecType l = (VecType)sqrt(x*x + y*y + z*z);
	if (l > 0) {
		x = (x/l)*length;
		y = (y/l)*length;
		z = (z/l)*length;
	}
	return *this;
}

//-----------------------------------------------
// Rotation
//
template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::rotated( VecType angle, const ofVec3_< VecType >& axis ) const {
	return getRotated(angle, axis);
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::getRotated( VecType angle, const ofVec3_< VecType >& axis ) const {
	ofVec3_< VecType > ax = axis.normalized();
	VecType a = (VecType)(angle*DEG_TO_RAD);
	VecType sina = sin( a );
	VecType cosa = cos( a );
	VecType cosb = 1.0f - cosa;
	
	return ofVec3_< VecType >( x*(ax.x*ax.x*cosb + cosa)
				   + y*(ax.x*ax.y*cosb - ax.z*sina)
				   + z*(ax.x*ax.z*cosb + ax.y*sina),
				   x*(ax.y*ax.x*cosb + ax.z*sina)
				   + y*(ax.y*ax.y*cosb + cosa)
				   + z*(ax.y*ax.z*cosb - ax.x*sina),
				   x*(ax.z*ax.x*cosb - ax.y*sina)
				   + y*(ax.z*ax.y*cosb + ax.x*sina)
				   + z*(ax.z*ax.z*cosb + cosa) );
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::getRotatedRad( VecType angle, const ofVec3_< VecType >& axis ) const {
	ofVec3_< VecType > ax = axis.normalized();
	VecType a = angle;
	VecType sina = sin( a );
	VecType cosa = cos( a );
	VecType cosb = 1.0f - cosa;
	
	return ofVec3_< VecType >( x*(ax.x*ax.x*cosb + cosa)
				   + y*(ax.x*ax.y*cosb - ax.z*sina)
				   + z*(ax.x*ax.z*cosb + ax.y*sina),
				   x*(ax.y*ax.x*cosb + ax.z*sina)
				   + y*(ax.y*ax.y*cosb + cosa)
				   + z*(ax.y*ax.z*cosb - ax.x*sina),
				   x*(ax.z*ax.x*cosb - ax.y*sina)
				   + y*(ax.z*ax.y*cosb + ax.x*sina)
				   + z*(ax.z*ax.z*cosb + cosa) );
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::rotate( VecType angle, const ofVec3_< VecType >& axis ) {
	ofVec3_< VecType > ax = axis.normalized();
	VecType a = (VecType)(angle*DEG_TO_RAD);
	VecType sina = sin( a );
	VecType cosa = cos( a );
	VecType cosb = 1.0f - cosa;
	
	VecType nx = x*(ax.x*ax.x*cosb + cosa)
	+ y*(ax.x*ax.y*cosb - ax.z*sina)
	+ z*(ax.x*ax.z*cosb + ax.y*sina);
	VecType ny = x*(ax.y*ax.x*cosb + ax.z*sina)
	+ y*(ax.y*ax.y*cosb + cosa)
	+ z*(ax.y*ax.z*cosb - ax.x*sina);
	VecType nz = x*(ax.z*ax.x*cosb - ax.y*sina)
	+ y*(ax.z*ax.y*cosb + ax.x*sina)
	+ z*(ax.z*ax.z*cosb + cosa);
	x = nx; y = ny; z = nz;
	return *this;
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::rotateRad(VecType angle, const ofVec3_< VecType >& axis ) {
	ofVec3_< VecType > ax = axis.normalized();
	VecType a = angle;
	VecType sina = sin( a );
	VecType cosa = cos( a );
	VecType cosb = 1.0f - cosa;
	
	VecType nx = x*(ax.x*ax.x*cosb + cosa)
	+ y*(ax.x*ax.y*cosb - ax.z*sina)
	+ z*(ax.x*ax.z*cosb + ax.y*sina);
	VecType ny = x*(ax.y*ax.x*cosb + ax.z*sina)
	+ y*(ax.y*ax.y*cosb + cosa)
	+ z*(ax.y*ax.z*cosb - ax.x*sina);
	VecType nz = x*(ax.z*ax.x*cosb - ax.y*sina)
	+ y*(ax.z*ax.y*cosb + ax.x*sina)
	+ z*(ax.z*ax.z*cosb + cosa);
	x = nx; y = ny; z = nz;
	return *this;
}

// const???
template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::rotated(VecType ax, VecType ay, VecType az) {
	return getRotated(ax,ay,az);
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::getRotated(VecType ax, VecType ay, VecType az) const {
	VecType a = (VecType)cos(DEG_TO_RAD*(ax));
	VecType b = (VecType)sin(DEG_TO_RAD*(ax));
	VecType c = (VecType)cos(DEG_TO_RAD*(ay));
	VecType d = (VecType)sin(DEG_TO_RAD*(ay));
	VecType e = (VecType)cos(DEG_TO_RAD*(az));
	VecType f = (VecType)sin(DEG_TO_RAD*(az));
	
	VecType nx = c * e * x - c * f * y + d * z;
	VecType ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	VecType nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
	
	return ofVec3_< VecType >( nx, ny, nz );
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::getRotatedRad(VecType ax, VecType ay, VecType az) const {
	VecType a = cos(ax);
	VecType b = sin(ax);
	VecType c = cos(ay);
	VecType d = sin(ay);
	VecType e = cos(az);
	VecType f = sin(az);
	
	VecType nx = c * e * x - c * f * y + d * z;
	VecType ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	VecType nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
	
	return ofVec3_< VecType >( nx, ny, nz );
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::rotate(VecType ax, VecType ay, VecType az) {
	VecType a = (VecType)cos(DEG_TO_RAD*(ax));
	VecType b = (VecType)sin(DEG_TO_RAD*(ax));
	VecType c = (VecType)cos(DEG_TO_RAD*(ay));
	VecType d = (VecType)sin(DEG_TO_RAD*(ay));
	VecType e = (VecType)cos(DEG_TO_RAD*(az));
	VecType f = (VecType)sin(DEG_TO_RAD*(az));
	
	VecType nx = c * e * x - c * f * y + d * z;
	VecType ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	VecType nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
	
	x = nx; y = ny; z = nz;
	return *this;
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::rotateRad(VecType ax, VecType ay, VecType az) {
	VecType a = cos(ax);
	VecType b = sin(ax);
	VecType c = cos(ay);
	VecType d = sin(ay);
	VecType e = cos(az);
	VecType f = sin(az);
	
	VecType nx = c * e * x - c * f * y + d * z;
	VecType ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	VecType nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
	
	x = nx; y = ny; z = nz;
	return *this;
}

//-----------------------------------------------
// Rotate point by angle (deg) around line 
// defined by pivot and axis.
//
template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::rotated( VecType angle,
								const ofVec3_< VecType >& pivot,
								const ofVec3_< VecType >& axis ) const{
	return getRotated(angle, pivot, axis);
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::getRotated( VecType angle,
								   const ofVec3_< VecType >& pivot,
								   const ofVec3_< VecType >& axis ) const {
	ofVec3_< VecType > ax = axis.normalized();
	VecType tx = x - pivot.x;
	VecType ty = y - pivot.y;
	VecType tz = z - pivot.z;
	
	VecType a = (VecType)(angle*DEG_TO_RAD);
	VecType sina = sin( a );
	VecType cosa = cos( a );
	VecType cosb = 1.0f - cosa;
	
	VecType xrot = tx*(ax.x*ax.x*cosb + cosa)
	+ ty*(ax.x*ax.y*cosb - ax.z*sina)
	+ tz*(ax.x*ax.z*cosb + ax.y*sina);
	VecType yrot = tx*(ax.y*ax.x*cosb + ax.z*sina)
	+ ty*(ax.y*ax.y*cosb + cosa)
	+ tz*(ax.y*ax.z*cosb - ax.x*sina);
	VecType zrot = tx*(ax.z*ax.x*cosb - ax.y*sina)
	+ ty*(ax.z*ax.y*cosb + ax.x*sina)
	+ tz*(ax.z*ax.z*cosb + cosa);
	
	
	return ofVec3_< VecType >( xrot+pivot.x, yrot+pivot.y, zrot+pivot.z );
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::getRotatedRad( VecType angle,
									  const ofVec3_< VecType >& pivot,
									  const ofVec3_< VecType >& axis ) const {
	ofVec3_< VecType > ax = axis.normalized();
	VecType tx = x - pivot.x;
	VecType ty = y - pivot.y;
	VecType tz = z - pivot.z;
	
	VecType a = angle;
	VecType sina = sin( a );
	VecType cosa = cos( a );
	VecType cosb = 1.0f - cosa;
	
	VecType xrot = tx*(ax.x*ax.x*cosb + cosa)
	+ ty*(ax.x*ax.y*cosb - ax.z*sina)
	+ tz*(ax.x*ax.z*cosb + ax.y*sina);
	VecType yrot = tx*(ax.y*ax.x*cosb + ax.z*sina)
	+ ty*(ax.y*ax.y*cosb + cosa)
	+ tz*(ax.y*ax.z*cosb - ax.x*sina);
	VecType zrot = tx*(ax.z*ax.x*cosb - ax.y*sina)
	+ ty*(ax.z*ax.y*cosb + ax.x*sina)
	+ tz*(ax.z*ax.z*cosb + cosa);
	
	return ofVec3_< VecType >( xrot+pivot.x, yrot+pivot.y, zrot+pivot.z );
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::rotate( VecType angle,
                                                       const ofVec3_< VecType >& pivot,
                                                       const ofVec3_< VecType >& axis ) {
	ofVec3_< VecType > ax = axis.normalized();
	x -= pivot.x;
	y -= pivot.y;
	z -= pivot.z;
	
	VecType a = (VecType)(angle*DEG_TO_RAD);
	VecType sina = sin( a );
	VecType cosa = cos( a );
	VecType cosb = 1.0f - cosa;
	
	VecType xrot = x * ( ax.x * ax.x * cosb + cosa )
                     + y * ( ax.x * ax.y * cosb - ax.z * sina )
                     + z * ( ax.x * ax.z * cosb + ax.y * sina );
	VecType yrot = x * ( ax.y * ax.x * cosb + ax.z * sina )
                     + y * ( ax.y * ax.y * cosb + cosa )
                     + z * ( ax.y * ax.z * cosb - ax.x * sina );
	VecType zrot = x * ( ax.z * ax.x * cosb - ax.y * sina )
                     + y * ( ax.z * ax.y * cosb + ax.x * sina )
                     + z * ( ax.z * ax.z * cosb + cosa );
	
	x = xrot + pivot.x;
	y = yrot + pivot.y;
	z = zrot + pivot.z;
	
	return *this;
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::rotateRad( VecType angle,
                                                          const ofVec3_< VecType >& pivot,
                                                          const ofVec3_< VecType >& axis ) {
	ofVec3_< VecType > ax = axis.normalized();
	x -= pivot.x;
	y -= pivot.y;
	z -= pivot.z;
	
	VecType a = angle;
	VecType sina = sin( a );
	VecType cosa = cos( a );
	VecType cosb = 1.0f - cosa;
	
	VecType xrot = x * ( ax.x * ax.x * cosb + cosa )
                     + y * ( ax.x * ax.y * cosb - ax.z * sina )
                     + z * ( ax.x * ax.z * cosb + ax.y * sina );
	VecType yrot = x * ( ax.y * ax.x * cosb + ax.z * sina )
                     + y * ( ax.y * ax.y * cosb + cosa )
                     + z * ( ax.y * ax.z * cosb - ax.x * sina );
	VecType zrot = x * ( ax.z * ax.x * cosb - ax.y * sina )
                     + y * ( ax.z * ax.y * cosb + ax.x * sina )
                     + z * ( ax.z * ax.z * cosb + cosa );
	
	x = xrot + pivot.x;
	y = yrot + pivot.y;
	z = zrot + pivot.z;
	
	return *this;
}

//-----------------------------------------------
// Map point to coordinate system defined 
// by origin, vx, vy, and vz.
//
template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::mapped( const ofVec3_< VecType >& origin,
                                                      const ofVec3_< VecType >& vx,
                                                      const ofVec3_< VecType >& vy,
                                                      const ofVec3_< VecType >& vz ) const {
	return getMapped(origin, vx, vy, vz);
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::getMapped( const ofVec3_< VecType >& origin,
                                                         const ofVec3_< VecType >& vx,
                                                         const ofVec3_< VecType >& vy,
                                                         const ofVec3_< VecType >& vz ) const {
	return ofVec3_< VecType >( origin.x + x*vx.x + y*vy.x + z*vz.x,
                               origin.y + x*vx.y + y*vy.y + z*vz.y,
                               origin.z + x*vx.z + y*vy.z + z*vz.z );
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::map( const ofVec3_< VecType >& origin,
                                                    const ofVec3_< VecType >& vx,
                                                    const ofVec3_< VecType >& vy,
                                                    const ofVec3_< VecType >& vz ) {
	VecType xmap = origin.x + x*vx.x + y*vy.x + z*vz.x;
	VecType ymap =  origin.y + x*vx.y + y*vy.y + z*vz.y;
	z = origin.z + x*vx.z + y*vy.z + z*vz.z;
	x = xmap;
	y = ymap;
	return *this;
}

//-----------------------------------------------
// Distance between two points.
//
template <typename VecType>
inline VecType ofVec3_< VecType >::distance( const ofVec3_< VecType >& pnt) const {
	VecType vx = x-pnt.x;
	VecType vy = y-pnt.y;
	VecType vz = z-pnt.z;
	return (VecType)sqrt(vx*vx + vy*vy + vz*vz);
}

template <typename VecType>
inline VecType  ofVec3_< VecType >::distanceSquared( const ofVec3_< VecType >& pnt ) const{
	return squareDistance(pnt);
}

template <typename VecType>
inline VecType  ofVec3_< VecType >::squareDistance( const ofVec3_< VecType >& pnt ) const {
	VecType vx = x-pnt.x;
	VecType vy = y-pnt.y;
	VecType vz = z-pnt.z;
	return vx*vx + vy*vy + vz*vz;
}

//-----------------------------------------------
// Linear interpolation.
//
// p==0.0 results in this point, p==0.5 results 
// in the midpoint, and p==1.0 results in pnt 
// being returned.
// 
template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::interpolated( const ofVec3_< VecType >& pnt, VecType p ) const {
	return getInterpolated(pnt,p);
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::getInterpolated( const ofVec3_< VecType >& pnt, VecType p ) const {
	return ofVec3_< VecType >( x*(1-p) + pnt.x*p,
				   y*(1-p) + pnt.y*p,
				   z*(1-p) + pnt.z*p );
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::interpolate( const ofVec3_< VecType >& pnt, VecType p ) {
	x = x*(1-p) + pnt.x*p;
	y = y*(1-p) + pnt.y*p;
	z = z*(1-p) + pnt.z*p;
	return *this;
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::middled( const ofVec3_< VecType >& pnt ) const {
	return getMiddle(pnt);
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::getMiddle( const ofVec3_< VecType >& pnt ) const {
	return ofVec3_< VecType >( (x+pnt.x)/2.0f, (y+pnt.y)/2.0f, (z+pnt.z)/2.0f );
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::middle( const ofVec3_< VecType >& pnt ) {
	x = (x+pnt.x)/2.0f;
	y = (y+pnt.y)/2.0f;
	z = (z+pnt.z)/2.0f;
	return *this;
}

//-----------------------------------------------
// Average (centroid) among points.
// Addition is sometimes useful for calculating 
// averages too.
//
template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::average( const ofVec3_< VecType >* points, int num ) {
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

//-----------------------------------------------
// Normalization
//
template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::normalized() const {
	return getNormalized();
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::getNormalized() const {
	VecType length = (VecType)sqrt(x*x + y*y + z*z);
	if( length > 0 ) {
		return ofVec3_< VecType >( x/length, y/length, z/length );
	} else {
		return ofVec3_< VecType >();
	}
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::normalize() {
	VecType length = (VecType)sqrt(x*x + y*y + z*z);
	if( length > 0 ) {
		x /= length;
		y /= length;
		z /= length;
	}
	return *this;
}

//-----------------------------------------------
// Limit length.
//
template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::limited(VecType max) const {
	return getLimited(max);
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::getLimited(VecType max) const {
    ofVec3_< VecType > limited;
    VecType lengthSquared = (x*x + y*y + z*z);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        VecType ratio = max/(VecType)sqrt(lengthSquared);
        limited.set( x*ratio, y*ratio, z*ratio);
    } else {
        limited.set(x,y,z);
    }
    return limited;
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::limit(VecType max) {
    VecType lengthSquared = (x*x + y*y + z*z);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        VecType ratio = max/(VecType)sqrt(lengthSquared);
        x *= ratio;
        y *= ratio;
        z *= ratio;
    }
    return *this;
}

//-----------------------------------------------
// Perpendicular vector.
//
template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::crossed( const ofVec3_< VecType >& vec ) const {
	return getCrossed(vec);
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::getCrossed( const ofVec3_< VecType >& vec ) const {
	return ofVec3_< VecType >( y*vec.z - z*vec.y,
				   z*vec.x - x*vec.z,
				   x*vec.y - y*vec.x );
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::cross( const ofVec3_< VecType >& vec ) {
	VecType _x = y*vec.z - z*vec.y;
	VecType _y = z*vec.x - x*vec.z;
	z = x*vec.y - y*vec.x;
	x = _x;
	y = _y;
	return *this;
}

//-----------------------------------------------
// Normalized perpendicular.
// 
template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::perpendiculared( const ofVec3_< VecType >& vec ) const {
	return getPerpendicular(vec);
}

template <typename VecType>
inline ofVec3_< VecType > ofVec3_< VecType >::getPerpendicular( const ofVec3_< VecType >& vec ) const {
	VecType crossX = y*vec.z - z*vec.y;
	VecType crossY = z*vec.x - x*vec.z;
	VecType crossZ = x*vec.y - y*vec.x;
	
	VecType length = (VecType)sqrt(crossX*crossX +
							   crossY*crossY +
							   crossZ*crossZ);
	
	if( length > 0 )
		return ofVec3_< VecType >( crossX/length, crossY/length, crossZ/length );
	else
		return ofVec3_< VecType >();
}

template <typename VecType>
inline ofVec3_< VecType >& ofVec3_< VecType >::perpendicular( const ofVec3_< VecType >& vec ) {
	VecType crossX = y*vec.z - z*vec.y;
	VecType crossY = z*vec.x - x*vec.z;
	VecType crossZ = x*vec.y - y*vec.x;
	
	VecType length = (VecType)sqrt(crossX*crossX +
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

//-----------------------------------------------
// Length
//
template <typename VecType>
inline VecType ofVec3_< VecType >::length() const {
	return (VecType)sqrt( x*x + y*y + z*z );
}

template <typename VecType>
inline VecType ofVec3_< VecType >::lengthSquared() const {
	return (VecType)(x*x + y*y + z*z);
}

template <typename VecType>
inline VecType ofVec3_< VecType >::squareLength() const {
	return lengthSquared();
}

//-----------------------------------------------
// Angle
// 
// Angle (deg) between two vectors.
// VecTypehis is an unsigned relative angle from 0 to 180.
//
// Ref:
// euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
// 
template <typename VecType>
inline VecType ofVec3_< VecType >::angle( const ofVec3_< VecType >& vec ) const {
	ofVec3_< VecType > n1 = this->normalized();
	ofVec3_< VecType > n2 = vec.normalized();
	return (VecType)(acos( n1.dot(n2) )*RAD_TO_DEG);
}

template <typename VecType>
inline VecType ofVec3_< VecType >::angleRad( const ofVec3_< VecType >& vec ) const {
	ofVec3_< VecType > n1 = this->normalized();
	ofVec3_< VecType > n2 = vec.normalized();
	return (VecType)acos( n1.dot(n2) );
}

//-----------------------------------------------
// Dot Product.
//
template <typename VecType>
inline VecType ofVec3_< VecType >::dot( const ofVec3_< VecType >& vec ) const {
	return x*vec.x + y*vec.y + z*vec.z;
}

//-----------------------------------------------
// Non-Member operators
//
template <typename VecType>
inline ofVec3_< VecType > operator+( VecType f, const ofVec3_< VecType >& vec ) {
    return ofVec3_< VecType >( f+vec.x, f+vec.y, f+vec.z );
}

template <typename VecType>
inline ofVec3_< VecType > operator-( VecType f, const ofVec3_< VecType >& vec ) {
    return ofVec3_< VecType >( f-vec.x, f-vec.y, f-vec.z );
}

template <typename VecType>
inline ofVec3_< VecType > operator*( VecType f, const ofVec3_< VecType >& vec ) {
    return ofVec3_< VecType >( f*vec.x, f*vec.y, f*vec.z );
}

template <typename VecType>
inline ofVec3_< VecType > operator/( VecType f, const ofVec3_< VecType >& vec ) {
    return ofVec3_< VecType >( f/vec.x, f/vec.y, f/vec.z);
}

//-----------------------------------------------
// Templates
//
typedef ofVec3_< float  > ofVec3f;
// typedef ofVec3_< double > ofVec3d;
