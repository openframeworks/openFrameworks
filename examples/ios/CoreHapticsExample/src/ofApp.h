#pragma once

#include "ofxiOS.h"
#include "ofxiOSCoreHaptics.h"

class ofApp : public ofxiOSApp {
    
    ofxiOSCoreHaptics haptics_;
    void update() override;
    
};


