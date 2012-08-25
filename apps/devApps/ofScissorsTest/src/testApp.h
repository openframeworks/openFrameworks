#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void drawScissor();
    
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    
    bool    bToggleCenterMode;
    bool    bToggleFBO;
    bool    bSavePDF;
    ofImage img;
    ofFbo   fbo;
    
};
