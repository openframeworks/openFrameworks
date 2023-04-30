#include "ofVec2f.h"
#include "ofVec3f.h"
#include "ofVec4f.h"

#define GLM_FORCE_CTOR_INIT
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

ofVec4f::ofVec4f(const ofVec3f& vec) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = 0;
}

ofVec4f::ofVec4f(const ofVec2f& vec) {
	x = vec.x;
	y = vec.y;
	z = 0;
	w = 0;
}

ofVec2f::ofVec2f(const glm::vec3 & v): x(v.x), y(v.y) {}

ofVec2f::ofVec2f(const glm::vec4 & v): x(v.x), y(v.y) {}

ofVec4f::ofVec4f( const glm::vec2& v):x(v.x), y(v.y), z(0.0), w(0.0) {}
ofVec4f::ofVec4f( const glm::vec3& v):x(v.x), y(v.y), z(v.z), w(0.0) {}
ofVec4f::ofVec4f( const glm::vec4& v):x(v.x), y(v.y), z(v.z), w(v.w) {}
