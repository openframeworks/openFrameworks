#include "ofVec2f.h"
#include "ofVec3f.h"
#include "ofVec4f.h"

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
