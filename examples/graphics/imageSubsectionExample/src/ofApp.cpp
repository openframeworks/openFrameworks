#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	img.load("indispensable.jpg");
	ofSetLineWidth(2);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(ofColor::black);
	
	// draw the original image
	ofSetColor(ofColor::white);
	img.draw(0, 0);
	
	// draw the four rectangles
	ofNoFill();
	ofSetColor(ofColor::red);
	ofDrawRectangle(mouseX, mouseY, 100, 100);
	
	ofSetColor(ofColor::green);
	ofDrawRectangle(mouseX, mouseY, 50, 50);
	
	ofSetColor(ofColor::blue);
	ofDrawRectangle(mouseX + 25, mouseY + 25, 50, 50);
	
	ofSetColor(ofColor::magenta);
	ofDrawRectangle(mouseX - 25, mouseY - 25, 50, 50);
	
	// draw the four corresponding subsections
	ofTranslate(427, 0);
	ofSetColor(ofColor::white);
	img.drawSubsection(0, 0, 100, 100, mouseX, mouseY);
	ofSetColor(ofColor::red);
	ofDrawRectangle(0, 0, 100, 100);
	
	ofSetColor(ofColor::white);
	img.drawSubsection(0, 100, 100, 100, mouseX, mouseY, 50, 50);
	ofSetColor(ofColor::green);
	ofDrawRectangle(0, 100, 100, 100);
	
	ofSetColor(ofColor::white);
	img.drawSubsection(0, 200, 100, 100, mouseX + 25, mouseY + 25, 50, 50);
	ofSetColor(ofColor::blue);
	ofDrawRectangle(0, 200, 100, 100);
	
	ofSetColor(ofColor::white);
	img.drawSubsection(0, 300, 100, 100, mouseX - 25, mouseY - 25, 50, 50);
	ofSetColor(ofColor::magenta);
	ofDrawRectangle(0, 300, 100, 100);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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