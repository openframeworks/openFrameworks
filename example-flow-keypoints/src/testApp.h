#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    ofVideoGrabber grabber;
    ofxCv::FlowPyrLK flow;
    ofVec2f p1;
    ofRectangle rect;
};
