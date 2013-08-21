#pragma once

#include "ofConstants.h"
#ifndef TARGET_ANDROID
#include "ofConstants.h"
#include "ofBaseTypes.h"
#include "ofPixels.h"
#include "ofTypes.h"
#include "ofEvents.h"
#include "ofThread.h"

#define GST_DISABLE_DEPRECATED
#include <gst/gst.h>
#include <gst/gstpad.h>

class ofGstAppSink;
typedef struct _GstElement GstElement;
typedef struct _GstBuffer GstBuffer;
typedef struct _GstMessage GstMessage;

//-------------------------------------------------
//----------------------------------------- ofGstUtils
//-------------------------------------------------

class ofGstUtils{
public:
	ofGstUtils();
	virtual ~ofGstUtils();

	bool 	setPipelineWithSink(string pipeline, string sinkname="sink", bool isStream=false);
	bool 	setPipelineWithSink(GstElement * pipeline, GstElement * sink, bool isStream=false);

	void 	play();
	void 	stop();
	void 	setPaused(bool bPause);
	bool 	isPaused() const {return bPaused;}
	bool 	isLoaded() const {return bLoaded;}
	bool 	isPlaying() const {return bPlaying;}

	float	getPosition() const;
	float 	getSpeed() const;
	float 	getDuration() const;
	int64_t  getDurationNanos() const;
	bool  	getIsMovieDone() const;

	void 	setPosition(float pct);
	void 	setVolume(float volume);
	void 	setLoopState(ofLoopType state);
	ofLoopType	getLoopState() const {return loopMode;}
	void 	setSpeed(float speed);

	void 	setFrameByFrame(bool bFrameByFrame);
	bool	isFrameByFrame() const;

	GstElement 	* getPipeline() const;
	GstElement 	* getSink() const;
	GstElement 	* getGstElementByName(const string & name) const;
	unsigned long getMinLatencyNanos() const;
	unsigned long getMaxLatencyNanos() const;

	virtual void close();

	void setSinkListener(ofGstAppSink * appsink);

	// callbacks to get called from gstreamer
#if GST_VERSION_MAJOR==0
	virtual GstFlowReturn preroll_cb(GstBuffer * buffer);
	virtual GstFlowReturn buffer_cb(GstBuffer * buffer);
#else
	virtual GstFlowReturn preroll_cb(GstSample * buffer);
	virtual GstFlowReturn buffer_cb(GstSample * buffer);
#endif
	virtual void 		  eos_cb();

	static void startGstMainLoop();
protected:
	ofGstAppSink * 		appsink;
	bool				isStream;

private:
	static bool			busFunction(GstBus * bus, GstMessage * message, ofGstUtils * app);
	bool				gstHandleMessage(GstBus * bus, GstMessage * message);
	bool				startPipeline();

	bool 				bPlaying;
	bool 				bPaused;
	bool				bIsMovieDone;
	bool 				bLoaded;
	bool 				bFrameByFrame;
	ofLoopType			loopMode;

	GstElement  *		gstSink;
	GstElement 	*		gstPipeline;

	float				speed;
	mutable gint64      durationNanos;
	bool				isAppSink;

	class ofGstMainLoopThread: public ofThread{
		GMainLoop *main_loop;
	public:
		void start(){
			startThread();
		}
		void threadedFunction(){
			main_loop = g_main_loop_new (NULL, FALSE);
			g_main_loop_run (main_loop);
		}
	};

	static ofGstMainLoopThread * mainLoop;
	GstBus * bus;
};





//-------------------------------------------------
//----------------------------------------- videoUtils
//-------------------------------------------------

class ofGstVideoUtils: public ofBaseVideo, public ofGstUtils{
public:

	ofGstVideoUtils();
	virtual ~ofGstVideoUtils();

	bool 			setPipeline(string pipeline, int bpp=24, bool isStream=false, int w=-1, int h=-1);

	bool 			allocate(int w, int h, int bpp);

	bool 			isFrameNew() const;
	unsigned char * getPixels();
	ofPixelsRef		getPixelsRef();
	void 			update();

	float 			getHeight() const;
	float 			getWidth() const;

	void 			close();

	// this events happen in a different thread
	// do not use them for opengl stuff
	ofEvent<ofPixels> prerollEvent;
	ofEvent<ofPixels> bufferEvent;
	ofEvent<ofEventArgs> eosEvent;

protected:
#if GST_VERSION_MAJOR==0
	GstFlowReturn preroll_cb(GstBuffer * buffer);
	GstFlowReturn buffer_cb(GstBuffer * buffer);
#else
	GstFlowReturn preroll_cb(GstSample * buffer);
	GstFlowReturn buffer_cb(GstSample * buffer);
#endif
	void			eos_cb();


	ofPixels		pixels;				// 24 bit: rgb
	ofPixels		backPixels;
	ofPixels		eventPixels;
private:
	bool			bIsFrameNew;			// if we are new
	bool			bHavePixelsChanged;
	bool			bBackPixelsChanged;
	ofMutex			mutex;
#if GST_VERSION_MAJOR==0
	GstBuffer * 	buffer, *prevBuffer;
#else
	GstSample * 	buffer, *prevBuffer;
	GstMapInfo mapinfo;
#endif
};


//-------------------------------------------------
//----------------------------------------- appsink listener
//-------------------------------------------------

class ofGstAppSink{
public:
	virtual ~ofGstAppSink(){}
#if GST_VERSION_MAJOR==0
	virtual GstFlowReturn on_preroll(GstBuffer * buffer){
		return GST_FLOW_OK;
	}
	virtual GstFlowReturn on_buffer(GstBuffer * buffer){
		return GST_FLOW_OK;
	}
#else
	virtual GstFlowReturn on_preroll(GstSample * buffer){
		return GST_FLOW_OK;
	}
	virtual GstFlowReturn on_buffer(GstSample * buffer){
		return GST_FLOW_OK;
	}
#endif
	virtual void			on_eos(){}

	// return true to set the message as attended so upstream doesn't try to process it
	virtual bool on_message(GstMessage* msg){return false;};

	// pings when enough data has arrived to be able to get sink properties
	virtual void on_stream_prepared(){};
};

#endif

