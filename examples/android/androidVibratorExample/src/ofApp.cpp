#include "ofApp.h"
#include "ofxAndroidVibrator.h"

//--------------------------------------------------------------
void ofApp::setup(){
	float y=0;
	float height=2;
	ofLogError() << ofGetWidth() << "," << ofGetHeight();
	while(y<ofGetHeight()){
		areas.push_back(ofRectangle(0,y,ofGetWidth(),height));
		y+=height*2.5;
		height+=1;
	}
	ofBackground(0);
	inside = false;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);
	for(int i=0;i<areas.size();i++){
		ofDrawRectangle(areas[i]);
	}
	ofSetColor(255,0,0);
	ofDrawBitmapString(ofToString(ofGetWidth())+","+ofToString(ofGetHeight()),20,20);
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
	bool found=false;
	for(int i=0;i<areas.size();i++){
		if(areas[i].inside(x,y)){
			found=true;
			break;
		}
	}
	if((!inside && found) || (inside && !found)){
		ofxAndroidVibrator::vibrate(50);
	}else if(!inside && !found){
		ofxAndroidVibrator::stop();
	}
	inside = found;
}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){
	inside = false;
	ofxAndroidVibrator::stop();
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
