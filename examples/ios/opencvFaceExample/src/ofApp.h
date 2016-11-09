#pragma once


#include "ofxiOS.h"
#include "ofxOpenCv.h"

//UNCOMMENT TO USE CAMERA. CAMERA ONLY WORKS ON DEVICE NOT SIMULATOR
//#define USE_CAMERA

class ofApp : public ofxiOSApp{
	
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

		ofImage img;
				
		#ifdef USE_CAMERA
			ofVideoGrabber grabber;
		#endif
		
		ofTexture tex;
		unsigned char * pix;
		
		ofxCvColorImage colorCv;
		ofxCvColorImage colorCvSmall;
		ofxCvGrayscaleImage grayCv;
		ofxCvHaarFinder finder;
		
		ofImage colorImg;
		
		vector<ofxCvBlob> faces;
};
