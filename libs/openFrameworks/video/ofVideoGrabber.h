#pragma once

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofBaseTypes.h"
#include "ofPixels.h"
#include "ofTypes.h"


#ifdef OF_VIDEO_CAPTURE_IOS
	#include "ofxiOSVideoGrabber.h"
	#define OF_VID_GRABBER_TYPE ofxiOSVideoGrabber
#endif

#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	#include "ofQuickTimeGrabber.h"
	#define OF_VID_GRABBER_TYPE ofQuickTimeGrabber
#endif

#ifdef OF_VIDEO_CAPTURE_QTKIT
	#include "ofQTKitGrabber.h"
	#define OF_VID_GRABBER_TYPE ofQTKitGrabber
#endif

#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	#include "ofDirectShowGrabber.h"
	#define OF_VID_GRABBER_TYPE ofDirectShowGrabber
#endif

#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	#include "ofGstVideoGrabber.h"
	#define OF_VID_GRABBER_TYPE ofGstVideoGrabber
#endif

#ifdef OF_VIDEO_CAPTURE_ANDROID
	#include "ofxAndroidVideoGrabber.h"
	#define OF_VID_GRABBER_TYPE ofxAndroidVideoGrabber
#endif

class ofVideoGrabber : public ofBaseVideoGrabber,public ofBaseVideoDraws{

	public :

		ofVideoGrabber();
		virtual ~ofVideoGrabber();
		
		void					setGrabber(ofPtr<ofBaseVideoGrabber> newGrabber);
		ofPtr<ofBaseVideoGrabber> getGrabber();

		vector<ofVideoDevice> listDevices();
		bool				isFrameNew();
		void				update();
		void				close();	
		bool				initGrabber(int w, int h){return initGrabber(w,h,true);}
		bool				initGrabber(int w, int h, bool bTexture);
		
		bool				setPixelFormat(ofPixelFormat pixelFormat);
		ofPixelFormat 		getPixelFormat();
		
		void				videoSettings();
		unsigned char 	*	getPixels();
		ofPixelsRef			getPixelsRef();
		ofTexture &			getTextureReference();
		void				setVerbose(bool bTalkToMe);
		void				setDeviceID(int _deviceID);
		void				setDesiredFrameRate(int framerate);
		void				setUseTexture(bool bUse);
		void				draw(float x, float y, float w, float h);
		void				draw(float x, float y);
		using ofBaseDraws::draw;

		//the anchor is the point the image is drawn around.
		//this can be useful if you want to rotate an image around a particular point.
        void				setAnchorPercent(float xPct, float yPct);	//set the anchor as a percentage of the image width/height ( 0.0-1.0 range )
        void				setAnchorPoint(float x, float y);				//set the anchor point in pixels
        void				resetAnchor();								//resets the anchor to (0, 0)

		float				getHeight();
		float				getWidth();

		bool				isInitialized();

		//this is kept as legacy to support people accessing width and height directly. 
		int					height;
		int					width;

	private:
		
		ofTexture tex;
		bool bUseTexture;
		bool bInitialized;
		ofPtr<ofBaseVideoGrabber> grabber;
		int RequestedDeviceID;
		
		bool grabberRunning; //this keeps track of whether the grabber opened sucessfully and is still open. //TODO: maybe expose this in a method? 
		
		ofPixelFormat internalPixelFormat;
		int desiredFramerate;
};



