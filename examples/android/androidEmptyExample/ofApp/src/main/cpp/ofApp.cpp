#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(255,255,255);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();

    bool loadok = font.load("verdana.ttf", 100, true, false, true, 0.4, 72);


}

void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    int r = 128 + 128 * cosf(ofGetElapsedTimef());
    int g = 0;
    int b = 128 + 128 * sinf(ofGetElapsedTimef());

    ofBackground(r,g,b);
	
	ofSetColor(225);
	ofDrawBitmapString("ANDROID WORKING", 200, 300);


    ofSetColor(245, 58, 135);
    ofFill();

    font.drawStringAsShapes("meditate", 300, 400);



}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void ofApp::pause(){

}

//--------------------------------------------------------------
void ofApp::stop(){

}

//--------------------------------------------------------------
void ofApp::resume(){

}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

}

//--------------------------------------------------------------
bool ofApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void ofApp::okPressed(){

}

//--------------------------------------------------------------
void ofApp::cancelPressed(){

}

void ofApp::deviceRefreshRateChanged(int refreshRate) {

}

void ofApp::deviceHighestRefreshRateChanged(int refreshRate) {

}

void ofApp::deviceRefreshRateChangedEvent(int &refreshRate) {

}

void ofApp::deviceHighestRefreshRateChangedEvent(int &refreshRate) {

}
