#include "testApp.h"

/*
	rotateToNormal will rotate everything using ofRotate. the rotation amount
	and axis are generated using an ofQuaternion. the trick is to use ofQuaternion
	to determine the rotation that is required from a standard axis (0,0,1) to the
	desired normal vector, then apply that rotation. 
*/
//--------------------------------------------------------------
void rotateToNormal(ofVec3f normal) {
	normal.normalize();
	
	float rotationAmount;
	ofVec3f rotationAngle;
	ofQuaternion rotation;
	
	ofVec3f axis(0, 0, 1);
	rotation.makeRotate(axis, normal);
	rotation.getRotate(rotationAmount, rotationAngle);
	ofRotate(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);
}

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	// draw the vertices in pathLines as a line strip
	pathLines.setMode(OF_PRIMITIVE_LINE_STRIP);
}

//--------------------------------------------------------------
void testApp::update(){
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
void testApp::draw(){
	ofColor cyan = ofColor::fromHex(0x00abec);
	ofColor magenta = ofColor::fromHex(0xec008c);
	ofColor yellow = ofColor::fromHex(0xffee00);
	
	ofBackgroundGradient(magenta * .6, magenta * .4);
	ofNoFill();
	
	easyCam.begin();
	ofRotateX(15);

	ofSetColor(0);
	ofDrawGrid(500, 10, false, false, true, false);
	
	// draw the path of the box
	ofSetLineWidth(2);
	ofSetColor(cyan);
	pathLines.draw();
	
	// draw a line connecting the box to the grid
	ofSetColor(yellow);
	ofLine(current.x, current.y, current.z, current.x, 0, current.z);
	
	// translate and rotate to the current position and orientation
	ofTranslate(current.x, current.y, current.z);
	rotateToNormal(current - previous);
	ofSetColor(255);
	ofDrawBox(32);
	ofDrawAxis(32);
	
	easyCam.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
