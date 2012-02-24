#pragma once
#include "ofMain.h"


// ---------------------------------------------
class ofApp : public ofBaseApp {
	
public:
    
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
    ofTrueTypeFont  font;
    vector <string> words;
    int             step;
    
    bool            bRandomVoice;
    string          voice;
    
};
