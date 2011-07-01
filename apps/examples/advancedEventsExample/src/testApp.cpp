#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(50,50,50);

    enabled=false;

	// add the newFloat method to listen for eventsAddon newFloatEvent
	ofAddListener(evObject.newFloatEvent,this,&testApp::newFloat);

	// add the newFloat method to listen for eventsAddon newFloatEvent
	ofAddListener(evObject.newIntEvent,this,&testApp::newInt);
}


//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofDrawBitmapString(floatMsg,20,20);
	ofDrawBitmapString(intMsg,20,40);
	ofDrawBitmapString("click to enable/disable events",20,60);
}

//--------------------------------------------------------------
void testApp::newFloat(float & f){
	floatMsg 	= "newFloat event:  " + ofToString(f);
}

//--------------------------------------------------------------
void testApp::newInt(int & i){
	intMsg	 	= "newInt   event:  " + ofToString(i);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(enabled)
        evObject.disable();
    else
        evObject.enable();

    enabled=!enabled;

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

		

