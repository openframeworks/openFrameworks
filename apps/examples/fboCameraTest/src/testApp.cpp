#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	fbo.setup(800, 600, GL_RGB);
}

//--------------------------------------------------------------
void testApp::update(){
	fbo.begin();
	ofClear(1, 1, 0,1);
	ofSetColor(255, 0, 0);
	
	cam.setTarget(ofVec3f(800/2, 400/2, 0));
				  
	cam.begin();
	ofBeginShape();
	ofVertex(400, 200);
	ofVertex(600, 400);
	ofVertex(200, 400);
	ofEndShape(true);
	cam.end();
	
	fbo.end();
}

//--------------------------------------------------------------
void testApp::draw(){
	fbo.draw(0,0);
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