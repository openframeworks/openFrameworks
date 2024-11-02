#pragma once

// MARK: if OF_USE_LEGACY_VECTOR_MATH
#include "ofConstants.h"

#if OF_USE_LEGACY_VECTOR_MATH
class ofVec2f;
class ofVec3f;
class ofVec4f;
using ofDefaultVec2 = ofVec2f;
using ofDefaultVec3 = ofVec3f;
using ofDefaultVec4 = ofVec4f;
#else
#include <glm/detail/qualifier.hpp>
namespace glm {
	typedef vec<2, float, defaultp>		vec2;
	typedef vec<3, float, defaultp>		vec3;
	typedef vec<4, float, defaultp>		vec4;
}
using ofDefaultVec2 = glm::vec2;
using ofDefaultVec3 = glm::vec3;
using ofDefaultVec4 = glm::vec4;
#endif

#define OF_USE_LEGACY_MATH_MACROS

#ifdef OF_USE_LEGACY_MATH_MACROS
	#include <glm/gtc/constants.hpp>

	#ifndef PI
	#define PI glm::pi<float>()
	#endif

	#ifndef TWO_PI
	#define TWO_PI glm::two_pi<float>()
	#endif

	#ifndef M_TWO_PI
	#define M_TWO_PI glm::two_pi<float>()
	#endif

	#ifndef FOUR_PI
	#define FOUR_PI 12.56637061435917295385
	#endif

	#ifndef HALF_PI
	#define HALF_PI glm::half_pi<float>()
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

#endif
