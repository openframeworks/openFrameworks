//
//  ofxiOSExternalDisplay.h
//
//  Created by lukasz karluk on 21/03/12.
//  http://julapy.com
//

#pragma once

#include "ofConstants.h"

struct ofxiOSExternalDisplayMode{
    int width;
    int height;
    float pixelAspectRatio;
};

class ofxiOSExternalDisplay {
    
public:
    
    //-------------------------------------------------------
	static void alertExternalDisplayConnected();
   	static void alertExternalDisplayDisconnected();
    static void alertExternalDisplayChanged();
    
    //-------------------------------------------------------
    static vector<ofxiOSExternalDisplayMode> getExternalDisplayModes();
    static bool displayOnExternalScreen(ofxiOSExternalDisplayMode externalDisplayMode);
    static bool displayOnExternalScreenWithPreferredDisplayMode();
    static bool displayOnDeviceScreen();
    static bool mirrorOn();
    static bool mirrorOff();

    //-------------------------------------------------------
    static bool isDisplayingOnExternalScreen();
    static bool isDisplayingOnDeviceScreen();
    static bool isExternalScreenConnected();
    static bool isMirroring();
 
    //-------------------------------------------------------
    ofxiOSExternalDisplay();
    ~ofxiOSExternalDisplay();
    
    //-------------------------------------------------------
	virtual void externalDisplayConnected(){}
	virtual void externalDisplayDisconnected(){}
    virtual void externalDisplayChanged(){}
    
};

#define ofxiPhoneExternalDisplay ofxiOSExternalDisplay
