/***********************************************************************
 
 Copyright (c) 2008, 2009, Zach Gage, stfj
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of stfj nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/ 

#pragma once

#include <list>
#include "ofxiOSAlertsListener.h"

class ofxiOSAlertsHandler : public ofxiOSAlertsListener {
public:
	void addListener(ofxiOSAlertsListener* o) {
		listeners.push_back(o);
	}
	
	void removeListener(ofxiOSAlertsListener* o) {
		listeners.remove(o);
	}
	
	//alerts engine will call this when the program loses focus (e.g. the user receives a text message or phone call, or turns off the display)
	void lostFocus()
	{
		for(std::list<ofxiOSAlertsListener*>::iterator it=listeners.begin(); it!=listeners.end(); ++it) {
			ofxiOSAlertsListener* o = *it;
			o->lostFocus();
		}
	}
	
	//alerts engine will call this when the program regains focus
	void gotFocus()
	{
		for(std::list<ofxiOSAlertsListener*>::iterator it=listeners.begin(); it!=listeners.end(); ++it) {
			ofxiOSAlertsListener* o = *it;
			o->gotFocus();
		}
	}
	
	//alerts engine will call this when the program receives a memory warning 
	void gotMemoryWarning()
	{
		for(std::list<ofxiOSAlertsListener*>::iterator it=listeners.begin(); it!=listeners.end(); ++it) {
			ofxiOSAlertsListener* o = *it;
			o->gotMemoryWarning();
		}
	}
    
    //alerts engine will call this when the program receives an orientation changed notification 
	void deviceOrientationChanged(int newOrientation)
    {
		for(std::list<ofxiOSAlertsListener*>::iterator it=listeners.begin(); it!=listeners.end(); ++it) {
			ofxiOSAlertsListener* o = *it;
			o->deviceOrientationChanged(newOrientation);
		}
	}
	
	//alerts engine will call this when the program is launched with a url
	void launchedWithURL(std::string url)
	{
		for(std::list<ofxiOSAlertsListener*>::iterator it=listeners.begin(); it!=listeners.end(); ++it) {
			ofxiOSAlertsListener* o = *it;
			o->launchedWithURL(url);
		}
	}
	
	
protected:
	std::list<ofxiOSAlertsListener*> listeners;
	
};

#define ofxiPhoneAlerts ofxiOSAlerts

extern ofxiOSAlertsHandler ofxiOSAlerts;

#define ofxiPhoneAlertsHandler ofxiOSAlertsHandler

