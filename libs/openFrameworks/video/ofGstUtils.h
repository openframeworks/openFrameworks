#pragma once

#include <gst/gst.h>
#include "ofConstants.h"
#include "ofBaseTypes.h"
#include "ofPixels.h"
#include "ofTypes.h"

struct ofGstVideoData{
	GMainLoop 		*	loop;
	GstElement 		*	pipeline;
	ofPixels			pixels;				// 24 bit: rgb
	unsigned			totalsize;
	ofMutex			 	buffer_mutex;
	bool				bHavePixelsChanged;

	guint64				durationNanos;
	guint64				nFrames;
	int					pipelineState;
	float				speed;

	guint64				lastFrame;
};

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

class ofGstUtils: public ofBaseVideoPlayer, public ofBaseVideoGrabber {
public:
	ofGstUtils();
	virtual ~ofGstUtils();

	bool loadMovie(string uri);

	void listDevices();
	void setDeviceID(unsigned id);
	void setDesiredFrameRate(int framerate);
	bool initGrabber(int w, int h);

	bool setPipeline(string pipeline, int bpp=24, bool isStream=false, int w=-1, int h=-1);
	bool setPipelineWithSink(string pipeline);

	bool isFrameNew();
	unsigned char * getPixels();
	ofPixels getOFPixels();
	ofPixels getOFPixels() const;
	void update();

	void play();
	void stop(){setPaused(true);}
	void setPaused(bool bPause);
	bool isPaused(){return bPaused;}
	bool isLoaded(){return bLoaded;}
	bool isPlaying(){return bPlaying;}

	int	getCurrentFrame();
	int	getTotalNumFrames();

	void firstFrame();
	void nextFrame();
	void previousFrame();

	float getHeight();
	float getWidth();

	float getPosition();
	float getSpeed();
	float getDuration();
	bool  getIsMovieDone();

	void setPosition(float pct);
	void setVolume(int volume);
	void setLoopState(ofLoopType state);
	void setSpeed(float speed);
	void setFrame(int frame);  // frame 0 = first frame...

	void setFrameByFrame(bool bFrameByFrame);

	void close();

	GstElement 	* getPipeline();

protected:
	void 				gstHandleMessage();
	bool 				allocate(int width=0, int height=0, int bpp=24);
	bool				startPipeline(int width=0, int height=0, int bpp=24);
	ofGstVideoFormat&	selectFormat(int w, int h, int desired_framerate);

	bool 				bStarted;
	bool 				bPlaying;
	bool 				bPaused;
	bool 				bIsFrameNew;			// if we are new
	bool				bIsMovieDone;
	int					loopMode;

	ofGstVideoData 		gstData;
	ofGstCamData		camData;

	bool				bIsStream;
	bool				bIsCamera;
	bool				bIsCustomWithSink;

	GstElement  *		gstSink;
	GstElement 	*		gstPipeline;


	bool				posChangingPaused;

	int					attemptFramerate;
	bool 				bLoaded;


	// common with gstdata
	ofPixels			pixels;				// 24 bit: rgb
	bool				bHavePixelsChanged;

	gint64				durationNanos;
	guint64				nFrames;
	int					pipelineState;
	float				speed;

	bool 				bFrameByFrame;

	int fps_n;
	int fps_d;

	//camera only
	int					deviceID;

};

