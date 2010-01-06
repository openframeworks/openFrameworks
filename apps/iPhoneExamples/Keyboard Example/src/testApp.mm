
#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){	
	ofBackground(50,50,50);
	// touch events will be sent to myTouchListener
	ofxMultiTouch.addListener(this);
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
		
	keyboard = new ofxiPhoneKeyboard(0,32,300,32);
	keyboard->setVisible(true);
	keyboard->setBgColor(0, 0, 0, 255);
	keyboard->setFontColor(255,255,255,255);
	keyboard->setFontSize(26);
	
}


//--------------------------------------------------------------
void testApp::update()
{
	
}

//--------------------------------------------------------------
void testApp::draw()
{
}

void testApp::exit() {
	printf("exit()\n");
}

//--------------------------------------------------------------
void testApp::touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data){
	
	printf("touch %i down at (%i,%i)\n", touchId, x,y);
	
	if(!keyboard->isKeyboardShowing())
		keyboard->openKeyboard();
}

//--------------------------------------------------------------
void testApp::touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data){
}

//--------------------------------------------------------------
void testApp::touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data){
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data){
}
//--------------------------------------------------------------
void testApp::lostFocus()
{
}
//--------------------------------------------------------------
void testApp::gotFocus()
{
}
//--------------------------------------------------------------
void testApp::gotMemoryWarning()
{
}

