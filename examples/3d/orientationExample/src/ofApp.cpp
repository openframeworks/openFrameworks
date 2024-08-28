#include "ofApp.h"

/*
	rotateToNormal will rotate everything using a rotation matrix which is generated using a quaternion (glm::quat).
	for this we use the function glm::rotation(glm::vec3 v1, glm::vec3 v2) which takes in two normalized vectors and returns
	a quaternion that represents the rotation between these two vectors.
	We will use the normal which is passed to this function and the standard z axis.
 */
//--------------------------------------------------------------
void rotateToNormal(glm::vec3 normal) {

	// the normal must be normalized.
	normal = glm::normalize(normal);

	// the standard Z axis.
	glm::vec3 axis(0, 0, 1);

	// create the quaternion which represents the rotation between normal and axis
	glm::quat rotation = glm::rotation(axis, normal);

	// transform the quaternion to a 4x4 matrix using the glm function for such.
	glm::mat4 rotationMatrix = glm::toMat4(rotation);

	// multiply the rotation matrix with the current matrix.
	// This means that the rotation matrix, thus the transformation it represents, will be applied on over the current transformation matrix.
	ofMultMatrix( rotationMatrix );
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	// draw the vertices in pathLines as a line strip
	pathLines.setMode(OF_PRIMITIVE_LINE_STRIP);
}

//--------------------------------------------------------------
void ofApp::update(){
	previous = current;

	// generate a noisy 3d position over time
	float t = (2 + ofGetElapsedTimef()) * .1;
	current.x = ofSignedNoise(t, 0, 0);
	current.y = ofSignedNoise(0, t, 0);
	current.z = ofSignedNoise(0, 0, t);
	current *= 400; // scale from -1,+1 range to -400,+400

	// add the current position to the pathVertices deque
	pathVertices.push_back(current);
	// if we have too many vertices in the deque, get rid of the oldest ones
	while(pathVertices.size() > 200) {
		pathVertices.pop_front();
	}

	// clear the pathLines ofMesh from any old vertices
	pathLines.clear();
	// add all the vertices from pathVertices
	for(unsigned int i = 0; i < pathVertices.size(); i++) {
		pathLines.addVertex(pathVertices[i]);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofColor cyan = ofColor::fromHex(0x00abec);
	ofColor magenta = ofColor::fromHex(0xec008c);
	ofColor yellow = ofColor::fromHex(0xffee00);

	ofBackgroundGradient(magenta * .6, magenta * .4);
	ofNoFill();

	easyCam.begin();
	ofRotateXDeg(15);

	ofSetColor(0);
	ofDrawGrid(500, 10, false, false, true, false);

	// draw the path of the box
	ofSetLineWidth(2);
	ofSetColor(cyan);
	pathLines.draw();

	// draw a line connecting the box to the grid
	ofSetColor(yellow);
	ofDrawLine(current.x, current.y, current.z, current.x, 0, current.z);

	ofTranslate(current.x, current.y, current.z);
	if( glm::length(current - previous ) > 0.0 ){
		// translate and rotate every 3D object after this line to the current position and orientation of our line, but only if the line is longer than 0 or has a length
		rotateToNormal(current - previous);
	}
	ofSetColor(255);
	ofDrawBox(32);
	ofDrawAxis(32);

	easyCam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
