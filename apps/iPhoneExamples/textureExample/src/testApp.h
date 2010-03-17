#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"

class testApp : public ofxiPhoneApp {
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	void touchDoubleTap(ofTouchEventArgs &touch);

	void lostFocus();
	void gotFocus();
	void gotMemoryWarning();
	void deviceOrientationChanged(int newOrientation);

	ofTexture		texGray;
	ofTexture 		texColor;
	ofTexture		texColorAlpha;

	int 			w, h;
		
	unsigned char 	* colorPixels;
	unsigned char 	* grayPixels;
	unsigned char 	* colorAlphaPixels;	
};


