#pragma once


#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"


//ON IPHONE NOTE INCLUDE THIS BEFORE ANYTHING ELSE
#include "ofxOpenCv.h"

//warning video player doesn't currently work - use live video only
//#define _USE_LIVE_VIDEO

class testApp : public ofxiPhoneApp{
	
	public:
		
		void setup();
		void update();
		void draw();
        void exit();
    
        void touchDown(ofTouchEventArgs & touch);
        void touchMoved(ofTouchEventArgs & touch);
        void touchUp(ofTouchEventArgs & touch);
        void touchDoubleTap(ofTouchEventArgs & touch);
        void touchCancelled(ofTouchEventArgs & touch);
	
        void lostFocus();
        void gotFocus();
        void gotMemoryWarning();
        void deviceOrientationChanged(int newOrientation);

        #ifdef _USE_LIVE_VIDEO
            ofVideoGrabber vidGrabber;
        #endif
		ofVideoPlayer vidPlayer;

		ofTexture tex;

        ofxCvColorImage	colorImg;

        ofxCvGrayscaleImage grayImage;
		ofxCvGrayscaleImage grayBg;
		ofxCvGrayscaleImage grayDiff;
		
        float capW;
        float capH;

        ofxCvContourFinder contourFinder;

		int threshold;
		bool bLearnBakground;

};
