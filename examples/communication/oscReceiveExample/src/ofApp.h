#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

// listening port
#define PORT 12345

// max number of strings to display
#define NUM_MSG_STRINGS 20

// demonstrates receiving and processing OSC messages with an ofxOscReceiver,
// use in conjunction with the oscSenderExample
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

		ofTrueTypeFont font;
		ofxOscReceiver receiver;

		int currentMsgString;
		string msgStrings[NUM_MSG_STRINGS];
		float timers[NUM_MSG_STRINGS];

		float mouseXf = 0;
		float mouseYf = 0;
		int mouseButtonInt = 0;
		string mouseButtonState = "";

		ofImage receivedImage;
};
