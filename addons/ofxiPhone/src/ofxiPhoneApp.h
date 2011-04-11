/*
 *  ofxiPhoneApp.h
 *  MSA Labs Vol. 1
 *
 *  Created by Mehmet Akten on 14/07/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxiPhoneAlerts.h"
#include "ofxMultitouch.h"

class ofxiPhoneApp : public ofSimpleApp, public ofxiPhoneAlertsListener, public ofxMultiTouchListener {
	
public:
    virtual ~ofxiPhoneApp() {};
    
	virtual void setup() {};
	virtual void update() {};
	virtual void draw() {};
	virtual void exit() {};
	
	virtual void lostFocus() {}
	virtual void gotFocus() {}
	virtual void gotMemoryWarning() {}

    
	virtual void touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL) {}
	virtual void touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL) {}
	virtual void touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL) {}
	virtual void touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL) {}
	virtual void touchCancelled(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL) {}
    
	virtual void touchDown(ofTouchEventArgs & touch){
		touchDown(touch.x,touch.y,touch.id, &touch);
	}
	virtual void touchMoved(ofTouchEventArgs & touch){
		touchMoved(touch.x,touch.y,touch.id, &touch);
	}
	virtual void touchUp(ofTouchEventArgs & touch){
		touchUp(touch.x,touch.y,touch.id, &touch);
	}
	virtual void touchDoubleTap(ofTouchEventArgs & touch){
		touchDoubleTap(touch.x,touch.y,touch.id, &touch);
	}
	virtual void touchCancelled(ofTouchEventArgs & touch){
		touchCancelled(touch.x,touch.y,touch.id, &touch);
	}

};

