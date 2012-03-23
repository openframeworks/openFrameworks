#pragma once
#include "ofMain.h"

class Bullet {

public:
    
    ofVec2f pos, vel;
    bool    bRemove;
    
    void update() {
        pos += vel;
        vel *= 0.99; // slowdown in a long time
    }
    
    void draw() {
        ofSetColor(20);
        ofCircle(pos, 2);
    }
    
};
