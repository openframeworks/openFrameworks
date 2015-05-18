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

#ifdef OF_VIDEO_CAPTURE_EMSCRIPTEN
	#include "ofxEmscriptenVideoGrabber.h"
	#define OF_VID_GRABBER_TYPE ofxEmscriptenVideoGrabber
#endif

class ofVideoGrabber : public ofBaseVideoGrabber,public ofBaseVideoDraws{

	public :

		ofVideoGrabber();
		virtual ~ofVideoGrabber();

		vector<ofVideoDevice> listDevices() const;
		bool				isFrameNew() const;
		void				update();
		void				close();	
		bool				setup(int w, int h){return setup(w,h,true);}
		bool				setup(int w, int h, bool bTexture);
		OF_DEPRECATED_MSG("Use setup instead",bool initGrabber(int w, int h){return setup(w,h);})
		OF_DEPRECATED_MSG("Use setup instead",bool initGrabber(int w, int h, bool bTexture));
		
		bool				setPixelFormat(ofPixelFormat pixelFormat);
		ofPixelFormat 		getPixelFormat() const;
		
		void				videoSettings();
		ofPixels& 			getPixels();
		const ofPixels&		getPixels() const;
        OF_DEPRECATED_MSG("Use getPixels() instead", ofPixels&	getPixelsRef());
        OF_DEPRECATED_MSG("Use getPixels() instead", const ofPixels&  getPixelsRef() const);
		ofTexture &			getTexture();
		const ofTexture &	getTexture() const;
		OF_DEPRECATED_MSG("Use getTexture",ofTexture &			getTextureReference());
		OF_DEPRECATED_MSG("Use getTexture",const ofTexture &	getTextureReference() const);
		vector<ofTexture> & getTexturePlanes();
		const vector<ofTexture> & getTexturePlanes() const;
		void				setVerbose(bool bTalkToMe);
		void				setDeviceID(int _deviceID);
		void				setDesiredFrameRate(int framerate);
		void				setUseTexture(bool bUse);
		bool 				isUsingTexture() const;
		void				draw(float x, float y, float w, float h) const;
		void				draw(float x, float y) const;
		using ofBaseDraws::draw;

		void 				bind() const;
		void 				unbind() const;

		//the anchor is the point the image is drawn around.
		//this can be useful if you want to rotate an image around a particular point.
        void				setAnchorPercent(float xPct, float yPct);	//set the anchor as a percentage of the image width/height ( 0.0-1.0 range )
        void				setAnchorPoint(float x, float y);				//set the anchor point in pixels
        void				resetAnchor();								//resets the anchor to (0, 0)

		float				getHeight() const;
		float				getWidth() const;

		bool				isInitialized() const;

		void					setGrabber(shared_ptr<ofBaseVideoGrabber> newGrabber);
		shared_ptr<ofBaseVideoGrabber> getGrabber();
		const shared_ptr<ofBaseVideoGrabber> getGrabber() const;

		template<typename GrabberType>
		shared_ptr<GrabberType> getGrabber(){
			return dynamic_pointer_cast<GrabberType>(getGrabber());
		}

		template<typename GrabberType>
		const shared_ptr<GrabberType> getGrabber() const{
			return dynamic_pointer_cast<GrabberType>(getGrabber());
		}

	private:
		
		vector<ofTexture> tex;
		bool bUseTexture;
		shared_ptr<ofBaseVideoGrabber> grabber;
		int requestedDeviceID;

		mutable ofPixelFormat internalPixelFormat;
		int desiredFramerate;
};



