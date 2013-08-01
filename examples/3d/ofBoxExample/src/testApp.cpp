#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);

	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	ofEnableDepthTest();
	
	// ofBox uses texture coordinates from 0-1, so you can load whatever
	// sized images you want and still use them to texture your box
	// but we have to explicitly normalize our tex coords here
	ofEnableNormalizedTexCoords();
	
	// loads the OF logo from disk
	ofLogo.loadImage("of.png");
	
	// draw the ofBox outlines with some weight
	ofSetLineWidth(10);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0, 0, 0);
	
	float movementSpeed = .1;
	float cloudSize = ofGetWidth() / 2;
	float maxBoxSize = 100;
	float spacing = 1;
	int boxCount = 100;
	
	cam.begin();
	
	for(int i = 0; i < boxCount; i++) {
		ofPushMatrix();
		
		float t = (ofGetElapsedTimef() + i * spacing) * movementSpeed;
		ofVec3f pos(
			ofSignedNoise(t, 0, 0),
			ofSignedNoise(0, t, 0),
			ofSignedNoise(0, 0, t));
		
		float boxSize = maxBoxSize * ofNoise(pos.x, pos.y, pos.z);
		
		pos *= cloudSize;
		ofTranslate(pos);
		ofRotateX(pos.x);
		ofRotateY(pos.y);
		ofRotateZ(pos.z);
		
		ofLogo.bind();
		ofFill();
		ofSetColor(255);
		ofDrawBox(boxSize);
		ofLogo.unbind();
		
		ofNoFill();
		ofSetColor(ofColor::fromHsb(sinf(t) * 128 + 128, 255, 255));
		ofDrawBox(boxSize * 1.1f);
		
		ofPopMatrix();
	}
	
	cam.end();
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
