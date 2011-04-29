#include "ofDrawBitmapString.h"

#include "ofGraphics.h"

//--------------------------------------------------
ofDrawBitmapString::ofDrawBitmapString(const ofPoint & p){
	pos = p;
	bPrinted = false;
}

//--------------------------------------------------
ofDrawBitmapString::ofDrawBitmapString(float x, float y, float z){
	pos.set(x, y, z);
	bPrinted = false;
}

//--------------------------------------------------
ofDrawBitmapString::ofDrawBitmapString(string textString, const ofPoint & p){
	ofGetDefaultRenderer()->drawString(textString, p.x, p.y, p.z, ofGetStyle().drawBitmapMode);
	bPrinted = true;
}

//--------------------------------------------------
ofDrawBitmapString::ofDrawBitmapString(string textString, float x, float y, float z){
	ofGetDefaultRenderer()->drawString(textString, x, y, z, ofGetStyle().drawBitmapMode);
	bPrinted = true;
}

//--------------------------------------------------
ofDrawBitmapString::~ofDrawBitmapString(){
	if(!bPrinted){
		ofGetDefaultRenderer()->drawString(message.str(), pos.x, pos.y, pos.z, ofGetStyle().drawBitmapMode);
	}
}
