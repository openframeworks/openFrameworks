#include "testApp.h"

	
//--------------------------------------------------------------
void testApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);

	ofBackground(255,255,0);

    rpiLogo.loadImage("Raspi_Colour_R.png");

}

//--------------------------------------------------------------
void testApp::exit(){
    ofLogVerbose("testApp") << "exit()";
}

//--------------------------------------------------------------
void testApp::update(){
    angle += 3;
    angle = angle % 360;
}

//--------------------------------------------------------------
void testApp::draw() {

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
void testApp::keyPressed(int key){
    ofLogVerbose("testApp") << "keyPressed() key = " << key;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    ofLogVerbose("testApp") << "keyReleased() key = " << key;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    ofLogVerbose("testApp") << "mouseMoved() x = " << x << "/" << y;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    ofLogVerbose("testApp") << "mouseDragged() x = " << x << "/" << y << " button=" << button;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    ofLogVerbose("testApp") << "mousePressed() x = " << x << "/" << y << " button=" << button;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    ofLogVerbose("testApp") << "mouseReleased() x = " << x << "/" << y << " button=" << button;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    ofLogVerbose("testApp") << "windowResized() w = " << w  << " h = " << h;
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    ofLogVerbose("testApp") << "gotMessage() msg.message = " << msg.message;
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    ofLogVerbose("testApp") << "dragEvent()";
}
