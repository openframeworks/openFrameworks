/**
*  ofxPoint2f.h
*  by stefanix
*
* DEPRECATED!! use ofxVec2f.h for both points and vectors.
*/


#ifndef _OFX_POINT2f
#define _OFX_POINT2f

#if (_MSC_VER)
#pragma message("warning: ofxPoint2f is deprecated, use ofxVec2f.h for both points and vectors")
#else
#warning "ofxPoint2f is deprecated, use ofxVec2f.h for both points and vectors"
#endif


#include "ofxVec2f.h"

typedef ofxVec2f  ofxPoint2f;


#endif
