#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	// register touch events
	ofxRegisterMultitouch(this);
	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
	
	// initialize all of the Ball particles
	for(int i=0; i<NUM_POINTS; i++) balls[i].init();
}


//--------------------------------------------------------------
void testApp::update() {
	for(int i=0; i<NUM_POINTS; i++) balls[i].update();
	printf("x = %f   y = %f \n", ofxAccelerometer.getForce().x, ofxAccelerometer.getForce().y);
}

//--------------------------------------------------------------
void testApp::draw() {
	for(int i=0; i<NUM_POINTS; i++) balls[i].draw();
}

//--------------------------------------------------------------
void testApp::exit() {
	printf("exit()\n");
}

//--------------------------------------------------------------
void testApp::touchDown(int x, int y, int id){
	printf("touch %i down at (%i,%i)\n", id, x,y);
	balls[id].moveTo(x, y);
}

//--------------------------------------------------------------
void testApp::touchMoved(int x, int y, int id){
	printf("touch %i moved at (%i,%i)\n", id, x, y);
	balls[id].moveTo(x, y);
}

//--------------------------------------------------------------
void testApp::touchUp(int x, int y, int id){
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

