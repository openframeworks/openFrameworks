#pragma once

#include "ofGstUtils.h"

struct ofGstFramerate{
  int numerator;
  int denominator;
};

struct ofGstVideoFormat{
  string mimetype;
  int    width;
  int    height;
  vector<ofGstFramerate> framerates;
  ofGstFramerate choosen_framerate;
};

struct ofGstDevice{
  string video_device;
  string gstreamer_src;
  string product_name;
  vector<ofGstVideoFormat> video_formats;
  int current_format;
};

struct ofGstCamData{
  vector<ofGstDevice> webcam_devices;
  bool bInited;
};

class ofGstVideoGrabber: public ofGstVideoUtils, public ofBaseVideoGrabber{
public:
	ofGstVideoGrabber();
	~ofGstVideoGrabber();

	/// needs to be called before initGrabber
	void setPixelFormat(ofPixelFormat pixelFormat);
	void videoSettings(){};//TODO: what is this??

	void listDevices();
	void setDeviceID(int id);
	void setDesiredFrameRate(int framerate);
	bool initGrabber(int w, int h);

	void 	update(){ ofGstVideoUtils::update(); }

	virtual bool isStream(){return false;}

	bool 			isFrameNew(){ return ofGstVideoUtils::isFrameNew();}
	unsigned char * getPixels(){ return ofGstVideoUtils::getPixels();}
	float 			getHeight(){ return ofGstVideoUtils::getHeight();}
	float 			getWidth(){ return ofGstVideoUtils::getWidth();}

	void 			close(){ ofGstUtils::close();}

	void			setVerbose(bool bVerbose);
private:
	bool				allocate(){return true;}
	ofGstVideoFormat&	selectFormat(int w, int h, int desired_framerate);

	ofGstCamData		camData;
	bool				bIsCamera;
	int					attemptFramerate;
	int					deviceID;
	ofPixelFormat		internalPixelFormat;
};
