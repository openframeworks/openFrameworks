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

		vector<ofVideoDevice>	listDevices() const;
		bool					setup(int w, int h);
		void					update();
		bool					isFrameNew() const;
		bool					isInitialized() const;

		bool					setPixelFormat(ofPixelFormat pixelFormat);
		ofPixelFormat			getPixelFormat() const;		

		ofPixels&				getPixels();
		const ofPixels&			getPixels() const;
		
		void					close();
		void					clearMemory();

		void					videoSettings();
		
		float					getWidth() const;
		float					getHeight() const;

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

