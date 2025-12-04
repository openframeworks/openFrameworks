#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofLogNotice() << "Resolved path: " << ofToDataPath("hands.mp4", true);


    ofDirectory dir(ofToDataPath("", true));
    dir.listDir();

    ofLogNotice() << "Listing of: " << dir.getAbsolutePath();
    for (size_t i = 0; i < dir.size(); ++i) {
        ofLogNotice() << "  " << dir.getFile(i).getFileName();
    }

    player.load("hands.mp4");
	player.play();

}

//--------------------------------------------------------------
void ofApp::update(){

	player.update();

}

//--------------------------------------------------------------
void ofApp::draw(){

	player.draw(0,0);

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

//--------------------------------------------------------------
void ofApp::deviceHighestRefreshRateChanged(int refreshRate) {
}

//--------------------------------------------------------------
void ofApp::deviceRefreshRateChangedEvent(int &refreshRate) {
}

//--------------------------------------------------------------
void ofApp::deviceHighestRefreshRateChangedEvent(int &refreshRate) {
}

//--------------------------------------------------------------
void ofApp::exit(){

}
