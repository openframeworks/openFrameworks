#pragma once

#include "ofxiOS.h"
#include "ofxiOSCoreMotion.h"
#include "Ball.h"

class ofApp : public ofxiOSApp{
	
public:
    
    void setup();
    void update();
    void draw();
    void exit();
    
    void touchDown(ofTouchEventArgs & touch);
    void touchMoved(ofTouchEventArgs & touch);
    void touchUp(ofTouchEventArgs & touch);
    void touchDoubleTap(ofTouchEventArgs & touch);
    void touchCancelled(ofTouchEventArgs & touch);
    
    void lostFocus();
    void gotFocus();
    void gotMemoryWarning();
    void deviceOrientationChanged(int newOrientation);
    
    void gotMessage(ofMessage msg);
    
    ofxiOSCoreMotion coreMotion;
    glm::vec3 accelerometerData;
    vector<Ball> balls;
    ofImage arrow;

};
