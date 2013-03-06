#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void drawScene(bool isPreview);
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

		ofVideoGrabber video;
		ofxCvHaarFinder finder;
	
		//the view window is defined by 3 corners
		ofVec3f windowTopLeft;
		ofVec3f windowBottomLeft;
		ofVec3f windowBottomRight;
		ofCamera headTrackedCamera;
		ofEasyCam previewCamera;
	
		bool usePreview;
		float windowWidth;
		float windowHeight;
		float viewerDistance;
	
		deque<ofPoint> headPositionHistory;
};
