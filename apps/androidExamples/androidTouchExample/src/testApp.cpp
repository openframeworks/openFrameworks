#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	// register touch events
	ofxRegisterMultitouch(this);
	
	// initialize the accelerometer
	ofxAccelerometer.setup();

	// initialize all of the Ball particles
	for(int i=0; i<NUM_POINTS; i++) balls[i].init();
}


//--------------------------------------------------------------
void testApp::update() {
	for(int i=0; i<NUM_POINTS; i++) balls[i].update();
}

//--------------------------------------------------------------
void testApp::draw() {
	for(int i=0; i<NUM_POINTS; i++) balls[i].draw();
}

//--------------------------------------------------------------
void testApp::exit() {
	ofLog(OF_LOG_NOTICE,"exit()\n");
}

//--------------------------------------------------------------
void testApp::touchDown(int x, int y, int id){
	ofLog(OF_LOG_NOTICE,"touch %i down at (%i,%i)\n", id, x,y);
	balls[id].moveTo(x, y);
}

//--------------------------------------------------------------
void testApp::touchMoved(int x, int y, int id){
	ofLog(OF_LOG_NOTICE,"touch %i moved at (%i,%i)\n", id, x, y);
	balls[id].moveTo(x, y);
}

//--------------------------------------------------------------
void testApp::touchUp(int x, int y, int id){
	ofLog(OF_LOG_NOTICE,"touch %i up at (%i,%i)\n", id, x, y);
}

