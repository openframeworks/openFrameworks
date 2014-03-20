#pragma once

#include "ofConstants.h"

#if !defined(TARGET_LINUX) && !defined(MAC_OS_X_VERSION_10_7)
	#include "ofQtUtils.h"
	#include "ofTexture.h"
	#include "ofBaseTypes.h"
	#include "ofPixels.h"

// todo:
// 		QT - callback, via SGSetDataProc - couldn't get this to work yet
// 		image decompress options ala mung...


class ofQuickTimeGrabber : public ofBaseVideoGrabber{

	public :

		ofQuickTimeGrabber();
		virtual ~ofQuickTimeGrabber();

        vector<ofVideoDevice>	listDevices();
		bool					initGrabber(int w, int h);
		void					update();
		bool					isFrameNew();

		bool					setPixelFormat(ofPixelFormat pixelFormat);
		ofPixelFormat			getPixelFormat();

		unsigned char			* getPixels();
		ofPixelsRef		 		getPixelsRef();
		
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
	    ofPixels				pixels;
		int						attemptFramerate;
		bool 					bIsFrameNew;	
		
		//int						width, height;

		unsigned char *			offscreenGWorldPixels;	// 32 bit: argb (qt k32ARGBPixelFormat)
		//int						w,h;
		bool					bHavePixelsChanged;
		GWorldPtr				videogworld;
		SeqGrabComponent		gSeqGrabber;
		SGChannel				gVideoChannel;
		Rect					videoRect;
		bool					bSgInited;
		string					deviceName;
		SGGrabCompleteBottleUPP	myGrabCompleteProc;
		
		bool					qtInitSeqGrabber();
		bool					qtCloseSeqGrabber();
		bool					qtSelectDevice(int deviceNumber, bool didWeChooseADevice);

		bool					saveSettings();
		bool					loadSettings();
		

};


#endif
