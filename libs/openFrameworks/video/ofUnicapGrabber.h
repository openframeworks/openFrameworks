#pragma once

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofGraphics.h"
#include "ofTypes.h"
#include "ofBaseVideoGrabber.h"

#ifdef OF_VIDEO_CAPTURE_UNICAP
	#include "ofUCUtils.h"
#endif

class ofUnicapGrabber : public ofBaseVideoGrabber{

	public :

		ofUnicapGrabber();
		virtual ~ofUnicapGrabber();

		void					listDevices();
		bool					initGrabber(int w, int h);
		void					grabFrame();
		bool					isFrameNew();

		unsigned char			* getPixels();
		
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
		int						attemptFramerate;
		bool 					bIsFrameNew;	
	    unsigned char * 		pixels;
		int						width, height;	

		//--------------------------------- linux unicap
		#ifdef OF_VIDEO_CAPTURE_UNICAP
			ofUCUtils	ucGrabber;
		#endif
};

