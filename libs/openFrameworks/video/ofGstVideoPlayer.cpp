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

//#ifdef TARGET_RASPBERRYPI
#if GST_VERSION_MAJOR==1 && GST_VERSION_MINOR>=4
#include <gst/gl/egl/gstgldisplay_egl.h>
#else
#include <gst/egl/egl.h>
#endif
//#endif
#include "ofAppEGLWindow.h"
#include "ofAppRunner.h"


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
		GError * err = NULL;
		gchar* name_ptr = gst_filename_to_uri(ofToDataPath(name).c_str(),&err);
		name = name_ptr;
		g_free(name_ptr);
		if(err) g_free(err);
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
	if(internalPixelFormat==OF_PIXELS_NATIVE){
		g_object_set(G_OBJECT(gstPipeline), "uri", name.c_str(), (void*)NULL);
	}else{
		g_object_set(G_OBJECT(gstPipeline), "uri", name.c_str(), (void*)NULL);
	}

	// create the oF appsink for video rgb without sync to clock
	GstElement * gstSink = gst_element_factory_make("appsink", "app_sink");

	gst_base_sink_set_sync(GST_BASE_SINK(gstSink), true);
	gst_app_sink_set_max_buffers(GST_APP_SINK(gstSink), 8);
	gst_app_sink_set_drop (GST_APP_SINK(gstSink),true);
	gst_base_sink_set_max_lateness  (GST_BASE_SINK(gstSink), -1);

#if GST_VERSION_MAJOR==0
	GstCaps *caps;
	int bpp;
	switch(internalPixelFormat){
	case OF_PIXELS_MONO:
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
		//caps = gst_caps_new_any();
		caps = gst_caps_from_string((mime + ",format={RGBA,BGRA,RGB,BGR,RGB16,GRAY8,YV12,I420,NV12,NV21,YUY2}").c_str());
		/*
		GstCapsFeatures *features = gst_caps_features_new (GST_CAPS_FEATURE_META_GST_VIDEO_GL_TEXTURE_UPLOAD_META, NULL);
		gst_caps_set_features (caps, 0, features);*/
	}else{
		string format = ofGstVideoUtils::getGstFormatName(internalPixelFormat);
		caps = gst_caps_new_simple(mime.c_str(),
											"format", G_TYPE_STRING, format.c_str(),
											NULL);
	}
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

#ifdef TARGET_LINUX_ARM
#if GST_VERSION_MINOR<4
	/*if(dynamic_cast<ofAppEGLWindow*>(ofGetWindowPtr())){
		EGLDisplay display = ((ofAppEGLWindow*)ofGetWindowPtr())->getEglDisplay();
		GstEGLDisplay * gstEGLDisplay = gst_egl_display_new (display,NULL);
		GstContext *context = gst_context_new_egl_display(gstEGLDisplay,true);
		GstMessage * msg = gst_message_new_have_context (GST_OBJECT (gstPipeline), context);
		gst_element_post_message (GST_ELEMENT_CAST (gstPipeline), msg);
	}*/
#else

	/*if(dynamic_cast<ofAppEGLWindow*>(ofGetWindowPtr())){
		EGLDisplay display = ((ofAppEGLWindow*)ofGetWindowPtr())->getEglDisplay();
		GstGLDisplayEGL * gstEGLDisplay = gst_gl_display_egl_new_with_egl_display (display);
		GstContext *context = gst_context_new();
		gst_gl_display_create_context (context, gstEGLDisplay);
		GstMessage * msg = gst_message_new_have_context (GST_OBJECT (gstPipeline), context);
		gst_element_post_message (GST_ELEMENT_CAST (gstPipeline), msg);
	}*/
#endif
#endif

	return videoUtils.setPipelineWithSink(gstPipeline,gstSink,bIsStream) &&
				videoUtils.startPipeline() &&
				(bIsStream || allocate());
}

void ofGstVideoPlayer::setThreadAppSink(bool threaded){
	threadAppSink = threaded;
}


bool ofGstVideoPlayer::allocate(){
	if(bIsAllocated){
		ofLogWarning("ofGstVideoPlayer") << "already allocated";
		return true;
	}

	guint64 durationNanos = videoUtils.getDurationNanos();

	nFrames		  = 0;
	if(GstPad* pad = gst_element_get_static_pad(videoUtils.getSink(), "sink")){
#if GST_VERSION_MAJOR==0
		int width,height;
		if(gst_video_get_size(GST_PAD(pad), &width, &height)){
			if(!videoUtils.allocate(width,height,pixelFormat)) return false;
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
					ofLogNotice("ofGstVideoPlayer") << "allocating as" << info.width << "x" << info.height << " " << info.finfo->description << " " << info.finfo->name;
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
