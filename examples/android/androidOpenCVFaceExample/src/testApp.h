#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxOpenCv.h"
#include <vector>

class testApp : public ofBaseApp{
	
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
		void windowResized(int w, int h);


		void drawButton(const string& s, int x, int y, int w, int h, bool isToggled);

		ofVideoGrabber grabber;
		ofxCvColorImage colorCv;
		ofxCvColorImage colorCvSmall;
		ofxCvGrayscaleImage grayCv;
		ofxCvHaarFinder faceFinder;
		std::vector<ofxCvBlob> faces;

		void exit();

		int one_second_time;
		int camera_fps;
		int frames_one_sec;
};

#endif	

