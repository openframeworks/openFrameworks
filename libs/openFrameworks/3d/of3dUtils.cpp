#include "of3dUtils.h"
#include "ofAppRunner.h"
#include "ofGraphicsBaseTypes.h"

#if !defined(GLM_FORCE_CTOR_INIT)
#define GLM_FORCE_CTOR_INIT
#endif
#if !defined(GLM_ENABLE_EXPERIMENTAL)
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/vec3.hpp>

//--------------------------------------------------------------
void ofDrawAxis(float size) {
	ofGetCurrentRenderer()->drawAxis(size);
}

//--------------------------------------------------------------
void ofDrawGrid(float stepSize, size_t numberOfSteps, bool labels, bool x, bool y, bool z) {
	ofGetCurrentRenderer()->drawGrid(stepSize,numberOfSteps,labels,x,y,z);
}

//--------------------------------------------------------------
void ofDrawGridPlane(float stepSize, size_t numberOfSteps, bool labels) {
	ofGetCurrentRenderer()->drawGridPlane(stepSize,numberOfSteps,labels);
}

//--------------------------------------------------------------
void ofDrawArrow(const glm::vec3& start, const glm::vec3& end, float headSize) {
	ofGetCurrentRenderer()->drawArrow(start,end,headSize);
}

//--------------------------------------------------------------
void ofDrawRotationAxes(float radius, float stripWidth, int circleRes){
	ofGetCurrentRenderer()->drawRotationAxes(radius,stripWidth,circleRes);
	
}

