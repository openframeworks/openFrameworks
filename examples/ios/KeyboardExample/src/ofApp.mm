#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	keyboard = new ofxiOSKeyboard(0,52,320,32);
	keyboard->setVisible(true);
	keyboard->setBgColor(255, 255, 255, 255);
	keyboard->setFontColor(0,0,0, 255);
	keyboard->setFontSize(26);
}


//--------------------------------------------------------------
void ofApp::update(){
 
}

//--------------------------------------------------------------
void ofApp::draw(){	
	ofSetColor(180, 180, 180, 255);
	ofDrawBitmapString("tap the textfield to open the keyboard", 2, 10);

	ofSetColor(20, 160, 240, 255);
	ofDrawBitmapString("text entered = "+  keyboard->getText() , 2, 70);

}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
	
	if (touch.id == 1){
		
		if(!keyboard->isKeyboardShowing()){
			keyboard->openKeyboard();
			keyboard->setVisible(true);
		} else{
			keyboard->setVisible(false);
		}	
		
	}
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
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
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

