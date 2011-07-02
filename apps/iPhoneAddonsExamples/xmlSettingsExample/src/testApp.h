#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#include "ofMain.h"

#include "ofxXmlSettings.h"

#define NUM_PTS 800

class testApp : public ofxiPhoneApp{

	public:

		void setup();
		void update();
		void draw();

		void touchDown(ofTouchEventArgs &touch);
		void touchMoved(ofTouchEventArgs &touch);
		void touchUp(ofTouchEventArgs &touch);
		void touchDoubleTap(ofTouchEventArgs &touch);
		void touchCancelled(ofTouchEventArgs &touch);

		ofxXmlSettings XML;
		ofTrueTypeFont TTF;

		string xmlStructure;
		string message;

		ofPoint dragPts[NUM_PTS];

		int pointCount;
		int lineCount;
		int lastTagNumber;

		float red;
		float green;
		float blue;
};

