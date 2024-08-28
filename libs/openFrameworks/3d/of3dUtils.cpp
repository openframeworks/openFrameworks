#include "of3dUtils.h"
#include "ofAppRunner.h"
#include "ofGraphicsBaseTypes.h"

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

