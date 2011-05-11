#pragma once

#include "ofConstants.h"
#ifndef TARGET_ANDROID
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include "ofConstants.h"
#include "ofBaseTypes.h"
#include "ofPixels.h"
#include "ofTypes.h"
#include "ofEvents.h"

class ofGstAppSink;

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
	void 	stop(){setPaused(true);}
	void 	setPaused(bool bPause);
	bool 	isPaused(){return bPaused;}
	bool 	isLoaded(){return bLoaded;}
	bool 	isPlaying(){return bPlaying;}

	float	getPosition();
	float 	getSpeed();
	float 	getDuration();
	guint64 getDurationNanos();
	bool  	getIsMovieDone();

	void 	setPosition(float pct);
	void 	setVolume(int volume);
	void 	setLoopState(ofLoopType state);
	int		getLoopState(){return loopMode;}
	void 	setSpeed(float speed);

	void 	setFrameByFrame(bool bFrameByFrame);
	bool	isFrameByFrame();

	GstElement 	* getPipeline();
	GstElement 	* getSink();

	virtual void close();

	void setSinkListener(ofGstAppSink * appsink);

protected:
	// callbacks to get called from gstreamer
	virtual GstFlowReturn preroll_cb(GstBuffer * buffer);
	virtual GstFlowReturn buffer_cb(GstBuffer * buffer);
	virtual void 		  eos_cb();

private:
	void 				gstHandleMessage();
	void				update(ofEventArgs & args);
	bool				startPipeline();

	bool 				bPlaying;
	bool 				bPaused;
	bool				bIsMovieDone;
	bool 				bLoaded;
	bool 				bFrameByFrame;
	ofLoopType			loopMode;

	GstElement  *		gstSink;
	GstElement 	*		gstPipeline;
	ofGstAppSink * 		appsink;

	bool				posChangingPaused;
	int					pipelineState;
	float				speed;
	gint64				durationNanos;
	bool				isAppSink;
	bool				isStream;

	// the gst callbacks need to be friended to be able to call us
	friend GstFlowReturn on_new_buffer_from_source (GstAppSink * elt, void * data);
	friend GstFlowReturn on_new_preroll_from_source (GstAppSink * elt, void * data);
	friend void on_eos_from_source (GstAppSink * elt, void * data);

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

	bool 			isFrameNew();
	unsigned char * getPixels();
	ofPixelsRef		getPixelsRef();
	void 			update();

	float 			getHeight();
	float 			getWidth();

	void 			close();

	// this events happen in a different thread
	// do not use them for opengl stuff
	ofEvent<ofPixels> prerollEvent;
	ofEvent<ofPixels> bufferEvent;
	ofEvent<ofEventArgs> eosEvent;

protected:
	GstFlowReturn 	preroll_cb(GstBuffer * buffer);
	GstFlowReturn 	buffer_cb(GstBuffer * buffer);
	void			eos_cb();


	ofPixels		pixels;				// 24 bit: rgb
	ofPixels		backPixels;
private:
	bool			bIsFrameNew;			// if we are new
	bool			bHavePixelsChanged;
	bool			bBackPixelsChanged;
	ofMutex			mutex;
};


//-------------------------------------------------
//----------------------------------------- appsink listener
//-------------------------------------------------

class ofGstAppSink{
public:
	virtual GstFlowReturn	on_preroll(GstBuffer * buffer){
		return GST_FLOW_OK;
	}
	virtual GstFlowReturn	on_buffer(GstBuffer * buffer){
		return GST_FLOW_OK;
	}
	virtual void			on_eos(){}

	// return true to set the message as attended so upstream doesn't try to process it
	virtual bool on_message(GstMessage* msg){return false;};

	// pings when enough data has arrived to be able to get sink properties
	virtual void on_stream_prepared(){};
};

#endif

