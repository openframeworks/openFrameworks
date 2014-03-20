#pragma once

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofBaseTypes.h"
#include "ofPixels.h"

#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	#include "videoInput.h"
#endif


class ofDirectShowGrabber : public ofBaseVideoGrabber{

	public :

		ofDirectShowGrabber();
		virtual ~ofDirectShowGrabber();

		vector<ofVideoDevice>	listDevices();
		bool					initGrabber(int w, int h);
		void					update();
		bool					isFrameNew();
		
		bool					setPixelFormat(ofPixelFormat pixelFormat);
		ofPixelFormat			getPixelFormat();		

		unsigned char		* 	getPixels();
		ofPixelsRef				getPixelsRef();
		
		void					close();
		void					clearMemory();

		void					videoSettings();
		
		float					getWidth();
		float					getHeight();

		void					setVerbose(bool bTalkToMe);
		void					setDeviceID(int _deviceID);
		void					setDesiredFrameRate(int framerate);


		

	protected:
	
		bool					bChooseDevice;
		int						deviceID;
		bool 					bVerbose;
		bool 					bGrabberInited;
	    ofPixels		 		pixels;
		int						attemptFramerate;
		bool 					bIsFrameNew;	
		
		int						width, height;	
		//--------------------------------- directshow
		#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
			int 					device;
			videoInput 				VI;
			bool 					bDoWeNeedToResize;
		#endif	
};

