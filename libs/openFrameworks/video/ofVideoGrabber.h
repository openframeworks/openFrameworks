#pragma once

#include "ofTexture.h"
#include "ofVideoBaseTypes.h"

template<typename T>
class ofPixels_;
typedef ofPixels_<unsigned char> ofPixels;
typedef ofPixels_<float> ofFloatPixels;
typedef ofPixels_<unsigned short> ofShortPixels;
typedef ofPixels& ofPixelsRef;


class ofVideoGrabber : public ofBaseVideoGrabber,public ofBaseVideoDraws{

	public :

		ofVideoGrabber();
		virtual ~ofVideoGrabber();

		std::vector<ofVideoDevice> listDevices() const;
		bool				isFrameNew() const;
		void				update();
		void				close();	
		bool				setup(int w, int h){return setup(w,h,bUseTexture);}
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
		std::vector<ofTexture> & getTexturePlanes();
		const std::vector<ofTexture> & getTexturePlanes() const;
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

		void					setGrabber(std::shared_ptr<ofBaseVideoGrabber> newGrabber);
		std::shared_ptr<ofBaseVideoGrabber> getGrabber();
		const std::shared_ptr<ofBaseVideoGrabber> getGrabber() const;

		template<typename GrabberType>
		std::shared_ptr<GrabberType> getGrabber(){
			return std::dynamic_pointer_cast<GrabberType>(getGrabber());
		}

		template<typename GrabberType>
		const std::shared_ptr<GrabberType> getGrabber() const{
			return std::dynamic_pointer_cast<GrabberType>(getGrabber());
		}

	private:
		
		std::vector<ofTexture> tex;
		bool bUseTexture;
		std::shared_ptr<ofBaseVideoGrabber> grabber;
		int requestedDeviceID;

		mutable ofPixelFormat internalPixelFormat;
		int desiredFramerate;
};



