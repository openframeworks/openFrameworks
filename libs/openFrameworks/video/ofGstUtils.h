#pragma once

#include "ofConstants.h"
#ifndef TARGET_ANDROID
#include "ofPixels.h"
#include "ofEvents.h"
#include "ofThread.h"
#include "ofVideoBaseTypes.h"
#define GST_DISABLE_DEPRECATED
#include <gst/gst.h>
#include <gst/gstpad.h>
#include <gst/video/video.h>
#include <queue>
#include <condition_variable>
#include <mutex>

//#define OF_USE_GST_GL
#ifdef OF_USE_GST_GL
#define GST_USE_UNSTABLE_API
#include <gst/gl/gl.h>
#endif

class ofGstAppSink;
class ofTexture;
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

	bool 	setPipelineWithSink(std::string pipeline, std::string sinkname="sink", bool isStream=false);
	bool 	setPipelineWithSink(GstElement * pipeline, GstElement * sink, bool isStream=false);
	bool	startPipeline();

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
	GstElement 	* getGstElementByName(const std::string & name) const;
	uint64_t getMinLatencyNanos() const;
	uint64_t getMaxLatencyNanos() const;

	virtual void close();

	void setSinkListener(ofGstAppSink * appsink);

	// callbacks to get called from gstreamer
#if GST_VERSION_MAJOR==0
	virtual GstFlowReturn preroll_cb(std::shared_ptr<GstBuffer> buffer);
	virtual GstFlowReturn buffer_cb(std::shared_ptr<GstBuffer> buffer);
#else
	virtual GstFlowReturn preroll_cb(std::shared_ptr<GstSample> buffer);
	virtual GstFlowReturn buffer_cb(std::shared_ptr<GstSample> buffer);
#endif
	virtual void 		  eos_cb();

	static void startGstMainLoop();
	static GMainLoop * getGstMainLoop();
	static void quitGstMainLoop();
protected:
	ofGstAppSink * 		appsink;
	bool				isStream;
	bool				closing;

private:
	static bool			busFunction(GstBus * bus, GstMessage * message, ofGstUtils * app);
	bool				gstHandleMessage(GstBus * bus, GstMessage * message);

	bool 				bPlaying;
	bool 				bPaused;
	bool				bIsMovieDone;
	bool 				bLoaded;
	bool 				bFrameByFrame;
	ofLoopType			loopMode;

	GstElement  *		gstSink;
	GstElement 	*		gstPipeline;

	float				speed;
	mutable gint64		durationNanos;
	bool				isAppSink;
	std::condition_variable		eosCondition;
	std::mutex			eosMutex;
	guint				busWatchID;

	class ofGstMainLoopThread: public ofThread{
	public:
		ofGstMainLoopThread()
		:main_loop(nullptr)
		{
		}

		virtual ~ofGstMainLoopThread(){};

		void start(){
			main_loop = g_main_loop_new (NULL, FALSE);
			startThread();
		}
		void threadedFunction(){
			g_main_loop_run (main_loop);
		}

		GMainLoop * getMainLoop(){
			return main_loop;
		}

		void quit(){
			if(main_loop){
				g_main_loop_quit(main_loop);
				waitForThread();
			}
		}
	private:
		GMainLoop *main_loop = nullptr;
	};

	static ofGstMainLoopThread * mainLoop;
};





//-------------------------------------------------
//----------------------------------------- videoUtils
//-------------------------------------------------

class ofGstVideoUtils: public ofBaseVideo, public ofGstUtils{
public:

	ofGstVideoUtils();
	virtual ~ofGstVideoUtils();

	bool 			setPipeline(std::string pipeline, ofPixelFormat pixelFormat=OF_PIXELS_RGB, bool isStream=false, int w=-1, int h=-1);

	bool 			setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat 	getPixelFormat() const;
	bool 			allocate(int w, int h, ofPixelFormat pixelFormat);
	void 			reallocateOnNextFrame();

	bool 			isFrameNew() const;
	ofPixels&		getPixels();
	const ofPixels&	getPixels() const;
	ofTexture * 	getTexture();
	void 			update();

	float 			getHeight() const;
	float 			getWidth() const;

	void 			close();

#if GST_VERSION_MAJOR>0
	static std::string			getGstFormatName(ofPixelFormat format);
	static GstVideoFormat	getGstFormat(ofPixelFormat format);
	static ofPixelFormat	getOFFormat(GstVideoFormat format);
#endif

	bool			isInitialized() const;
	
	// copy pixels from gst buffer to avoid
	// https://bugzilla.gnome.org/show_bug.cgi?id=737427
	void setCopyPixels(bool copy);

	// this events happen in a different thread
	// do not use them for opengl stuff
	ofEvent<ofPixels> prerollEvent;
	ofEvent<ofPixels> bufferEvent;
	ofEvent<ofEventArgs> eosEvent;

protected:
#if GST_VERSION_MAJOR==0
	GstFlowReturn process_buffer(std::shared_ptr<GstBuffer> buffer);
	GstFlowReturn preroll_cb(std::shared_ptr<GstBuffer> buffer);
	GstFlowReturn buffer_cb(std::shared_ptr<GstBuffer> buffer);
#else
	GstFlowReturn process_sample(std::shared_ptr<GstSample> sample);
	GstFlowReturn preroll_cb(std::shared_ptr<GstSample> buffer);
	GstFlowReturn buffer_cb(std::shared_ptr<GstSample> buffer);
#endif
	void			eos_cb();


	ofPixels		pixels;				// 24 bit: rgb
	ofPixels		backPixels;
	ofPixels		eventPixels;
private:
	static gboolean	sync_bus_call (GstBus * bus, GstMessage * msg, gpointer data);
	bool			bIsFrameNew;			// if we are new
	bool			bHavePixelsChanged;
	bool			bBackPixelsChanged;
	std::mutex		mutex;
#if GST_VERSION_MAJOR==0
	std::shared_ptr<GstBuffer> 	frontBuffer, backBuffer;
#else
	std::shared_ptr<GstSample> 	frontBuffer, backBuffer;
	std::queue<std::shared_ptr<GstSample> > bufferQueue;
	GstMapInfo mapinfo;
	#ifdef OF_USE_GST_GL
		ofTexture		frontTexture, backTexture;
	#endif
#endif
	ofPixelFormat	internalPixelFormat;
	bool copyPixels; // fix for certain versions bug with v4l2

#ifdef OF_USE_GST_GL
	GstGLDisplay *		glDisplay;
	GstGLContext *		glContext;
#endif
};


//-------------------------------------------------
//----------------------------------------- appsink listener
//-------------------------------------------------

class ofGstAppSink{
public:
	virtual ~ofGstAppSink(){}
#if GST_VERSION_MAJOR==0
	virtual GstFlowReturn on_preroll(std::shared_ptr<GstBuffer> buffer){
		return GST_FLOW_OK;
	}
	virtual GstFlowReturn on_buffer(std::shared_ptr<GstBuffer> buffer){
		return GST_FLOW_OK;
	}
#else
	virtual GstFlowReturn on_preroll(std::shared_ptr<GstSample> buffer){
		return GST_FLOW_OK;
	}
	virtual GstFlowReturn on_buffer(std::shared_ptr<GstSample> buffer){
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

