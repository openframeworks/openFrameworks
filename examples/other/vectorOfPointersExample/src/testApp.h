#pragma once

#include "ofMain.h"

struct Link {
    Link(int _x, int _y):pos(_x,_y),prev(NULL),next(NULL){}; //Constructor
    
    ofVec2f pos;    // Position
    Link * prev;    // a pointer to the previus link
    Link * next;    // a pointer to the next link
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
	
	void createNew(int _x, int _y);
    void deleteLast();
    void deleteAll();
    
    void drawArrow(ofPoint from, ofPoint to, float size);
    
    vector<Link*> chain;
    
    Link * last;
    
    int blink;
    int time;
    bool bArrowToPrev;
};
