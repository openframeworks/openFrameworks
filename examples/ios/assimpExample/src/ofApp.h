#pragma once

#include "ofxiOS.h"
#include "ofxAssimpModelLoader.h"

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
    
    void loadModel(int aindex);
    void loadModel(string filename);
    void showNextAnimation();
    void updateAnimation(float val);
    void loadModel(bool decrease = false);
    void toggleAnimation(bool t);
    void toggleCamera(bool t);
    
    ofxAssimpModelLoader model;
    ofLight    light;
    ofEasyCam cam;
    
    vector<string> modelPaths;
    
    int animationIndex = 0;
    int modelIndex;
    
    float animationPosition = 0;
    
    bool bUseCamera;
    bool bAnimate;
};

