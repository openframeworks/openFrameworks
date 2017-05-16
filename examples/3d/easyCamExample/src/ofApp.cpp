#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);

	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	ofEnableDepthTest();

	// this sets the camera's distance from the object
	cam.setDistance(100);

	ofSetCircleResolution(64);
	bHelpText = true;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	cam.begin();
	ofRotateXRad(.5);
	ofRotateYRad(-.5);

	ofBackground(0);

	ofSetColor(255,0,0);
	ofFill();
	ofDrawBox(30);
	ofNoFill();
	ofSetColor(0);
	ofDrawBox(30);

	ofPushMatrix();
	ofTranslate(0,0,20);
	ofSetColor(0,0,255);
	ofFill();
	ofDrawBox(5);
	ofNoFill();
	ofSetColor(0);
	ofDrawBox(5);
	ofPopMatrix();
	cam.end();
	drawInteractionArea();
	ofSetColor(255);

    if (bHelpText) {
        stringstream ss;
        ss << "FPS: " << ofToString(ofGetFrameRate(),0) <<endl<<endl;
        ss << "(c): Toggle mouse input"<<endl<<endl;
        ss <<"(LEFT MOUSE BUTTON DRAG inside yellow circle): camera x,y rotation"<<endl;
        ss <<"(LEFT MOUSE BUTTON DRAG outside yellow circle): camera z rotation or roll"<<endl<<endl;
        ss <<"(LEFT MOUSE BUTTON DRAG + alt key): move over x,y axis / truck and boom"<<endl;
        ss <<"(LEFT MOUSE BUTTON DRAG + m): move over x,y axis / truck and boom"<<endl;
        ss <<"(LEFT MOUSE BUTTON DRAG + MIDDLE MOUSE BUTTON PRESS): move over x,y axis / truck and boom"<<endl<<endl;
        ss <<"(RIGHT MOUSE BUTTON DRAG): move over z axis / dolly"<<endl<<endl;
        ss <<"(h): Toggle help."<<endl;
        ofDrawBitmapString(ss.str().c_str(), 20, 20);
    }

}
//--------------------------------------------------------------
void ofApp::drawInteractionArea(){
	ofRectangle vp = ofGetCurrentViewport();
	float r = MIN(vp.width, vp.height) * 0.5f;
	float x = vp.width * 0.5f;
	float y = vp.height * 0.5f;

	ofPushStyle();
	ofSetLineWidth(3);
	ofSetColor(255, 255, 0);
	ofNoFill();
	glDepthMask(false);
	ofDrawCircle(x, y, r);
	glDepthMask(true);
	ofPopStyle();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key) {
		case 'C':
		case 'c':
			if(cam.getMouseInputEnabled()) cam.disableMouseInput();
			else cam.enableMouseInput();
			break;

		case 'F':
		case 'f':
			ofToggleFullscreen();
			break;
		case 'H':
		case 'h':
			bHelpText ^=true;
			break;
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
