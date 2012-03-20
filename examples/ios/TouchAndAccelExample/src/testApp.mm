#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	ofBackground(225, 225, 225);
	ofSetCircleResolution(80);
    ofSetLogLevel(OF_LOG_VERBOSE);
	
	// register touch events
	ofxRegisterMultitouch(this);
	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
	
	balls.assign(10, Ball());
	
	arrow.loadImage("arrow.png");
	arrow.setAnchorPercent(1.0, 0.5);
	
	// initialize all of the Ball particles
	for(int i=0; i<balls.size(); i++){
		balls[i].init(i);
	}
}


//--------------------------------------------------------------
void testApp::update() {
	for(int i=0; i < balls.size(); i++){
		balls[i].update();
	}
    ofLog(OF_LOG_VERBOSE, "x = %f, y = %f", ofxAccelerometer.getForce().x, ofxAccelerometer.getForce().y);
}

//--------------------------------------------------------------
void testApp::draw() {
	ofSetColor(54);
	ofDrawBitmapString("Multitouch and Accel Example", 10, 20);

	float angle = 180 - RAD_TO_DEG * atan2( ofxAccelerometer.getForce().y, ofxAccelerometer.getForce().x );

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
void testApp::exit(){

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch){
    ofLog(OF_LOG_VERBOSE, "touch %d down at (%d,%d)", touch.id, touch.x, touch.y);
	balls[touch.id].moveTo(touch.x, touch.y);
	balls[touch.id].bDragged = true;
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs & touch){
    ofLog(OF_LOG_VERBOSE, "touch %d moved at (%d,%d)", touch.id, touch.x, touch.y);
	balls[touch.id].moveTo(touch.x, touch.y);
	balls[touch.id].bDragged = true;	
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs & touch){
    ofLog(OF_LOG_VERBOSE, "touch %d up at (%d,%d)", touch.id, touch.x, touch.y);
	balls[touch.id].bDragged = false;
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs & touch){
    ofLog(OF_LOG_VERBOSE, "touch %d double tap at (%d,%d)", touch.id, touch.x, touch.y);
}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void testApp::lostFocus(){
    
}

//--------------------------------------------------------------
void testApp::gotFocus(){
    
}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){
    
}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

