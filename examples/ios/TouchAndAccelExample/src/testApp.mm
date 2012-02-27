#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	ofBackground(225, 225, 225);
	ofSetCircleResolution(80);
	
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
	printf("x = %f   y = %f \n", ofxAccelerometer.getForce().x, ofxAccelerometer.getForce().y);
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
void testApp::exit() {

}

//--------------------------------------------------------------
void testApp::touchDown(int x, int y, int id){
	printf("touch %i down at (%i,%i)\n", id, x,y);
	balls[id].moveTo(x, y);
	balls[id].bDragged = true;
}

//--------------------------------------------------------------
void testApp::touchMoved(int x, int y, int id){
	printf("touch %i moved at (%i,%i)\n", id, x, y);
	balls[id].moveTo(x, y);
	balls[id].bDragged = true;	
}

//--------------------------------------------------------------
void testApp::touchUp(int x, int y, int id){
	balls[id].bDragged = false;
	printf("touch %i up at (%i,%i)\n", id, x, y);
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(int x, int y, int id){
	printf("touch %i double tap at (%i,%i)\n", id, x, y);
}

//--------------------------------------------------------------
void testApp::lostFocus() {
}

//--------------------------------------------------------------
void testApp::gotFocus() {
}

//--------------------------------------------------------------
void testApp::gotMemoryWarning() {
}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){
}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs& args){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

