#pragma once

#include "ofMain.h"

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
	
	//this holds all of our points
    vector<ofVec3f> points;
    //this keeps track of the center of all the points
    ofVec3f center;
    
	//our camera objects for looking at the scene from multiple perspectives
	ofCamera camera;
	
	//if usecamera is true, we'll turn on the camera view
    bool usecamera;
};
