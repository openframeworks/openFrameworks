#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(ofMouseEventArgs & args);
		void mouseDragged(ofMouseEventArgs & args);
		void mousePressed(ofMouseEventArgs & args);
		void mouseReleased(ofMouseEventArgs & args);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	private:

		ofxGuiPage page;

		ofxTabbedPages tabbed_pages;
		ofxGuiPage page1, page2;

		ofxPanel panel1, panel2, panel3;

		void setupPage1();
		void setupPage2();
		void setupPage3();

		ofParameter <float> slider_param;
        ofParameter <bool> toggle2_param, toggle3_param, toggle4_param;

};

