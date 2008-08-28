#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#define OF_ADDON_USING_OFXOPENCV

#include "ofAddons.h"

//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
								// otherwise, we'll use a movie file


class testApp : public ofSimpleApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();

        #ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber 		vidGrabber;
		#else
		  ofVideoPlayer 		vidPlayer;
		#endif
        
        ofxCvColorImage		colorImg;
        
        ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;
		
        ofxCvContourFinder 	contourFinder;

		int 				threshold;
		bool				bLearnBakground;
		
		
};

#endif
