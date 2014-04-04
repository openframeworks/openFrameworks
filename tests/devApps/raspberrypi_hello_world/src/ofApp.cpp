#include "ofApp.h"

	
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);

	ofBackground(255,255,0);

    rpiLogo.loadImage("Raspi_Colour_R.png");

}

//--------------------------------------------------------------
void ofApp::exit(){
    ofLogVerbose("ofApp") << "exit()";
}

//--------------------------------------------------------------
void ofApp::update(){
    angle += 3;
    angle = angle % 360;
}

//--------------------------------------------------------------
void ofApp::draw() {

    ofSetColor(255);
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2.0f,ofGetHeight()/2.0f);
    ofRotateZ(angle);
    ofEnableAlphaBlending();
    rpiLogo.draw(-rpiLogo.width/2.0f,-rpiLogo.height/2.0f);
    ofDisableAlphaBlending();
    ofPopMatrix();

    // draw the mouse
    ofPushStyle();
    ofSetColor(255,0,0);
    ofNoFill();
    ofLine(ofGetMouseX(),0,ofGetMouseX(),ofGetHeight());
    ofLine(0,ofGetMouseY(),ofGetWidth(),ofGetMouseY());
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    ofLogVerbose("ofApp") << "keyPressed() key = " << key;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    ofLogVerbose("ofApp") << "keyReleased() key = " << key;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    ofLogVerbose("ofApp") << "mouseMoved() x = " << x << "/" << y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    ofLogVerbose("ofApp") << "mouseDragged() x = " << x << "/" << y << " button=" << button;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    ofLogVerbose("ofApp") << "mousePressed() x = " << x << "/" << y << " button=" << button;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    ofLogVerbose("ofApp") << "mouseReleased() x = " << x << "/" << y << " button=" << button;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    ofLogVerbose("ofApp") << "windowResized() w = " << w  << " h = " << h;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    ofLogVerbose("ofApp") << "gotMessage() msg.message = " << msg.message;
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    ofLogVerbose("ofApp") << "dragEvent()";
}
