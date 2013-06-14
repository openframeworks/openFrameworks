#include "ofVec2.h"
#include "ofVec3.h"
#include "ofVec4.h"

template <typename VecType>
inline ofVec2_< VecType >::ofVec2_() {
}

template <typename VecType>
inline ofVec2_< VecType >::ofVec2_( VecType _s ) {
}

template <typename VecType>
inline ofVec2_< VecType >::ofVec2_( VecType _x, VecType _y ) {
	x = _x;
	y = _y;
}

template <typename VecType>
inline ofVec2_< VecType >::ofVec2_( const ofVec3_< VecType >& vec ) {
	x = vec.x;
	y = vec.y;
}

template <typename VecType>
inline ofVec2_< VecType >::ofVec2_( const ofVec4_< VecType >& vec ) {
	x = vec.x;
	y = vec.y;
}

template class ofVec2_< float  >;
template class ofVec2_< double >;
