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
  string serial_id;
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
	ofPixelFormat	getPixelFormat() const;
	
	void videoSettings(){};//TODO: what is this??

	vector<ofVideoDevice> listDevices() const;
	void setDeviceID(int id);
	void setDesiredFrameRate(int framerate);
	bool setup(int w, int h);

	void 			update();
	bool 			isFrameNew() const;

	ofPixels&		getPixels();
	const ofPixels &		getPixels() const;
	ofTexture * getTexturePtr();

	float 			getHeight() const;
	float 			getWidth() const;
	void 			close();

	void			setVerbose(bool bVerbose);
	bool			isInitialized() const;

	ofGstVideoUtils *	getGstVideoUtils();
private:
	ofGstVideoFormat&	selectFormat(int w, int h, int desired_framerate, ofPixelFormat desiredPixelFormat);

	mutable ofGstCamData camData;
	bool				bIsCamera;
	int					attemptFramerate;
	int					deviceID;
	ofPixelFormat		internalPixelFormat;
	ofGstVideoUtils		videoUtils;
};
