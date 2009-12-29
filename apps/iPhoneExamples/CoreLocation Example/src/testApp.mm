
#include "testApp.h"



//--------------------------------------------------------------
void testApp::setup(){	
	ofBackground(0,0,0);

	coreLocation = new ofxiPhoneCoreLocation();
	hasCompass = coreLocation->startHeading();
	hasGPS = coreLocation->startLocation();
}


//--------------------------------------------------------------
void testApp::update()
{
	
}

//--------------------------------------------------------------
void testApp::draw()
{
	if(hasCompass)
	{
		//compass 3GS only:
		float cx = cos(ofDegToRad(coreLocation->getTrueHeading()) ) * 100;
		float cy = sin(ofDegToRad(coreLocation->getTrueHeading()) ) * 100;
		float x = ofGetWidth()/2;
		float y = ofGetHeight()/2;
		ofSetColor(255,255,255);
		ofLine(x,y,x+cx,y+cy);
		cout<<coreLocation->getTrueHeading()<<" | ";
	}
	if(hasGPS)
	{
		cout<<coreLocation->getLatitude()<<" | "<<coreLocation->getLatitude()<<endl;
	}
}

void testApp::exit() {
}


//--------------------------------------------------------------
void testApp::touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data){
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
void testApp::lostFocus() {
}

//--------------------------------------------------------------
void testApp::gotFocus()
{
}

//--------------------------------------------------------------
void testApp::gotMemoryWarning()
{
}

