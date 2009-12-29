
#include "testApp.h"



//--------------------------------------------------------------
void testApp::setup(){	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	// touch events will be sent to myTouchListener
	ofxMultiTouch.addListener(this);
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
	
	// initialize all of the Thing particles
	for(int i=0; i<NUM_POINTS; i++) balls[i].init();
}


//--------------------------------------------------------------
void testApp::update()
{
	for(int i=0; i<NUM_POINTS; i++) balls[i].update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	for(int i=0; i<NUM_POINTS; i++) balls[i].draw();
}

void testApp::exit() {
	printf("exit()\n");
}

//--------------------------------------------------------------
void testApp::touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data){
	printf("touch %i down at (%i,%i)\n", touchId, x,y);
	balls[touchId].moveTo(x, y);
}
//--------------------------------------------------------------
void testApp::touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data){
	printf("touch %i moved at (%i,%i)\n", touchId, x,y);
	balls[touchId].moveTo(x, y);
}
//--------------------------------------------------------------
void testApp::touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data){
	printf("touch %i up at (%i,%i)\n", touchId, x,y);
}
//--------------------------------------------------------------
void testApp::touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data){
	printf("touch %i double tap at (%i,%i)\n", touchId, x,y);
}
//--------------------------------------------------------------
void testApp::lostFocus()
{
	cout<<"lostFocus"<<endl;
}
//--------------------------------------------------------------
void testApp::gotFocus()
{
	cout<<"gotFocus"<<endl;
}
//--------------------------------------------------------------
void testApp::gotMemoryWarning()
{
	cout<<"MemoryWarning"<<endl;
}

