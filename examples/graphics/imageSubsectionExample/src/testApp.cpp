#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	img.loadImage("indispensable.jpg");
	ofSetLineWidth(2);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(ofColor::black);
	
	// draw the original image
	ofSetColor(ofColor::white);
	img.draw(0, 0);
	
	// draw the four rectangles
	ofNoFill();
	ofSetColor(ofColor::red);
	ofRect(mouseX, mouseY, 100, 100);
	
	ofSetColor(ofColor::green);
	ofRect(mouseX, mouseY, 50, 50);
	
	ofSetColor(ofColor::blue);
	ofRect(mouseX + 25, mouseY + 25, 50, 50);
	
	ofSetColor(ofColor::magenta);
	ofRect(mouseX - 25, mouseY - 25, 50, 50);
	
	// draw the four corresponding subsections
	ofTranslate(427, 0);
	ofSetColor(ofColor::white);
	img.drawSubsection(0, 0, 100, 100, mouseX, mouseY);
	ofSetColor(ofColor::red);
	ofRect(0, 0, 100, 100);
	
	ofSetColor(ofColor::white);
	img.drawSubsection(0, 100, 100, 100, mouseX, mouseY, 50, 50);
	ofSetColor(ofColor::green);
	ofRect(0, 100, 100, 100);
	
	ofSetColor(ofColor::white);
	img.drawSubsection(0, 200, 100, 100, mouseX + 25, mouseY + 25, 50, 50);
	ofSetColor(ofColor::blue);
	ofRect(0, 200, 100, 100);
	
	ofSetColor(ofColor::white);
	img.drawSubsection(0, 300, 100, 100, mouseX - 25, mouseY - 25, 50, 50);
	ofSetColor(ofColor::magenta);
	ofRect(0, 300, 100, 100);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

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