#pragma once

#include "ofxtvOS.h"

class ofApp : public ofxiOSApp {
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void controllerPressed(ofControllerEventArgs & event);
    void controllerReleased(ofControllerEventArgs & event);
    
    void lostFocus();
    void gotFocus();
    void gotMemoryWarning();
    
};