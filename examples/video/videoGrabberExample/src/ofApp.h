#pragma once

#include "ofMain.h"

// This openFrameworks example is designed to demonstrate how to access the
// webcam.
//
// For more information regarding this example take a look at the README.md.

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofVideoGrabber vidGrabber;
		ofPixels videoInverted;
		ofTexture videoTexture;
		int camWidth;
		int camHeight;
};
