#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
	ofBackground(50, 50, 50);
	
    enabled = false;
	
	// add the newFloat method to listen for eventsAddon newFloatEvent
	ofAddListener(evObject.newFloatEvent, this, &ofApp::newFloat);
	
	// add the newFloat method to listen for eventsAddon newFloatEvent
	ofAddListener(evObject.newIntEvent, this, &ofApp::newInt);	
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofDrawBitmapString(floatMsg, 20, 20);
	ofDrawBitmapString(intMsg, 20, 40);
	ofDrawBitmapString("click to enable/disable events", 20, 60);
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::newFloat(float & f){
	floatMsg = "newFloat event:  " + ofToString(f);
	cout << floatMsg << "\n";
}

//--------------------------------------------------------------
void ofApp::newInt(int & i){
	intMsg = "newInt   event:  " + ofToString(i);
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
	if(enabled){
        evObject.disable();
    }else{
        evObject.enable();
    }
	
    enabled = !enabled;
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::lostFocus(){
    
}

//--------------------------------------------------------------
void ofApp::gotFocus(){
    
}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){
    
}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
    
}
