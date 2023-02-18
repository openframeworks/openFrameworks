// created by artificiel 2023-02-15
//
// features to add:
//
//      - detect if haptics are supported to fail gracefully
//      - recover from going in BG (haptics are only for front app)
//

#pragma once
#import <CoreHaptics/CoreHaptics.h>

class ofxiOSCoreHaptics {
public:
    auto sendParameters(float intensity, float sharpness) -> void;
    auto setup(); // will be lazy-called; usefull in ofApp::setup() to prevent small engine hiccup
    
private:
    auto prepare_engine();
    auto is_vibrating();
    bool is_vibrating_ {false};
    CHHapticEngine * engine_;
    id<CHHapticAdvancedPatternPlayer> player_;
};
