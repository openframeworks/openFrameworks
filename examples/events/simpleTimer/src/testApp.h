#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    void gotMessage(ofMessage msg);
    
    float startTime;        // store when we start time timer
    float endTime;          // when do want to stop the timer
    
    bool  bTimerReached;    // used as a trigger when we hit the timer
    
};
