#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
	ofBackground(225, 225, 225);
	ofSetCircleResolution(80);
    ofSetLogLevel(OF_LOG_VERBOSE);
	
	// initialize the accelerometer
    coreMotion.setupAccelerometer();
	
	balls.assign(10, Ball());
	
	arrow.load("arrow.png");
	arrow.setAnchorPercent(1.0, 0.5);
	
	// initialize all of the Ball particles
	for(int i=0; i<balls.size(); i++){
		balls[i].init(i);
	}
}


//--------------------------------------------------------------
void ofApp::update() {
    
    coreMotion.update();
    accelerometerData = coreMotion.getAccelerometerData();
    
	for(int i=0; i < balls.size(); i++){
		balls[i].update(accelerometerData);
	}
    ofLog(OF_LOG_VERBOSE, "x = %f, y = %f", accelerometerData.x, accelerometerData.y);
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(54);
	ofDrawBitmapString("Multitouch and Accel Example", 10, 20);

	float angle = 180 - RAD_TO_DEG * atan2(accelerometerData.y, accelerometerData.x);

	ofEnableAlphaBlending();
	ofSetColor(255);
	ofPushMatrix();
		ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0);
		ofRotateZ(angle);
		arrow.draw(0,0);
	ofPopMatrix();

	ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
		for(int i = 0; i< balls.size(); i++){
			balls[i].draw();
		}
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
    ofLog(OF_LOG_VERBOSE, "touch %d down at (%i,%i)", touch.id, (int)touch.x, (int)touch.y);
	balls[touch.id].moveTo(touch.x, touch.y);
	balls[touch.id].bDragged = true;
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
    ofLog(OF_LOG_VERBOSE, "touch %d moved at (%i,%i)", touch.id, (int)touch.x, (int)touch.y);
	balls[touch.id].moveTo(touch.x, touch.y);
	balls[touch.id].bDragged = true;	
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
    ofLog(OF_LOG_VERBOSE, "touch %d up at (%i,%i)", touch.id, (int)touch.x, (int)touch.y);
	balls[touch.id].bDragged = false;
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){
    ofLog(OF_LOG_VERBOSE, "touch %d double tap at (%i,%i)", touch.id, (int)touch.x, (int)touch.y);
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

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}

