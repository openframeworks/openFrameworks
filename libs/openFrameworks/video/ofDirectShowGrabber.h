#pragma once

// MARK: OF_VIDEO_CAPTURE_DIRECTSHOW
#include "ofConstants.h"
#include "ofTexture.h"
#include "ofVideoBaseTypes.h"
#include "ofPixels.h" // MARK: ofPixels pixels

// comment out this following line, if you'd like to use the
// quicktime capture interface on windows
// if not, we default to videoInput library for
// direct show capture...

#define OF_SWITCH_TO_DSHOW_FOR_WIN_VIDCAP

#ifdef OF_SWITCH_TO_DSHOW_FOR_WIN_VIDCAP
	#define OF_VIDEO_CAPTURE_DIRECTSHOW
#else
	#define OF_VIDEO_CAPTURE_QUICKTIME
#endif


#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	#include <videoInput.h>
#endif


class ofDirectShowGrabber : public ofBaseVideoGrabber{

	public :

		ofDirectShowGrabber();
		virtual ~ofDirectShowGrabber();

		/// \brief Sets the preffered DirectShow media subtype - MJPG / H264 can provide faster framerates 
		///
		/// /use formats like: MEDIASUBTYPE_H264, MEDIASUBTYPE_MJPG, MEDIASUBTYPE_Y800 etc, default is: MEDIASUBTYPE_RGB24
		/// /use -1 to revert to the default behavior 
		///
		static void setPreferredFormat(int aPreferredFormat); 		

		std::vector<ofVideoDevice>	listDevices() const;
		bool					setup(int w, int h);
		void					update();
		bool					isFrameNew() const;
		bool					isInitialized() const;

		bool					setPixelFormat(ofPixelFormat pixelFormat);
		ofPixelFormat			getPixelFormat() const;		

		ofPixels &				getPixels();
		const ofPixels &		getPixels() const;
		
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
	    ofPixels pixels;
		int						attemptFramerate;
		bool 					bIsFrameNew;	
		
		int						width, height;	
		static int				preferredFormat; 
		//--------------------------------- directshow
		#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
			int 					device;
			videoInput 				VI;
			bool 					bDoWeNeedToResize;
		#endif	
};

