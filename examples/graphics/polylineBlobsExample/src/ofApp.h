#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp {
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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofVideoGrabber cam;
		ofxCvColorImage cvImgColor;
		ofxCvGrayscaleImage cvImgGrayscale;
		ofxCvContourFinder contourFinder;
		vector<ofPolyline> polylines, smoothed, resampled;
		vector<ofRectangle> boundingBoxes;
		vector<glm::vec3> closestPoints;
		vector<unsigned int> closestIndices;
		int camWidth, camHeight;
		
		int threshold;	
};
