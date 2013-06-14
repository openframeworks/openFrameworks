#pragma once

template <typename VecType>
class ofVec2_;

template <typename VecType>
class ofVec3_;
#include "ofConstants.h"

// class ofVec4_ : public ofVec_<VecType, 4>
template <typename VecType>
class ofVec4_ {
public:
	VecType x, y, z, w;

    static const int DIM = 4;

	ofVec4_();
	explicit ofVec4_( VecType _scalar );
	ofVec4_( VecType _x, VecType _y, VecType _z, VecType _w );
	ofVec4_( const ofVec2_< VecType >& vec);
	ofVec4_( const ofVec3_< VecType >& vec);
		
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
    void set( VecType _x, VecType _y, VecType _z, VecType _w );
    void set( const ofVec4_< VecType >& vec );
	
    //-----------------------------------------------
    // Check similarity/equality.
    //
    bool operator == ( const ofVec4_< VecType >& vec ) const;
    bool operator != ( const ofVec4_< VecType >& vec ) const;
    bool match( const ofVec4_< VecType >& vec, VecType tolerance=0.0001 ) const;
	
    //-----------------------------------------------
    // Operators + & -
    //
    ofVec4_< VecType >  operator +  ( const ofVec4_< VecType >& vec ) const;
    ofVec4_< VecType >& operator += ( const ofVec4_< VecType >& vec );
    ofVec4_< VecType >  operator -  ( const ofVec4_< VecType >& vec ) const;
    ofVec4_< VecType >& operator -= ( const ofVec4_< VecType >& vec );
    ofVec4_< VecType >  operator +  ( const VecType f ) const;
    ofVec4_< VecType >& operator += ( const VecType f );
    ofVec4_< VecType >  operator -  ( const VecType f ) const;
    ofVec4_< VecType >& operator -= ( const VecType f );
    ofVec4_< VecType >  operator -  () const;
		
	ofVec4_< VecType >& operator =  ( const ofVec4_< VecType >& vec ) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
		return *this;
	}
	
    //-----------------------------------------------
    // Scalings
    //
    ofVec4_< VecType >  operator *  ( const ofVec4_< VecType >& vec ) const;
    ofVec4_< VecType >& operator *= ( const ofVec4_< VecType >& vec );
    ofVec4_< VecType >  operator *  ( const VecType f ) const;
    ofVec4_< VecType >& operator *= ( const VecType f );
    ofVec4_< VecType >  operator /  ( const ofVec4_< VecType >& vec ) const;
    ofVec4_< VecType >& operator /= ( const ofVec4_< VecType >& vec );
    ofVec4_< VecType >  operator /  ( const VecType f ) const;
    ofVec4_< VecType >& operator /= ( const VecType f );
	
    template <typename S>
	friend ostream& operator << ( ostream& os, const ofVec4_< S >& vec );
    template <typename S>
	friend istream& operator >> ( istream& is, const ofVec4_< S >& vec );
	
    //-----------------------------------------------
    // Scale
    //
    ofVec4_< VecType >  getScaled( const VecType length ) const;
    ofVec4_< VecType >& scale( const VecType length );
	
    //-----------------------------------------------
    // Distance between two points.
    //
    VecType distance( const ofVec4_< VecType >& pnt) const;
    VecType squareDistance( const ofVec4_< VecType >& pnt ) const;
	
    //-----------------------------------------------
    // Linear interpolation.
    // 
	// p==0.0 results in this point, p==0.5 results in the
	// midpoint, and p==1.0 results in pnt being returned.
	// 
    ofVec4_< VecType >   getInterpolated( const ofVec4_< VecType >& pnt, VecType p ) const;
    ofVec4_< VecType >&      interpolate( const ofVec4_< VecType >& pnt, VecType p );

    ofVec4_< VecType >   getMiddle( const ofVec4_< VecType >& pnt ) const;
    ofVec4_< VecType >&     middle( const ofVec4_< VecType >& pnt );

    ofVec4_< VecType >&  average( const ofVec4_< VecType >* points, int num );
    
    //-----------------------------------------------
    // Normalization
    //
    ofVec4_<VecType>  getNormalized() const;
    ofVec4_<VecType>& normalize();
	
    //-----------------------------------------------
    // Limit length.
    //
	ofVec4_<VecType>  getLimited(VecType max) const;
    ofVec4_<VecType>& limit(VecType max);
	
    //-----------------------------------------------
    // Length
    //
    VecType length() const;
    VecType lengthSquared() const;
	OF_DEPRECATED_MSG("Use ofVec4_<VecType>::lengthSquared() instead.", VecType squareLength() const);

    //-----------------------------------------------
    // Dot Product
    //
    VecType dot( const ofVec4_< VecType >& vec ) const;
	
    //-----------------------------------------------
    // DEPRECATED 006, please use:
    // getScaled
    ofVec4_< VecType > rescaled( const VecType length ) const;
    // scale
    ofVec4_< VecType >& rescale( const VecType length );
    // getNormalized
    ofVec4_< VecType > normalized() const;
    // getLimited
    ofVec4_< VecType > limited(VecType max) const;
    // use squareDistance
    VecType  distanceSquared( const ofVec4_< VecType >& pnt ) const;
    // use getInterpolated
    ofVec4_< VecType > 	interpolated( const ofVec4_< VecType >& pnt, VecType p ) const;
    // use getMiddle
    ofVec4_< VecType > 	middled( const ofVec4_< VecType >& pnt ) const;    

    // return all zero vector
    static ofVec4_< VecType > zero() { return ofVec4_< VecType >( 0, 0, 0, 0 ); }
    // return all one vector
    static ofVec4_< VecType > one() { return ofVec4_< VecType >( 1, 1, 1, 1 ); }
};

//-----------------------------------------------
// Non-Member operators
//
template <typename VecType> 
ofVec4_< VecType > operator+( VecType f, const ofVec4_< VecType >& vec );
template <typename VecType> 
ofVec4_< VecType > operator-( VecType f, const ofVec4_< VecType >& vec );
template <typename VecType> 
ofVec4_< VecType > operator*( VecType f, const ofVec4_< VecType >& vec );
template <typename VecType> 
ofVec4_< VecType > operator/( VecType f, const ofVec4_< VecType >& vec );

//-----------------------------------------------
// Getters and Setters.
//
template <typename VecType> 
inline void ofVec4_< VecType >::set( VecType _s ) {
	x = _s;
	y = _s;
	z = _s;
	w = _s;
}

template <typename VecType> 
inline void ofVec4_< VecType >::set( VecType _x, VecType _y, VecType _z, VecType _w ) {
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

template <typename VecType> 
inline void ofVec4_< VecType >::set( const ofVec4_< VecType >& vec ) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}

//-----------------------------------------------
// Check similarity/equality.
//
template <typename VecType> 
inline bool ofVec4_< VecType >::operator==( const ofVec4_< VecType >& vec ) const {
	return (x == vec.x) && (y == vec.y) && (z == vec.z) && (w == vec.w);
}

template <typename VecType> 
inline bool ofVec4_< VecType >::operator!=( const ofVec4_< VecType >& vec ) const {
	return (x != vec.x) || (y != vec.y) || (z != vec.z) || (w != vec.w);
}

template <typename VecType> 
inline bool ofVec4_< VecType >::match( const ofVec4_< VecType >& vec, VecType tolerance) const {
	return (fabs(x - vec.x) < tolerance)
	&& (fabs(y - vec.y) < tolerance)
	&& (fabs(z - vec.z) < tolerance)
	&& (fabs(w - vec.w) < tolerance);
}

//-----------------------------------------------
// Operators + & -
//
template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::operator+( const ofVec4_< VecType >& vec ) const {
	return ofVec4_< VecType >( x+vec.x, y+vec.y, z+vec.z, w+vec.w);
}

template <typename VecType> 
inline ofVec4_< VecType >& ofVec4_< VecType >::operator+=( const ofVec4_< VecType >& vec ) {
	x += vec.x;
	y += vec.y;
	z += vec.z;
	w += vec.w;
	return *this;
}

template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::operator-( const VecType f ) const {
	return ofVec4_< VecType >( x-f, y-f, z-f, w-f );
}

template <typename VecType> 
inline ofVec4_< VecType >& ofVec4_< VecType >::operator-=( const VecType f ) {
	x -= f;
	y -= f;
	z -= f;
	w -= f;
	return *this;
}

template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::operator-( const ofVec4_< VecType >& vec ) const {
	return ofVec4_< VecType >( x-vec.x, y-vec.y, z-vec.z, w-vec.w );
}

template <typename VecType> 
inline ofVec4_< VecType >& ofVec4_< VecType >::operator-=( const ofVec4_< VecType >& vec ) {
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	w -= vec.w;
	return *this;
}

template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::operator+( const VecType f ) const {
	return ofVec4_< VecType >( x+f, y+f, z+f, w+f );
}

template <typename VecType> 
inline ofVec4_< VecType >& ofVec4_< VecType >::operator+=( const VecType f ) {
	x += f;
	y += f;
	z += f;
	w += f;
	return *this;
}

template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::operator-() const {
	return ofVec4_< VecType >( -x, -y, -z, -w );
}

//-----------------------------------------------
// Scale
//
template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::operator*( const ofVec4_< VecType >& vec ) const {
	return ofVec4_< VecType >( x*vec.x, y*vec.y, z*vec.z, w*vec.w );
}

template <typename VecType> 
inline ofVec4_< VecType >& ofVec4_< VecType >::operator*=( const ofVec4_< VecType >& vec ) {
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
	w *= vec.w;
	return *this;
}

template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::operator*( const VecType f ) const {
	return ofVec4_< VecType >( x*f, y*f, z*f, w*f );
}

template <typename VecType> 
inline ofVec4_< VecType >& ofVec4_< VecType >::operator*=( const VecType f ) {
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::operator/( const ofVec4_< VecType >& vec ) const {
	return ofVec4_< VecType >( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y, vec.z!=0 ? z/vec.z : z, vec.w!=0 ? w/vec.w : w  );
}

template <typename VecType> 
inline ofVec4_< VecType >& ofVec4_< VecType >::operator/=( const ofVec4_< VecType >& vec ) {
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	vec.z!=0 ? z/=vec.z : z;
	vec.w!=0 ? w/=vec.w : w;
	return *this;
}

template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::operator/( const VecType f ) const {
	if(f == 0) return ofVec4_< VecType >(x, y, z, w);
	
	return ofVec4_< VecType >( x/f, y/f, z/f, w/f );
}

template <typename VecType> 
inline ofVec4_< VecType >& ofVec4_< VecType >::operator/=( const VecType f ) {
	if(f == 0)return *this;
	
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this;
}


template <typename VecType> 
inline ostream& operator<<(ostream& os, const ofVec4_< VecType >& vec) {
	os << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
	return os;
}

template <typename VecType> 
inline istream& operator>>( istream& is, ofVec4_< VecType >& vec ) {
	is >> vec.x;
	is.ignore(2);
	is >> vec.y;
	is.ignore(2);
	is >> vec.z;
	is.ignore(2);
	is >> vec.w;
	return is;
}

//-----------------------------------------------
// Scale
//
template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::rescaled( const VecType length ) const {
	return getScaled(length);
}

template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::getScaled( const VecType length ) const {
	VecType l = (VecType)sqrt(x*x + y*y + z*z + w*w);
	if( l > 0 )
		return ofVec4_< VecType >( (x/l)*length, (y/l)*length,
					   (z/l)*length, (w/l)*length );
	else
		return ofVec4_< VecType >();
}

template <typename VecType> 
inline ofVec4_< VecType >& ofVec4_< VecType >::rescale( const VecType length ) {
	return scale(length);
}

template <typename VecType> 
inline ofVec4_< VecType >& ofVec4_< VecType >::scale( const VecType length ) {
	VecType l = (VecType)sqrt(x*x + y*y + z*z + w*w);
	if (l > 0) {
		x = (x/l)*length;
		y = (y/l)*length;
		z = (z/l)*length;
		w = (w/l)*length;
	}
	return *this;
}

//-----------------------------------------------
// Distance between two points.
//
template <typename VecType> 
inline VecType ofVec4_< VecType >::distance( const ofVec4_< VecType >& pnt) const {
	VecType vx = x-pnt.x;
	VecType vy = y-pnt.y;
	VecType vz = z-pnt.z;
	VecType vw = w-pnt.w;
	return (VecType)sqrt( vx*vx + vy*vy + vz*vz + vw*vw );
}

template <typename VecType> 
inline VecType ofVec4_< VecType >::distanceSquared( const ofVec4_< VecType >& pnt ) const {
	return squareDistance(pnt);
}

template <typename VecType> 
inline VecType ofVec4_< VecType >::squareDistance( const ofVec4_< VecType >& pnt ) const {
	VecType vx = x-pnt.x;
	VecType vy = y-pnt.y;
	VecType vz = z-pnt.z;
	VecType vw = w-pnt.w;
	return vx*vx + vy*vy + vz*vz + vw*vw;
}

//-----------------------------------------------
// Linear interpolation.
//
// p==0.0 results in this point, p==0.5 results 
// in the midpoint, and p==1.0 results in pnt 
// being returned.
// 
template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::interpolated( const ofVec4_< VecType >& pnt, VecType p ) const{
	return getInterpolated(pnt,p);
}

template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::getInterpolated( const ofVec4_< VecType >& pnt, VecType p ) const {
	return ofVec4_< VecType >( x*(1-p) + pnt.x*p,
				   y*(1-p) + pnt.y*p,
				   z*(1-p) + pnt.z*p,
				   w*(1-p) + pnt.w*p );
}

template <typename VecType> 
inline ofVec4_< VecType >& ofVec4_< VecType >::interpolate( const ofVec4_< VecType >& pnt, VecType p ) {
	x = x*(1-p) + pnt.x*p;
	y = y*(1-p) + pnt.y*p;
	z = z*(1-p) + pnt.z*p;
	w = w*(1-p) + pnt.w*p;
	return *this;
}

template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::middled( const ofVec4_< VecType >& pnt ) const {
	return getMiddle(pnt);
}

template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::getMiddle( const ofVec4_< VecType >& pnt ) const {
	return ofVec4_< VecType >( (x+pnt.x)/2.0f, (y+pnt.y)/2.0f,
				   (z+pnt.z)/2.0f, (w+pnt.w)/2.0f );
}

template <typename VecType> 
inline ofVec4_< VecType >& ofVec4_< VecType >::middle( const ofVec4_< VecType >& pnt ) {
	x = (x+pnt.x)/2.0f;
	y = (y+pnt.y)/2.0f;
	z = (z+pnt.z)/2.0f;
	w = (w+pnt.w)/2.0f;
	return *this;
}

//-----------------------------------------------
// Average (centroid) among points.
// Addition is sometimes useful for calculating 
// averages too.
//
template <typename VecType> 
inline ofVec4_< VecType >& ofVec4_< VecType >::average( const ofVec4_< VecType >* points, int num ) {
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

//-----------------------------------------------
// Normalization
//
template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::normalized() const {
	return getNormalized();
}

template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::getNormalized() const {
	VecType length = (VecType)sqrt(x*x + y*y + z*z + w*w);
	if( length > 0 ) {
		return ofVec4_< VecType >( x/length, y/length, z/length, w/length );
	} else {
		return ofVec4_< VecType >();
	}
}

template <typename VecType> 
inline ofVec4_< VecType >& ofVec4_< VecType >::normalize() {
	VecType lenght = (VecType)sqrt(x*x + y*y + z*z + w*w);
	if( lenght > 0 ) {
		x /= lenght;
		y /= lenght;
		z /= lenght;
		w /= lenght;
	}
	return *this;
}

//-----------------------------------------------
// Limit length.
//
template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::limited(VecType max) const {
	return getLimited(max);
}

template <typename VecType> 
inline ofVec4_< VecType > ofVec4_< VecType >::getLimited(VecType max) const {
    ofVec4_< VecType > limited;
    VecType lengthSquared = (x*x + y*y + z*z + w*w);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        VecType ratio = max/(VecType)sqrt(lengthSquared);
        limited.set( x*ratio, y*ratio, z*ratio, w*ratio );
    } else {
        limited.set(x,y,z,w);
    }
    return limited;
}

template <typename VecType> 
inline ofVec4_< VecType >& ofVec4_< VecType >::limit(VecType max) {
    VecType lengthSquared = (x*x + y*y + z*z + w*w);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        VecType ratio = max/(VecType)sqrt(lengthSquared);
        x *= ratio;
        y *= ratio;
        z *= ratio;
        w *= ratio;
    }
    return *this;
}

//-----------------------------------------------
// Length
//
template <typename VecType> 
inline VecType ofVec4_< VecType >::length() const {
	return (VecType)sqrt( x*x + y*y + z*z + w*w );
}

template <typename VecType> 
inline VecType ofVec4_< VecType >::lengthSquared() const {
	return (VecType)(x*x + y*y + z*z + w*w);
}

template <typename VecType> 
inline VecType ofVec4_< VecType >::squareLength() const {
	return lengthSquared();
}

//-----------------------------------------------
// Dot Product.
//
template <typename VecType> 
inline VecType ofVec4_< VecType >::dot( const ofVec4_< VecType >& vec ) const {
	return x*vec.x + y*vec.y + z*vec.z + w*vec.w;
}

//-----------------------------------------------
// Non-Member operators
//
template <typename VecType> 
inline ofVec4_< VecType > operator+( VecType f, const ofVec4_< VecType >& vec ) {
    return ofVec4_< VecType >( f+vec.x, f+vec.y, f+vec.z, f+vec.w );
}

template <typename VecType> 
inline ofVec4_< VecType > operator-( VecType f, const ofVec4_< VecType >& vec ) {
    return ofVec4_< VecType >( f-vec.x, f-vec.y, f-vec.z, f-vec.w );
}

template <typename VecType> 
inline ofVec4_< VecType > operator*( VecType f, const ofVec4_< VecType >& vec ) {
    return ofVec4_< VecType >( f*vec.x, f*vec.y, f*vec.z, f*vec.w );
}

template <typename VecType> 
inline ofVec4_< VecType > operator/( VecType f, const ofVec4_< VecType >& vec ) {
    return ofVec4_< VecType >( f/vec.x, f/vec.y, f/vec.z, f/vec.w);
}

//-----------------------------------------------
// Templates
//
typedef ofVec4_< float  > ofVec4f;
// typedef ofVec4_< double  > ofVec4f;
