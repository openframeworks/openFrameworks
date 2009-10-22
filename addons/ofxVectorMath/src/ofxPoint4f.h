/**
*  ofxPoint4f.h
*  by stefanix
*
*  DEPRECATED!! use ofxVec4f.h for both points and vectors.
*/


#ifndef _OFX_POINT4f
#define _OFX_POINT4f

#if (_MSC_VER)
#pragma message("warning: ofxPoint4f is deprecated, use ofxVec4f.h for both points and vectors")
#else
#warning "ofxPoint4f is deprecated, use ofxVec4f.h for both points and vectors"
#endif

#include "ofxVec4f.h"


typedef ofxVec4f  ofxPoint4f;




#endif
