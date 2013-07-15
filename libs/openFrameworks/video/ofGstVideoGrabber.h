#pragma once

#include "ofGstUtils.h"
#include "ofTypes.h"


struct ofGstFramerate{
  int numerator;
  int denominator;
};

struct ofGstVideoFormat{
  string mimetype;
  string format_name;
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
	bool setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat	getPixelFormat();
	
	void videoSettings(){};//TODO: what is this??

	vector<ofVideoDevice> listDevices();
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
