/*
 * ofGstVideoUtils.cpp
 *
 *  Created on: 16/01/2011
 *      Author: arturo
 */

#include "ofGstVideoPlayer.h"
#include <gst/video/video.h>


ofGstVideoPlayer::ofGstVideoPlayer(){
	nFrames						= 0;
	internalPixelFormat			= OF_PIXELS_RGB;
	bIsStream					= false;

}

ofGstVideoPlayer::~ofGstVideoPlayer(){

}

void ofGstVideoPlayer::setPixelFormat(ofPixelFormat pixelFormat){
	internalPixelFormat = pixelFormat;
}

bool ofGstVideoPlayer::loadMovie(string name){
	if( name.find( "://",0 ) == string::npos){
		name 			= "file://"+ofToDataPath(name,true);
		bIsStream		= false;
	}else{
		bIsStream		= true;
	}
	ofLog(OF_LOG_VERBOSE,"loading "+name);

	GMainLoop* loop		= g_main_loop_new (NULL, FALSE);

	GstElement * gstPipeline = gst_element_factory_make("playbin2","player");
	g_object_set(G_OBJECT(gstPipeline), "uri", name.c_str(), (void*)NULL);

	// create the oF appsink for video rgb without sync to clock
	GstElement * gstSink = gst_element_factory_make("appsink", "sink");
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
										//"depth", G_TYPE_INT, 24,
										/*"endianness",G_TYPE_INT,4321,
										"red_mask",G_TYPE_INT,0xff0000,
										"green_mask",G_TYPE_INT,0x00ff00,
										"blue_mask",G_TYPE_INT,0x0000ff,*/


										NULL);
	gst_app_sink_set_caps(GST_APP_SINK(gstSink), caps);
	gst_caps_unref(caps);
	gst_base_sink_set_sync(GST_BASE_SINK(gstSink), false);
	gst_app_sink_set_drop (GST_APP_SINK(gstSink),true);

	g_object_set (G_OBJECT(gstPipeline),"video-sink",gstSink,(void*)NULL);


	GstElement *audioSink = gst_element_factory_make("gconfaudiosink", NULL);
	g_object_set (G_OBJECT(gstPipeline),"audio-sink",audioSink,(void*)NULL);

	return setPipelineWithSink(gstPipeline,gstSink);
}


bool ofGstVideoPlayer::allocate(){
	guint64 durationNanos = getDurationNanos();

	nFrames		  = 0;
	if(GstPad* pad = gst_element_get_static_pad(getSink(), "sink")){
		int width,height,bpp=24;
		if(gst_video_get_size(GST_PAD(pad), &width, &height)){
			cout << "allocating " << width << ", "<< height << "@" << bpp << endl;
			pixels.allocate(width,height,bpp);
			backPixels.allocate(width,height,bpp);
			pixels.set(0);
			pixels.set(0);
		}else{
			ofLog(OF_LOG_ERROR,"GStreamer: cannot query width and height");
			return false;
		}

		const GValue *framerate;
		framerate = gst_video_frame_rate(pad);
		fps_n=0;
		fps_d=0;
		if(framerate && GST_VALUE_HOLDS_FRACTION (framerate)){
			fps_n = gst_value_get_fraction_numerator (framerate);
			fps_d = gst_value_get_fraction_denominator (framerate);
			nFrames = (float)(durationNanos / GST_SECOND) * (float)fps_n/(float)fps_d;
			ofLog(OF_LOG_VERBOSE,"ofGstUtils: framerate: %i/%i",fps_n,fps_d);
		}else{
			ofLog(OF_LOG_WARNING,"Gstreamer: cannot get framerate, frame seek won't work");
		}
		gst_object_unref(GST_OBJECT(pad));
	}else{
		ofLog(OF_LOG_ERROR,"GStreamer: cannot get sink pad");
		return false;
	}
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

