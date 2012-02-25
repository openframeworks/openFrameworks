#pragma once
#include "ofMain.h"


// ---------------------------------------------
class testApp : public ofBaseApp, public ofThread {
	
public:
    
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void threadedFunction();
    
    
    ofTrueTypeFont  font;
    vector <string> words;
    int             step;
    
    bool            bRandomVoice;
    string          voice;
    
};
