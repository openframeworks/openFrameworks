#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofVideoGrabber cam;
	ofxCv::FlowFarneback flow;
    cv::Mat flow3;
    ofShader shader;
    float scaleFactor;
    ofTexture flowTexture;
    ofVboMesh mesh;
    
    int stepSize, xSteps, ySteps;
    cv::Mat accumulator;
    bool needToReset;
    
    float strength, learningRate;
    int blurAmount;
};