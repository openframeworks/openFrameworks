#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(50, 50, 50);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();

	shader.setGeometryInputType(GL_LINES);
	shader.setGeometryOutputType(GL_TRIANGLE_STRIP);
	shader.setGeometryOutputCount(4);
	shader.load("shaders/vert.glsl", "shaders/frag.glsl", "shaders/geom.glsl");

	ofLog() << "Maximum number of output vertices support is: " << shader.getGeometryMaxOutputCount();

	// create a bunch of random points
	float r = ofGetHeight()/2;
	for(int i=0; i<100; i++) {
		points.push_back(glm::vec3(ofRandomf() * r, ofRandomf() * r, ofRandomf() * r));
	}

	doShader = true;
	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushMatrix();

	if(doShader) {
		shader.begin();

		// set thickness of ribbons
		shader.setUniform1f("thickness", 20);

		// make light direction slowly rotate
		shader.setUniform3f("lightDir", sin(ofGetElapsedTimef()/10), cos(ofGetElapsedTimef()/10), 0);
	}

	ofColor(255);

	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);
	ofRotateXDeg(mouseY);
	ofRotateYDeg(mouseX);

	for(unsigned int i=1; i<points.size(); i++) {
		ofDrawLine(points[i-1], points[i]);
	}

	if(doShader) shader.end();

	ofPopMatrix();

	ofDrawBitmapString("fps: " + ofToString((int)ofGetFrameRate()) + "\nPress 's' to toggle shader: " + (doShader ? "ON" : "OFF"), 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	if( key == 's' ){
		doShader = !doShader;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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

