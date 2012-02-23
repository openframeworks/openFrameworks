#pragma once

#include "ofMain.h"

struct Link {
    Link(int _x, int _y):pos(_x,_y),previus(NULL){}; //Constructor
    
    ofVec2f pos;        // Position
    Link * previus;     // a pointer to the previus link
};

class testApp : public ofBaseApp{
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
		
    vector<Link*> chain;
    
    Link * last;
    
    bool bDrawArrows;
};
