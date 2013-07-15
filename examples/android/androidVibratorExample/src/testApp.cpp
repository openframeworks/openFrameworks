#include "testApp.h"
#include "ofxAndroidVibrator.h"

//--------------------------------------------------------------
void testApp::setup(){
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
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255);
	for(int i=0;i<areas.size();i++){
		ofRect(areas[i]);
	}
	ofSetColor(255,0,0);
	ofDrawBitmapString(ofToString(ofGetWidth())+","+ofToString(ofGetHeight()),20,20);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::touchDown(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::touchMoved(int x, int y, int id){
	bool found=false;
	for(int i=0;i<areas.size();i++){
		if(areas[i].inside(ofVec2f(x,y))){
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
void testApp::touchUp(int x, int y, int id){
	inside = false;
	ofxAndroidVibrator::stop();
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void testApp::pause(){

}

//--------------------------------------------------------------
void testApp::stop(){

}

//--------------------------------------------------------------
void testApp::resume(){

}

//--------------------------------------------------------------
void testApp::reloadTextures(){

}

//--------------------------------------------------------------
bool testApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void testApp::okPressed(){

}

//--------------------------------------------------------------
void testApp::cancelPressed(){

}
