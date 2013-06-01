#include "ofVec2.h"
#include "ofVec3.h"
#include "ofVec4.h"

template <typename VecType> 
inline ofVec4_< VecType >::ofVec4_() {
	x = 0;
	y = 0;
	z = 0;
    w = 0;
}

template <typename VecType> 
inline ofVec4_< VecType >::ofVec4_( VecType _s ) {
	x = _s;
	y = _s;
	z = _s;
    w = _s;
}

template <typename VecType> 
inline ofVec4_< VecType >::ofVec4_( VecType _x,
                                    VecType _y,
                                    VecType _z,
                                    VecType _w ) {
	x = _x;
	y = _y;
	z = _z;
    w = _w;
}

template <typename VecType>
ofVec4_< VecType >::ofVec4_( const ofVec3_< VecType >& vec ) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = 0;
}

template <typename VecType>
ofVec4_< VecType >::ofVec4_( const ofVec2_< VecType >& vec ) {
	x = vec.x;
	y = vec.y;
	z = 0;
	w = 0;
}

template class ofVec4_< float >;
template class ofVec4_< double >;
