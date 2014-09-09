#ifndef _OF_IOS_VIDEO_GRABBER
#define _OF_IOS_VIDEO_GRABBER


class AVFoundationVideoGrabber;

#include "ofVideoGrabber.h"

class ofxiOSVideoGrabber : public ofBaseVideoGrabber {
	
	public :
	
	ofxiOSVideoGrabber();
	~ofxiOSVideoGrabber();
	
	//needs implementing
	vector <ofVideoDevice> listDevices() const;
	void getDeviceList() const {};
	bool initGrabber(int w, int h);
    bool isInitialized() const;
	
	bool			setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat 	getPixelFormat() const;

	bool isFrameNew() const;
	
	unsigned char * getPixels();
	ofPixels&		getPixelsRef();
	const ofPixels& getPixelsRef() const;

	void close();	
	
	float getHeight() const;
	float getWidth() const;
	
	void update();
	
	void setDeviceID(int _deviceID);
	
	void setDesiredFrameRate(int framerate);
	
	//should implement!
	/*void setVerbose(bool bTalkToMe);
	
	void videoSettings();*/
	
protected:
	shared_ptr<AVFoundationVideoGrabber> grabber;
};

#endif

#define ofxiPhoneVideoGrabber ofxiOSVideoGrabber
