#pragma once

#include "ofMain.h"
#include "ofxSimpleSlider.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void speedChanged();

		vector<ofPolyline> trail;
		float x, prevX;
		vector<float> y;
	
	
		float preSpeed;

		ofxSimpleSlider yFactor;
		ofxSimpleSlider speed;
		ofxSimpleSlider freq;
		bool showPos;
		float radius,initTime,t;
		int rightMargin;
};
