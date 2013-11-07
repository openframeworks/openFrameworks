#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxFontStash.h"



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void valueChange(int & value);

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
		
		ofImage image0,image1,image2;

		ofxPanel gui;
		ofxDropDownList testDropDownList;
		ofxGuiGroup testGroup;
		
		ofxContent testContent0,testContent1,testContent2;
		ofxQuadWarp testQuadWarp;
		ofxToggle testToggle;

		ofxFontStash unicodeFont;
};
