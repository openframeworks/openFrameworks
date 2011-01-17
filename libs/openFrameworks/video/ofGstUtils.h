#pragma once

#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include "ofConstants.h"
#include "ofBaseTypes.h"
#include "ofPixels.h"
#include "ofTypes.h"
#include "ofEvents.h"



//-------------------------------------------------
//----------------------------------------- ofGstUtils
//-------------------------------------------------

class ofGstUtils{
public:
	ofGstUtils();
	virtual ~ofGstUtils();

	bool 	setPipelineWithSink(string pipeline, string sinkname="sink");
	bool 	setPipelineWithSink(GstElement * pipeline, GstElement * sink);

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

	/// every inheriting class needs to implement this in order to signal
	/// if is a stream or not
	virtual bool isStream()=0;
	virtual void update()=0;
	virtual void close();

protected:
	/// implement this to get gstreamer callbacks and bus messages
	virtual bool allocate(bool gotData)=0;
	virtual GstFlowReturn on_preroll(GstBuffer * buffer){return GST_FLOW_OK;};
	virtual GstFlowReturn on_buffer(GstBuffer * buffer){return GST_FLOW_OK;};
	virtual void on_eos(){};

	// return true to set the message as attended so upstream doesn't try to process it
	virtual bool on_message(GstMessage* msg){return false;};

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

	bool				posChangingPaused;
	int					pipelineState;
	float				speed;
	gint64				durationNanos;
	bool				isAppSink;


	// callbacks to get called from gstreamer
	friend GstFlowReturn on_new_buffer_from_source (GstAppSink * elt, void * data);
	friend GstFlowReturn on_new_preroll_from_source (GstAppSink * elt, void * data);
	friend void on_eos_from_source (GstAppSink * elt, void * data);

	GstFlowReturn preroll_cb(GstBuffer * buffer);
	GstFlowReturn buffer_cb(GstBuffer * buffer);
	void eos_cb();

};







//-------------------------------------------------
//----------------------------------------- videoUtils
//-------------------------------------------------

class ofGstVideoUtils: public ofBaseVideo, public ofGstUtils{
public:

	ofGstVideoUtils();
	virtual ~ofGstVideoUtils();

	bool 			setPipeline(string pipeline, int bpp=24, bool isStream=false, int w=-1, int h=-1);

	bool 			isFrameNew();
	unsigned char * getPixels();
	ofPixels 		getOFPixels();
	ofPixels 		getOFPixels() const;
	void 			update();

	float 			getHeight();
	float 			getWidth();

	virtual void 	close();

protected:
	virtual GstFlowReturn	on_preroll(GstBuffer * buffer);
	virtual GstFlowReturn	on_buffer(GstBuffer * buffer);
	virtual void			on_eos();
	virtual bool 	allocate()=0;

	bool			allocate(bool gotData);

	ofPixels		pixels;				// 24 bit: rgb
	ofPixels		backPixels;
private:
	bool			bIsFrameNew;			// if we are new
	bool			bHavePixelsChanged;
	bool			bBackPixelsChanged;
	ofMutex			mutex;
};

