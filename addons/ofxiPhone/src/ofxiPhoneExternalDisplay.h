//
//  ofxiPhoneExternalDisplay.h
//
//  Created by lukasz karluk on 21/03/12.
//  http://julapy.com
//

#pragma once

#import "ofMain.h"

struct ofxiPhoneExternalDisplayMode{
    int width;
    int height;
    float pixelAspectRatio;
};

class ofxiPhoneExternalDisplay {
    
public:
    
    //-------------------------------------------------------
	static void alertExternalDisplayConnected();
   	static void alertExternalDisplayDisconnected();
    static void alertExternalDisplayChanged();
    
    //-------------------------------------------------------
    static vector<ofxiPhoneExternalDisplayMode> getExternalDisplayModes();
    static bool displayOnExternalScreen(ofxiPhoneExternalDisplayMode externalDisplayMode);
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
    ofxiPhoneExternalDisplay();
    ~ofxiPhoneExternalDisplay();
    
    //-------------------------------------------------------
	virtual void externalDisplayConnected(){}
	virtual void externalDisplayDisconnected(){}
    virtual void externalDisplayChanged(){}
    
};