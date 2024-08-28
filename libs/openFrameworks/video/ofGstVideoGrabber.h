#pragma once

#include "ofGstUtils.h"


struct ofGstFramerate{
  int numerator;
  int denominator;
};

struct ofGstVideoFormat{
  std::string mimetype;
  std::string format_name;
  int    width;
  int    height;
  std::vector<ofGstFramerate> framerates;
  ofGstFramerate choosen_framerate;
};

struct ofGstDevice{
  std::string video_device;
  std::string gstreamer_src;
  std::string product_name;
  std::string serial_id;
  std::vector<ofGstVideoFormat> video_formats;
  int current_format;
};

struct ofGstCamData{
    std::vector<ofGstDevice> webcam_devices;
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

	std::vector<ofVideoDevice> listDevices() const;
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
