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


class ofxiPhoneApp : public ofSimpleApp, public ofxiPhoneAlertsListener {
	
public:
	void setup() {};
	void update() {};
	void draw() {};
	void exit() {};
	
	void touchDown(ofTouchEventArgs & touch) {};
	void touchMoved(ofTouchEventArgs & touch) {};
	void touchUp(ofTouchEventArgs & touch) {};
	void touchDoubleTap(ofTouchEventArgs & touch) {};
	
	void lostFocus() {}
	void gotFocus() {}
	void gotMemoryWarning() {}
};

