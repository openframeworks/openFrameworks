/**
*  ofxPoint3f.h
*  by stefanix
*
* DEPRECATED!! use ofxVec3f.h for both points and vectors.
*/


#ifndef _OFX_POINT3f
#define _OFX_POINT3f

#if (_MSC_VER)
#pragma message("warning: ofxPoint3f is deprecated, use ofxVec3f.h for both points and vectors")
#else
#warning "ofxPoint3f is deprecated, use ofxVec3f.h for both points and vectors"
#endif

#include "ofxVec3f.h"


typedef ofxVec3f  ofxPoint3f;



#endif
