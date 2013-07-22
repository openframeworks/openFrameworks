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


ofGstVideoPlayer::ofGstVideoPlayer(){
	nFrames						= 0;
	internalPixelFormat			= OF_PIXELS_RGB;
	bIsStream					= false;
	bIsAllocated				= false;
	threadAppSink				= false;
	videoUtils.setSinkListener(this);
}

ofGstVideoPlayer::~ofGstVideoPlayer(){
	close();
}

bool ofGstVideoPlayer::setPixelFormat(ofPixelFormat pixelFormat){
	internalPixelFormat = pixelFormat;
	return true;
}

ofPixelFormat ofGstVideoPlayer::getPixelFormat(){
	return internalPixelFormat;
}

bool ofGstVideoPlayer::loadMovie(string name){
	close();
	if( name.find( "file://",0 ) != string::npos){
		bIsStream		= false;
	}else if( name.find( "://",0 ) == string::npos){
		name 			= "file://"+ofToDataPath(name,true);
		bIsStream		= false;
	}else{
		bIsStream		= true;
	}
	ofLogVerbose("ofGstVideoPlayer") << "loadMovie(): loading \"" << name << "\"";

	ofGstUtils::startGstMainLoop();

#if GST_VERSION_MAJOR==0
	GstElement * gstPipeline = gst_element_factory_make("playbin2","player");
#else
	GstElement * gstPipeline = gst_element_factory_make("playbin","player");
#endif
	g_object_set(G_OBJECT(gstPipeline), "uri", name.c_str(), (void*)NULL);

	// create the oF appsink for video rgb without sync to clock
	GstElement * gstSink = gst_element_factory_make("appsink", "app_sink");

	gst_base_sink_set_sync(GST_BASE_SINK(gstSink), true);
	gst_app_sink_set_max_buffers(GST_APP_SINK(gstSink), 8);
	gst_app_sink_set_drop (GST_APP_SINK(gstSink),true);
	gst_base_sink_set_max_lateness  (GST_BASE_SINK(gstSink), -1);

#if GST_VERSION_MAJOR==0
	int bpp;
	string mime;
	switch(internalPixelFormat){
	case OF_PIXELS_MONO:
		mime = "video/x-raw-gray";
		bpp = 8;
		break;
	case OF_PIXELS_RGB:
		mime = "video/x-raw-rgb";
		bpp = 24;
		break;
	case OF_PIXELS_RGBA:
	case OF_PIXELS_BGRA:
		mime = "video/x-raw-rgb";
		bpp = 32;
		break;
	default:
		mime = "video/x-raw-rgb";
		bpp=24;
		break;
	}

	GstCaps *caps = gst_caps_new_simple(mime.c_str(),
										"bpp", G_TYPE_INT, bpp,
										"depth", G_TYPE_INT, 24,
										"endianness",G_TYPE_INT,4321,
										"red_mask",G_TYPE_INT,0xff0000,
										"green_mask",G_TYPE_INT,0x00ff00,
										"blue_mask",G_TYPE_INT,0x0000ff,
										"alpha_mask",G_TYPE_INT,0x000000ff,
										NULL);
#else
	int bpp;
	string mime="video/x-raw";
	string format;
	switch(internalPixelFormat){
	case OF_PIXELS_MONO:
		format = "GRAY8";
		bpp = 8;
		break;
	case OF_PIXELS_RGB:
		format = "RGB";
		bpp = 24;
		break;
	case OF_PIXELS_RGBA:
		format = "RGBA";
		bpp = 32;
		break;
	case OF_PIXELS_BGRA:
		format = "BGRA";
		bpp = 32;
		break;
	default:
		format = "RGB";
		bpp=24;
		break;
	}

	GstCaps *caps = gst_caps_new_simple(mime.c_str(),
										"format", G_TYPE_STRING, format.c_str(),
										/*"bpp", G_TYPE_INT, bpp,
										"depth", G_TYPE_INT, 24,
										"endianness",G_TYPE_INT,4321,
										"red_mask",G_TYPE_INT,0xff0000,
										"green_mask",G_TYPE_INT,0x00ff00,
										"blue_mask",G_TYPE_INT,0x0000ff,
										"alpha_mask",G_TYPE_INT,0x000000ff,*/
										NULL);
#endif


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

		gst_bin_add_many(GST_BIN(appBin), gstSink, NULL);
		gst_element_link_many(appQueue, gstSink, NULL);

		g_object_set (G_OBJECT(gstPipeline),"video-sink",appBin,(void*)NULL);
	}else{
		g_object_set (G_OBJECT(gstPipeline),"video-sink",gstSink,(void*)NULL);
	}

#ifdef TARGET_WIN32
	GstElement *audioSink = gst_element_factory_make("directsoundsink", NULL);
	g_object_set (G_OBJECT(gstPipeline),"audio-sink",audioSink,(void*)NULL);

#endif


	videoUtils.setPipelineWithSink(gstPipeline,gstSink,bIsStream);
	if(!bIsStream) return allocate(bpp);
	else return true;
}

void ofGstVideoPlayer::setThreadAppSink(bool threaded){
	threadAppSink = threaded;
}


bool ofGstVideoPlayer::allocate(int bpp){
	if(bIsAllocated) return true;

	guint64 durationNanos = videoUtils.getDurationNanos();

	nFrames		  = 0;
	if(GstPad* pad = gst_element_get_static_pad(videoUtils.getSink(), "sink")){
#if GST_VERSION_MAJOR==0
		int width,height;
		if(gst_video_get_size(GST_PAD(pad), &width, &height)){
			if(!videoUtils.allocate(width,height,bpp)) return false;
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
			nFrames = (float)(durationNanos / GST_SECOND) * (float)fps_n/(float)fps_d;
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
				if(!videoUtils.allocate(info.width,info.height,bpp)) return false;
			}else{
				ofLogError("ofGstVideoPlayer") << "allocate(): couldn't query width and height";
				return false;
			}

			fps_n = info.fps_n;
			fps_d = info.fps_d;
			nFrames = (float)(durationNanos / GST_SECOND) * (float)fps_n/(float)fps_d;
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
	if(!bIsAllocated) allocate(24);
}

int	ofGstVideoPlayer::getCurrentFrame(){
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

int	ofGstVideoPlayer::getTotalNumFrames(){
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

bool ofGstVideoPlayer::isStream(){
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

bool ofGstVideoPlayer::isPaused(){
	return videoUtils.isPaused();
}

bool ofGstVideoPlayer::isLoaded(){
	return videoUtils.isLoaded();
}

bool ofGstVideoPlayer::isPlaying(){
	return videoUtils.isPlaying();
}

float ofGstVideoPlayer::getPosition(){
	return videoUtils.getPosition();
}

float ofGstVideoPlayer::getSpeed(){
	return videoUtils.getSpeed();
}

float ofGstVideoPlayer::getDuration(){
	return videoUtils.getDuration();
}

bool ofGstVideoPlayer::getIsMovieDone(){
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

ofLoopType ofGstVideoPlayer::getLoopState(){
	return videoUtils.getLoopState();
}

void ofGstVideoPlayer::setSpeed(float speed){
	videoUtils.setSpeed(speed);
}

void ofGstVideoPlayer::close(){
	bIsAllocated = false;
	videoUtils.close();
}

bool ofGstVideoPlayer::isFrameNew(){
	return videoUtils.isFrameNew();
}

unsigned char * ofGstVideoPlayer::getPixels(){
	return videoUtils.getPixels();
}

ofPixelsRef ofGstVideoPlayer::getPixelsRef(){
	return videoUtils.getPixelsRef();
}

float ofGstVideoPlayer::getHeight(){
	return videoUtils.getHeight();
}

float ofGstVideoPlayer::getWidth(){
	return videoUtils.getWidth();
}

ofGstVideoUtils * ofGstVideoPlayer::getGstVideoUtils(){
	return &videoUtils;
}

void ofGstVideoPlayer::setFrameByFrame(bool frameByFrame){
	videoUtils.setFrameByFrame(frameByFrame);
}
