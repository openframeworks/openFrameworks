#pragma once

#include "ofConstants.h"
#include "glm/fwd.hpp"

#if OF_USE_LEGACY_VECTOR_MATH
class ofVec2f;
class ofVec3f;
class ofVec4f;
using ofDefaultVec2 = ofVec2f;
using ofDefaultVec3 = ofVec3f;
using ofDefaultVec4 = ofVec4f;
#else
using ofDefaultVec2 = glm::vec2;
using ofDefaultVec3 = glm::vec3;
using ofDefaultVec4 = glm::vec4;
#endif


#ifndef PI
	#define PI       3.14159265358979323846
#endif

#ifndef TWO_PI
	#define TWO_PI   6.28318530717958647693
#endif

#ifndef M_TWO_PI
	#define M_TWO_PI   6.28318530717958647693
#endif

#ifndef FOUR_PI
	#define FOUR_PI 12.56637061435917295385
#endif

#ifndef HALF_PI
	#define HALF_PI  1.57079632679489661923
#endif

#ifndef DEG_TO_RAD
	#define DEG_TO_RAD (PI/180.0)
#endif

#ifndef RAD_TO_DEG
	#define RAD_TO_DEG (180.0/PI)
#endif

#ifndef MIN
	#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
	#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef CLAMP
	#define CLAMP(val,min,max) ((val) < (min) ? (min) : ((val > max) ? (max) : (val)))
#endif

#ifndef ABS
	#define ABS(x) (((x) < 0) ? -(x) : (x))
#endif

