#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(50,50,50);

	enabled=false;

	// add the newFloat method to listen for eventsAddon newFloatEvent
	ofAddListener(evObject.newFloatEvent,this,&ofApp::newFloat);

	// add the newInt method to listen for eventsAddon newIntEvent
	ofAddListener(evObject.newIntEvent,this,&ofApp::newInt);
}


//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofDrawBitmapString(floatMsg,20,20);
	ofDrawBitmapString(intMsg,20,40);
	ofDrawBitmapString("click to enable/disable events",20,60);
}

//--------------------------------------------------------------
void ofApp::newFloat(float & f){
	floatMsg 	= "newFloat event:  " + ofToString(f);
}

//--------------------------------------------------------------
void ofApp::newInt(int & i){
	intMsg	 	= "newInt   event:  " + ofToString(i);
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
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
	if(enabled)
		evObject.disable();
	else
		evObject.enable();

	enabled=!enabled;

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

		

