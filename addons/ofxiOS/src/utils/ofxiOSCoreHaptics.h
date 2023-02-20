// created by artificiel 2023-02-15
//
// features to add:
//
//      - detect if haptics are supported on specific device to fail gracefully
//      - recover from going in BG (haptics are only for front app)
//

#pragma once
#import <CoreHaptics/CoreHaptics.h>

class ofxiOSCoreHaptics {
public:
    bool sendParameters(float intensity, float sharpness);
    void setup(); // will be lazy-called; usefull in ofApp::setup() to prevent small engine hiccup
    
private:
    bool prepare_engine();
    bool is_vibrating() ;
    bool is_vibrating_ {false};
    CHHapticEngine * engine_;
    id<CHHapticAdvancedPatternPlayer> player_;
};
