#ifndef _OF_IOS_VIDEO_GRABBER
#define _OF_IOS_VIDEO_GRABBER


class AVFoundationVideoGrabber;

#include "ofVideoGrabber.h"

class ofxiOSVideoGrabber : public ofBaseVideoGrabber {
	
	public :
	
	ofxiOSVideoGrabber();
	
	//needs implementing
	vector <ofVideoDevice> listDevices() const;
	void getDeviceList() const {};
	bool initGrabber(int w, int h);
	
	bool			setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat 	getPixelFormat() const;

	bool isFrameNew() const;
	
	unsigned char * getPixels();
	ofPixelsRef		getPixelsRef();
	
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
	ofPtr<AVFoundationVideoGrabber> grabber;
};

#endif

#define ofxiPhoneVideoGrabber ofxiOSVideoGrabber
