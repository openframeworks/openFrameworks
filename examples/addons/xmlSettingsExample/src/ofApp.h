#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

#define NUM_PTS 800

class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

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

