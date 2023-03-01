// Project Title: Slitscan Radial Clock
// Description: In this example we use the oF video grabber to sample camera input. Then the code accesses the pixels in each frame from the
// camera to create a slitscan video. The image is created in two styles, progressively across the screen or as a ribbon, pushing pixels
// sampled from the center of the screen. Rendered to the screen as a flat, horizontal image or using oftransform to display as a radial image.
// ©Daniel Buzzo 2020, 21, 22
// dan@buzzo.com
// http://buzzo.com
// https://github.com/danbz
// https://www.youtube.com/danbuzzo
//

#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void calculateTime();
    
    // initiate a videograbber object and objects to put image pixel data into
    ofVideoGrabber vidGrabber;
    ofPixels videoPixels, pixels;
    ofTexture videoTexture;
    
    int xSteps, scanStyle, seconds, minutes, hours, numOfSecs, numOfMins, numOfHours;
    bool b_radial, b_debug;
    float currTime, camWidth, camHeight;
    string scanName, time;
    ofTrueTypeFont font;
};
