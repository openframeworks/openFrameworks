#pragma once

#include "ofGstUtils.h"

struct ofGstFramerate{
  int numerator;
  int denominator;
};

struct ofGstVideoFormat{
  string mimetype;
  unsigned int 	 fourcc;
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

class ofGstVideoGrabber: public ofBaseVideoGrabber{
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

	void 			update();
	bool 			isFrameNew();

	unsigned char * getPixels();
	ofPixelsRef		getPixelsRef();

	float 			getHeight();
	float 			getWidth();
	void 			close();

	void			setVerbose(bool bVerbose);

	ofGstVideoUtils *	getGstVideoUtils();
private:
	ofGstVideoFormat&	selectFormat(int w, int h, int desired_framerate);

	ofGstCamData		camData;
	bool				bIsCamera;
	int					attemptFramerate;
	int					deviceID;
	ofPixelFormat		internalPixelFormat;
	ofGstVideoUtils		videoUtils;
};
