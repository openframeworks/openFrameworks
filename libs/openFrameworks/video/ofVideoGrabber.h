#ifndef _OF_VIDEO_GRABBER
#define _OF_VIDEO_GRABBER

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofGraphics.h"
#include "ofTypes.h"
#include "ofBaseVideoGrabber.h"

#ifdef OF_VIDEO_CAPTURE_QUICKTIME
	#include "ofQuickTimeGrabber.h"
	#define OF_VID_GRABBER_TYPE ofQuickTimeGrabber()
#endif

#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	#include "ofDirectShowGrabber.h"
	#define OF_VID_GRABBER_TYPE ofDirectShowGrabber()
#endif

#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	#include "ofGStreamerGrabber.h"
	#define OF_VID_GRABBER_TYPE ofGStreamerGrabber()	
#endif

#ifdef OF_VIDEO_CAPTURE_UNICAP
	#include "ofUnicapGrabber.h"
	#define OF_VID_GRABBER_TYPE ofUnicapGrabber()		
#endif

class ofVideoGrabber : public ofBaseVideo{

	public :

		ofVideoGrabber();
		virtual ~ofVideoGrabber();
		
		bool					setGrabber(ofBaseVideoGrabber * newGrabber);
		ofBaseVideoGrabber *	getGrabber();

		void				listDevices();
		bool				isFrameNew();
		void				update();
		void				grabFrame();
		void				close();
		bool				initGrabber(int w, int h, bool bTexture = true);
		
		void				videoSettings();
		unsigned char 	*	getPixels();
		ofTexture &			getTextureReference();
		void				setVerbose(bool bTalkToMe);
		void				setDeviceID(int _deviceID);
		void				setDesiredFrameRate(int framerate);
		void				setUseTexture(bool bUse);
		void				draw(float x, float y, float w, float h);
		void				draw(float x, float y);

		//the anchor is the point the image is drawn around.
		//this can be useful if you want to rotate an image around a particular point.
        void				setAnchorPercent(float xPct, float yPct);	//set the anchor as a percentage of the image width/height ( 0.0-1.0 range )
        void				setAnchorPoint(int x, int y);				//set the anchor point in pixels
        void				resetAnchor();								//resets the anchor to (0, 0)

		float				getHeight();
		float				getWidth();

		//this is kept as legacy to support people accessing width and height directly. 
		int					height;
		int					width;

	protected:
		
		ofTexture tex;
		bool bUseTexture;
		ofBaseVideoGrabber		* grabber;
		int RequestedDeviceID;

};




#endif

