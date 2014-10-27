#pragma once

#include "ofMain.h"

#include "NoiseGenerator.h"
#include "LowPassFilter.h"
#include "DigitalDelay.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofSoundStream soundStream;
	
	// these are all subclasses of ofSoundObject
	NoiseGenerator noise;
	LowPassFilter filter;
	DigitalDelay delay;
	
	float filterCutoff;
	float delayFeedback;
	
	ofTrueTypeFont font;
};
