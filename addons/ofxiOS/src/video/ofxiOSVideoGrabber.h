

#pragma once

#include "ofVideoBaseTypes.h"
#include "ofPixels.h"

class AVFoundationVideoGrabber;

class ofxiOSVideoGrabber : public ofBaseVideoGrabber {
	
public:
	
	ofxiOSVideoGrabber();
	~ofxiOSVideoGrabber();
	
	//---------------------------------------
	// inherited from ofBaseVideoGrabber
	//---------------------------------------
	
	std::vector<ofVideoDevice> listDevices() const;
	bool setup(int w, int h);

	float getHeight() const;
	float getWidth() const;
	
	ofTexture * getTexturePtr();
	
	void setVerbose(bool bTalkToMe);
	void setDeviceID(int deviceID);
	void setDesiredFrameRate(int framerate);
	void videoSettings();
	
	//---------------------------------------
	// inherited from ofBaseVideo
	//---------------------------------------
	
	bool isFrameNew() const;
	void close();
	bool isInitialized() const;
	
	bool setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat getPixelFormat() const;
	
	//---------------------------------------
	// inherited from ofBaseHasPixels
	//---------------------------------------
	
	ofPixels & getPixels();
	const ofPixels & getPixels() const;
	
	//---------------------------------------
	// inherited from ofBaseUpdates
	//---------------------------------------
	
	void update();
	
	//---------------------------------------
	// deprecated.
	//---------------------------------------
	
	[[deprecated("use setup(int w, int h)")]]
	bool initGrabber(int w, int h);
	[[deprecated("use listDevices()")]]
	void getDeviceList() const;
	[[deprecated("use getPixels()")]]
	ofPixels&	getPixelsRef();
	[[deprecated("use getPixels()")]]
	const ofPixels&  getPixelsRef() const;
	
protected:
	
	std::shared_ptr<AVFoundationVideoGrabber> grabber;
	
	ofPixels pixels;
	
};

#define ofxiPhoneVideoGrabber ofxiOSVideoGrabber
