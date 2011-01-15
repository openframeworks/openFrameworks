#ifndef _OF_IPHONE_VIDEO_GRABBER
#define _OF_IPHONE_VIDEO_GRABBER

#include "ofMain.h"

class AVFoundationVideoGrabber;

class ofiPhoneVideoGrabber : public ofBaseVideoGrabber {
	
	public :
	
	ofiPhoneVideoGrabber();
	virtual ~ofiPhoneVideoGrabber();
	
	//needs implementing
	void listDevices();	
	void getDeviceList(){};
	bool initGrabber(int w, int h);

	bool isFrameNew();
	
	unsigned char * getPixels();
	
	void close();	
	
	float getHeight();
	float getWidth();
	
	void update();
	
	void setDeviceID(int _deviceID);
	
	void setDesiredFrameRate(int framerate);
	
	void setPixelFormat(ofPixelFormat internalPixelFormat);
	//should implement!
	/*void setVerbose(bool bTalkToMe);
	
	
	void videoSettings();*/
	
protected:
	AVFoundationVideoGrabber * grabber;
};

#endif