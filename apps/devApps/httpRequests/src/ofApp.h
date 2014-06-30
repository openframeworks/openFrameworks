#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void keyPressed  (int key);
    void urlResponse(ofHttpResponse & response);
    void async(ofHttpRequest req);
    void sync(ofHttpRequest req);
};
