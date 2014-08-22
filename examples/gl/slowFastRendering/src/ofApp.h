#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void addPoint(float x, float y) {
        points.push_back(ofVec2f(x, y));
        speeds.push_back(ofVec2f(ofRandom(-1, 1), ofRandom(-1, 1)));
    }
    
    // a simple vector of points
    vector <ofVec2f> points;
    vector <ofVec2f> speeds;
    
    // in super fast mode we use a vbo
    ofVbo vbo;
    
    // mode switcher
    int mode;
    
    
};
