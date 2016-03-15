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

//		std::unique_ptr<ofx::DOM::Document> gui_doc;

//		ofxGuiTabs *tabbed_pages;

//		ofxPanel *page1, *page2;

//		ofxPanel *panel1, *panel2;

		ofParameterGroup group;
		ofParameter <float> parameter;
//		ofParameter <bool> toggle2_param, toggle3_param, toggle4_param;

};

