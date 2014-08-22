#pragma once

#include "ofMain.h"

//--------------------------------------------------------------
// OF logging example
//
// This logging example shows you how to use OF's built-in logging
// functions to generate logging output. It demonstrates the most
// important features of ofLog.
// Please note that the different logging levels are for demonstration
// purposes, so don't be alarmed if you see fatal errors in your console ;-)

class ofApp : public ofBaseApp {

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

		// Variables for logging
		int startTime;
		int elapsedTime;
		int triggerOne;
		int triggerTwo;
		
		bool verboseLevel;
		bool logToFile;
		bool currentVerboseLevel;
		bool currentLogToFile;

};
