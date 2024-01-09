#pragma once

#include "ofxOpenCv.h"
#include "ofxNetwork.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"
#include "ofxAssimpModelLoader.h"
#include "ofxThreadedImageLoader.h"
#include "ofxKinect.h"
#include "ofxGui.h"
#include "ofxSvg.h"
#include "ofxXmlPoco.h"
#include "ofMain.h"

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

		// we don't actually use these
		// just checking to see if they
		// all work in the same place :)

		ofxCvGrayscaleImage cvGray;
		ofxTCPClient client;
		ofxTCPServer server;
		ofxOscSender osc_sender;
		ofxXmlSettings settings;
	ofxAssimpModelLoader betterModelLoader;
	ofxThreadedImageLoader threadedLoader;
	ofxKinect kinect;
	ofxPanel gui;
	ofxSvg svg;
	ofxXmlPoco xml;
};
