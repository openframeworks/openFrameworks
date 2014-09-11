#ifndef _OF_IOS_VIDEO_GRABBER
#define _OF_IOS_VIDEO_GRABBER


class AVFoundationVideoGrabber;

#include "ofVideoGrabber.h"

class ofxiOSVideoGrabber : public ofBaseVideoGrabber {
	
	public :
	
	ofxiOSVideoGrabber();
	~ofxiOSVideoGrabber();
	
	//needs implementing
	vector <ofVideoDevice> listDevices();	
	void getDeviceList(){};
	bool initGrabber(int w, int h);
    bool isInitialized();
	
	bool			setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat 	getPixelFormat();	

	bool isFrameNew();
	
	unsigned char * getPixels();
	ofPixelsRef		getPixelsRef();
	
	void close();	
	
	float getHeight();
	float getWidth();
	
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
