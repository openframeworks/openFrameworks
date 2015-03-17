/*
 * ofGstVideoUtils.cpp
 *
 *  Created on: 16/01/2011
 *      Author: arturo
 */

#include "ofGstVideoPlayer.h"
#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/app/gstappsink.h>
#include "ofConstants.h"
#include "ofGstUtils.h"


ofGstVideoPlayer::ofGstVideoPlayer(){
	nFrames						= 0;
	internalPixelFormat			= OF_PIXELS_RGB;
	bIsStream					= false;
	bIsAllocated				= false;
	threadAppSink				= false;
	bAsyncLoad					= false;
	videoUtils.setSinkListener(this);
	fps_d = 1;
	fps_n = 1;
}

ofGstVideoPlayer::~ofGstVideoPlayer(){
	close();
}

bool ofGstVideoPlayer::setPixelFormat(ofPixelFormat pixelFormat){
	internalPixelFormat = pixelFormat;
	return true;
}

ofPixelFormat ofGstVideoPlayer::getPixelFormat() const {
	return internalPixelFormat;
}


bool ofGstVideoPlayer::createPipeline(string name){
#ifndef OF_USE_GST_GL
#if GST_VERSION_MAJOR==0
	GstCaps *caps;
	int bpp;
	switch(internalPixelFormat){
	case OF_PIXELS_GRAY:
		bpp = 8;
		caps = gst_caps_new_simple("video/x-raw-gray",
			"bpp", G_TYPE_INT, bpp,
			"depth", G_TYPE_INT, 8,
			NULL);
		break;
	case OF_PIXELS_RGB:
		bpp = 24;
		caps = gst_caps_new_simple("video/x-raw-rgb",
			"bpp", G_TYPE_INT, bpp,
			"depth", G_TYPE_INT, 24,
			"endianness",G_TYPE_INT,4321,
			"red_mask",G_TYPE_INT,0xff0000,
			"green_mask",G_TYPE_INT,0x00ff00,
			"blue_mask",G_TYPE_INT,0x0000ff,
			NULL);
		break;
	case OF_PIXELS_RGBA:
		bpp = 32;
		caps = gst_caps_new_simple("video/x-raw-rgb",
			"bpp", G_TYPE_INT, bpp,
			"depth", G_TYPE_INT, 32,
			"endianness",G_TYPE_INT,4321,
			"red_mask",G_TYPE_INT,0xff000000,
			"green_mask",G_TYPE_INT,0x00ff0000,
			"blue_mask",G_TYPE_INT,0x0000ff00,
			"alpha_mask",G_TYPE_INT,0x000000ff,
			NULL);
		break;
	case OF_PIXELS_BGRA:
		bpp = 32;
		caps = gst_caps_new_simple("video/x-raw-rgb",
			"bpp", G_TYPE_INT, bpp,
			"depth", G_TYPE_INT, 32,
			"endianness",G_TYPE_INT,4321,
			"red_mask",G_TYPE_INT,0x0000ff00,
			"green_mask",G_TYPE_INT,0x00ff0000,
			"blue_mask",G_TYPE_INT,0xff000000,
			"alpha_mask",G_TYPE_INT,0x000000ff,
			NULL);
		break;
	default:
		bpp = 32;
		caps = gst_caps_new_simple("video/x-raw-rgb",
			"bpp", G_TYPE_INT, bpp,
			"depth", G_TYPE_INT, 24,
			"endianness",G_TYPE_INT,4321,
			"red_mask",G_TYPE_INT,0xff0000,
			"green_mask",G_TYPE_INT,0x00ff00,
			"blue_mask",G_TYPE_INT,0x0000ff,
			NULL);
		break;
	}
#else
	string mime="video/x-raw";

	GstCaps *caps;
	if(internalPixelFormat==OF_PIXELS_NATIVE){
		caps = gst_caps_from_string((mime + ",format={RGBA,BGRA,RGB,BGR,RGB16,GRAY8,YV12,I420,NV12,NV21,YUY2}").c_str());
	}else{
		string format = ofGstVideoUtils::getGstFormatName(internalPixelFormat);
		caps = gst_caps_new_simple(mime.c_str(),
											"format", G_TYPE_STRING, format.c_str(),
											NULL);
	}
#endif

	#if GST_VERSION_MAJOR==0
		GstElement * gstPipeline = gst_element_factory_make("playbin2","player");
	#else
		GstElement * gstPipeline = gst_element_factory_make("playbin","player");
	#endif
	g_object_ref_sink(gstPipeline);
	g_object_set(G_OBJECT(gstPipeline), "uri", name.c_str(), (void*)NULL);

	// create the oF appsink for video rgb without sync to clock
	GstElement * gstSink = gst_element_factory_make("appsink", "app_sink");
	gst_app_sink_set_caps(GST_APP_SINK(gstSink), caps);
	gst_caps_unref(caps);

	if(threadAppSink){
		GstElement * appQueue = gst_element_factory_make("queue","appsink_queue");
		g_object_set(G_OBJECT(appQueue), "leaky", 0, "silent", 1, (void*)NULL);
		GstElement* appBin = gst_bin_new("app_bin");
		gst_bin_add(GST_BIN(appBin), appQueue);
		GstPad* appQueuePad = gst_element_get_static_pad(appQueue, "sink");
		GstPad* ghostPad = gst_ghost_pad_new("app_bin_sink", appQueuePad);
		gst_object_unref(appQueuePad);
		gst_element_add_pad(appBin, ghostPad);

		gst_bin_add(GST_BIN(appBin), gstSink);
		gst_element_link(appQueue, gstSink);

		g_object_set (G_OBJECT(gstPipeline),"video-sink",appBin,(void*)NULL);
	}else{
		g_object_set (G_OBJECT(gstPipeline),"video-sink",gstSink,(void*)NULL);
	}

	#ifdef TARGET_WIN32
		GstElement *audioSink = gst_element_factory_make("directsoundsink", NULL);
		g_object_set (G_OBJECT(gstPipeline),"audio-sink",audioSink,(void*)NULL);
	#endif

	return videoUtils.setPipelineWithSink(gstPipeline,gstSink,bIsStream);

#else
	/*auto gstPipeline = gst_parse_launch(("uridecodebin uri=" + name + " ! glcolorscale name=gl_filter ! appsink name=app_sink").c_str(),NULL);
	auto gstSink = gst_bin_get_by_name(GST_BIN(gstPipeline),"app_sink");
	auto glfilter = gst_bin_get_by_name(GST_BIN(gstPipeline),"gl_filter");
	gst_app_sink_set_caps(GST_APP_SINK(gstSink), caps);
	gst_caps_unref(caps);

	glXMakeCurrent (ofGetX11Display(), None, 0);
	glDisplay = (GstGLDisplay *)gst_gl_display_x11_new_with_display(ofGetX11Display());
	glContext = gst_gl_context_new_wrapped (glDisplay, (guintptr) ofGetGLXContext(),
	    		  GST_GL_PLATFORM_GLX, GST_GL_API_OPENGL);

	g_object_set (G_OBJECT (glfilter), "other-context", glContext, NULL);

	// FIXME: this seems to be the way to add the context in 1.4.5
	//
	// GstBus * bus = gst_pipeline_get_bus (GST_PIPELINE(gstPipeline));
	// gst_bus_enable_sync_message_emission (bus);
	// g_signal_connect (bus, "sync-message", G_CALLBACK (sync_bus_call), this);
	// gst_object_unref(bus);

	auto ret = videoUtils.setPipelineWithSink(gstPipeline,gstSink,bIsStream);
	glXMakeCurrent (ofGetX11Display(), ofGetX11Window(), ofGetGLXContext());
	return ret;*/

	return videoUtils.setPipeline("uridecodebin uri=" + name,internalPixelFormat,bIsStream,-1,-1);
	//return videoUtils.setPipeline("filesrc location=" + name + " ! qtdemux ",internalPixelFormat,bIsStream,-1,-1);
#endif
}

void ofGstVideoPlayer::loadAsync(string name){
	bAsyncLoad = true;
	load(name);
}

bool ofGstVideoPlayer::load(string name){
	if( name.find( "file://",0 ) != string::npos){
		bIsStream = bAsyncLoad;
	}else if( name.find( "://",0 ) == string::npos){
		GError * err = NULL;
		gchar* name_ptr = gst_filename_to_uri(ofToDataPath(name).c_str(),&err);
		name = name_ptr;
		g_free(name_ptr);
		if(err) g_free(err);
		//name = ofToDataPath(name);
		bIsStream = bAsyncLoad;
	}else{
		bIsStream = true;
	}
	ofLogVerbose("ofGstVideoPlayer") << "loadMovie(): loading \"" << name << "\"";

	if(isInitialized()){
		gst_element_set_state (videoUtils.getPipeline(), GST_STATE_READY);
		if(!bIsStream){
			gst_element_get_state (videoUtils.getPipeline(), NULL, NULL, -1);
		}
		internalPixelFormat = OF_PIXELS_NATIVE;
		bIsAllocated = false;
		videoUtils.reallocateOnNextFrame();
		g_object_set(G_OBJECT(videoUtils.getPipeline()), "uri", name.c_str(), (void*)NULL);
		gst_element_set_state (videoUtils.getPipeline(), GST_STATE_PAUSED);
		if(!bIsStream){
			gst_element_get_state (videoUtils.getPipeline(), NULL, NULL, -1);
			return allocate();
		}else{
			return true;
		}
	}else{
		ofGstUtils::startGstMainLoop();
		return createPipeline(name) &&
				videoUtils.startPipeline() &&
				(bIsStream || allocate());
	}
}

void ofGstVideoPlayer::setThreadAppSink(bool threaded){
	threadAppSink = threaded;
}


bool ofGstVideoPlayer::allocate(){
	if(bIsAllocated){
		return true;
	}

	guint64 durationNanos = videoUtils.getDurationNanos();

	nFrames		  = 0;
	if(GstPad* pad = gst_element_get_static_pad(videoUtils.getSink(), "sink")){
#if GST_VERSION_MAJOR==0
		int width,height;
		if(gst_video_get_size(GST_PAD(pad), &width, &height)){
			if(!videoUtils.allocate(width,height,internalPixelFormat)) return false;
		}else{
			ofLogError("ofGstVideoPlayer") << "allocate(): couldn't query width and height";
			return false;
		}

		const GValue *framerate = gst_video_frame_rate(pad);
		fps_n=0;
		fps_d=0;
		if(framerate && GST_VALUE_HOLDS_FRACTION (framerate)){
			fps_n = gst_value_get_fraction_numerator (framerate);
			fps_d = gst_value_get_fraction_denominator (framerate);
			nFrames = (float)(durationNanos / (float)GST_SECOND) * (float)fps_n/(float)fps_d;
			ofLogVerbose("ofGstVideoPlayer") << "allocate(): framerate: " << fps_n << "/" << fps_d;
		}else{
			ofLogWarning("ofGstVideoPlayer") << "allocate(): cannot get framerate, frame seek won't work";
		}
		bIsAllocated = true;
#else
		if(GstCaps *caps = gst_pad_get_current_caps (GST_PAD (pad))){
			GstVideoInfo info;
			gst_video_info_init (&info);
			if (gst_video_info_from_caps (&info, caps)){
				ofPixelFormat format = ofGstVideoUtils::getOFFormat(GST_VIDEO_INFO_FORMAT(&info));
				if(format!=internalPixelFormat){
					ofLogVerbose("ofGstVideoPlayer") << "allocating as " << info.width << "x" << info.height << " " << info.finfo->description << " " << info.finfo->name;
					internalPixelFormat = format;
				}
				if(!videoUtils.allocate(info.width,info.height,format)) return false;
			}else{
				ofLogError("ofGstVideoPlayer") << "allocate(): couldn't query width and height";
				return false;
			}

			fps_n = info.fps_n;
			fps_d = info.fps_d;
			nFrames = (float)(durationNanos / (float)GST_SECOND) * (float)fps_n/(float)fps_d;
			gst_caps_unref(caps);
			bIsAllocated = true;
		}else{
			ofLogError("ofGstVideoPlayer") << "allocate(): cannot get pipeline caps";
			bIsAllocated = false;
		}
#endif
		gst_object_unref(GST_OBJECT(pad));
	}else{
		ofLogError("ofGstVideoPlayer") << "allocate(): cannot get sink pad";
		bIsAllocated = false;
	}
	return bIsAllocated;
}

void ofGstVideoPlayer::on_stream_prepared(){
	if(!bIsAllocated) allocate();
}

int	ofGstVideoPlayer::getCurrentFrame() const {
	int frame = 0;

	// zach I think this may fail on variable length frames...
	float pos = getPosition();
	if(pos == -1) return -1;


	float  framePosInFloat = ((float)getTotalNumFrames() * pos);
	int    framePosInInt = (int)framePosInFloat;
	float  floatRemainder = (framePosInFloat - framePosInInt);
	if (floatRemainder > 0.5f) framePosInInt = framePosInInt + 1;
	//frame = (int)ceil((getTotalNumFrames() * getPosition()));
	frame = framePosInInt;

	return frame;
}

int	ofGstVideoPlayer::getTotalNumFrames() const {
	return nFrames;
}

void ofGstVideoPlayer::firstFrame(){
	setFrame(0);
}

void ofGstVideoPlayer::nextFrame(){
	gint64 currentFrame = getCurrentFrame();
	if(currentFrame!=-1) setFrame(currentFrame + 1);
}

void ofGstVideoPlayer::previousFrame(){
	gint64 currentFrame = getCurrentFrame();
	if(currentFrame!=-1) setFrame(currentFrame - 1);
}

void ofGstVideoPlayer::setFrame(int frame){ // frame 0 = first frame...
	float pct = (float)frame / (float)nFrames;
	setPosition(pct);
}

bool ofGstVideoPlayer::isStream() const {
	return bIsStream;
}

void ofGstVideoPlayer::update(){
	videoUtils.update();
}

void ofGstVideoPlayer::play(){
	videoUtils.play();
}

void ofGstVideoPlayer::stop(){
	videoUtils.stop();
}

void ofGstVideoPlayer::setPaused(bool bPause){
	videoUtils.setPaused(bPause);
}

bool ofGstVideoPlayer::isPaused() const {
	return videoUtils.isPaused();
}

bool ofGstVideoPlayer::isLoaded() const {
	return videoUtils.isLoaded();
}

bool ofGstVideoPlayer::isPlaying() const {
	return videoUtils.isPlaying();
}

float ofGstVideoPlayer::getPosition() const {
	return videoUtils.getPosition();
}

float ofGstVideoPlayer::getSpeed() const {
	return videoUtils.getSpeed();
}

float ofGstVideoPlayer::getDuration() const {
	return videoUtils.getDuration();
}

bool ofGstVideoPlayer::getIsMovieDone() const {
	return videoUtils.getIsMovieDone();
}

void ofGstVideoPlayer::setPosition(float pct){
	videoUtils.setPosition(pct);
}

void ofGstVideoPlayer::setVolume(float volume){
	videoUtils.setVolume(volume);
}

void ofGstVideoPlayer::setLoopState(ofLoopType state){
	videoUtils.setLoopState(state);
}

ofLoopType ofGstVideoPlayer::getLoopState() const {
	return videoUtils.getLoopState();
}

void ofGstVideoPlayer::setSpeed(float speed){
	videoUtils.setSpeed(speed);
}

void ofGstVideoPlayer::close(){
	bIsAllocated = false;
	videoUtils.close();
}

bool ofGstVideoPlayer::isFrameNew() const {
	return videoUtils.isFrameNew();
}

ofPixels& ofGstVideoPlayer::getPixels(){
	return videoUtils.getPixels();
}

const ofPixels& ofGstVideoPlayer::getPixels() const {
	return videoUtils.getPixels();
}

ofTexture * ofGstVideoPlayer::getTexturePtr(){
	return videoUtils.getTexture();
}

float ofGstVideoPlayer::getHeight() const {
	return videoUtils.getHeight();
}

float ofGstVideoPlayer::getWidth() const {
	return videoUtils.getWidth();
}

ofGstVideoUtils * ofGstVideoPlayer::getGstVideoUtils(){
	return &videoUtils;
}

void ofGstVideoPlayer::setFrameByFrame(bool frameByFrame){
	videoUtils.setFrameByFrame(frameByFrame);
}

bool ofGstVideoPlayer::isThreadedAppSink() const{
	return threadAppSink;
}

bool ofGstVideoPlayer::isFrameByFrame() const{
	return videoUtils.isFrameByFrame();
}
