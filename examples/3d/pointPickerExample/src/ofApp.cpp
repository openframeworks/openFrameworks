#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	mesh.load("lofi-bunny.ply");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor(64), ofColor(0));
	
	ofSetColor(255);
	cam.begin();
	
	ofSetColor(ofColor::gray);
	mesh.drawWireframe();
	
	#ifndef TARGET_EMSCRIPTEN
		glPointSize(2);
	#endif
	ofSetColor(ofColor::white);
	mesh.drawVertices();
	cam.end();
	
	int n = mesh.getNumVertices();
	float nearestDistance = 0;
	glm::vec2 nearestVertex;
	int nearestIndex = 0;
	glm::vec3 mouse(mouseX, mouseY,0);
	for(int i = 0; i < n; i++) {
		glm::vec3 cur = cam.worldToScreen(mesh.getVertex(i));
		float distance = glm::distance(cur, mouse);
		if(i == 0 || distance < nearestDistance) {
			nearestDistance = distance;
			nearestVertex = cur;
			nearestIndex = i;
		}
	}
	
	ofSetColor(ofColor::gray);
	ofDrawLine(nearestVertex, mouse);
	
	ofNoFill();
	ofSetColor(ofColor::yellow);
	ofSetLineWidth(2);
	ofDrawCircle(nearestVertex, 4);
	ofSetLineWidth(1);
	
	glm::vec2 offset(10, -10);
	ofDrawBitmapStringHighlight(ofToString(nearestIndex), mouse + offset);
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
