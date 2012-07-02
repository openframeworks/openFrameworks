#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);		

	void ringButtonPressed(bool & pressed);
	void grabScreenshot();
	void grabScreenshot(bool & pressed) { cout << "Pressed: "<< pressed<<endl; if (pressed) grabScreenshot(); };

	bool bHide;

	ofxFloatSlider radius;
	ofxIntSlider r, g, b;
	ofxIntSlider circleResolution;
	ofxToggle filled;
	ofxButton twoCircles;
	ofxButton ringButton;
	ofImage screenshotImage;
	ofxGuiImage screenshotGui;

	ofxPanel gui;

	ofSoundPlayer ring;
};

