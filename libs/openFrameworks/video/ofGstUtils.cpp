#include "ofGstUtils.h"
#ifndef TARGET_ANDROID
#include "ofUtils.h"
#include <gst/app/gstappsink.h>
#include <gst/video/video.h>

#include <glib-object.h>
#include <glib.h>
#include <algorithm>





ofGstUtils::ofGstMainLoopThread * ofGstUtils::mainLoop;

void ofGstUtils::startGstMainLoop(){
	static bool initialized = false;
	if(!initialized){
		mainLoop = new ofGstMainLoopThread;
		mainLoop->start();
		initialized=true;
	}
}




//-------------------------------------------------
//----------------------------------------- gstUtils
//-------------------------------------------------

static bool plugin_registered = false;
static bool gst_inited = false;


// called when the appsink notifies us that there is a new buffer ready for
// processing

static GstFlowReturn on_new_buffer_from_source (GstAppSink * elt, void * data){
#if GST_VERSION_MAJOR==0
	GstBuffer *buffer = gst_app_sink_pull_buffer (GST_APP_SINK (elt));
#else
	GstSample *buffer = gst_app_sink_pull_sample (GST_APP_SINK (elt));
#endif
	return ((ofGstUtils*)data)->buffer_cb(buffer);
}

static GstFlowReturn on_new_preroll_from_source (GstAppSink * elt, void * data){
#if GST_VERSION_MAJOR==0
	GstBuffer *buffer;
#else
	GstSample *buffer;
#endif
	buffer = gst_app_sink_pull_preroll(GST_APP_SINK (elt));
	return ((ofGstUtils*)data)->preroll_cb(buffer);
}

static void on_eos_from_source (GstAppSink * elt, void * data){
	((ofGstUtils*)data)->eos_cb();
}

static gboolean appsink_plugin_init (GstPlugin * plugin)
{
  gst_element_register (plugin, "appsink", GST_RANK_NONE, GST_TYPE_APP_SINK);

  return TRUE;
}



ofGstUtils::ofGstUtils() {
	bLoaded 					= false;
	speed 						= 1;
	bPaused						= false;
	bIsMovieDone				= false;
	bPlaying					= false;
	loopMode					= OF_LOOP_NONE;
	bFrameByFrame 				= false;

	gstPipeline					= NULL;
	gstSink						= NULL;

	durationNanos				= 0;

	isAppSink					= false;
	isStream					= false;

	appsink						= NULL;

#if GLIB_MINOR_VERSION<32
	if(!g_thread_supported()){
		g_thread_init(NULL);
	}
#endif

	if(!gst_inited){
		gst_init (NULL, NULL);
		gst_inited=true;
		ofLogVerbose("ofGstUtils") << "gstreamer inited";
	}
	if(!plugin_registered){
		gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR,
					"appsink", (char*)"Element application sink",
					appsink_plugin_init, "0.1", "LGPL", "ofVideoPlayer", "openFrameworks",
					"http://openframeworks.cc/");
		plugin_registered=true;
	}

}

ofGstUtils::~ofGstUtils() {
	close();
}

#if GST_VERSION_MAJOR==0
GstFlowReturn ofGstUtils::preroll_cb(GstBuffer * buffer){
#else
GstFlowReturn ofGstUtils::preroll_cb(GstSample * buffer){
#endif
	bIsMovieDone = false;
	if(appsink) return appsink->on_preroll(buffer);
	else return GST_FLOW_OK;
}


#if GST_VERSION_MAJOR==0
GstFlowReturn ofGstUtils::buffer_cb(GstBuffer * buffer){
#else
GstFlowReturn ofGstUtils::buffer_cb(GstSample * buffer){
#endif
	bIsMovieDone = false;
	if(appsink) return appsink->on_buffer(buffer);
	else return GST_FLOW_OK;
}

void ofGstUtils::eos_cb(){
	bIsMovieDone = true;
	if(appsink) appsink->on_eos();
}

bool ofGstUtils::setPipelineWithSink(string pipeline, string sinkname, bool isStream){
	ofGstUtils::startGstMainLoop();

	gchar* pipeline_string =
		g_strdup((pipeline).c_str());

	GError * error = NULL;
	gstPipeline = gst_parse_launch (pipeline_string, &error);

	ofLogNotice("ofGstUtils") << "setPipelineWithSink(): gstreamer pipeline: " << pipeline_string;
	if(error!=NULL){
		ofLogError("ofGstUtils") << "setPipelineWithSink(): couldn't create pipeline: " << error->message;
		return false;
	}

	if(sinkname!=""){
		gstSink = gst_bin_get_by_name(GST_BIN(gstPipeline),sinkname.c_str());

		if(!gstSink){
			ofLogError("ofGstUtils") << "setPipelineWithSink(): couldn't get sink from string pipeline";
		}
	}

	return setPipelineWithSink(gstPipeline,gstSink,isStream);
}

bool ofGstUtils::setPipelineWithSink(GstElement * pipeline, GstElement * sink, bool isStream_){
	ofGstUtils::startGstMainLoop();

	gstPipeline = pipeline;
	gstSink = sink;
	isStream = isStream_;

	if(gstSink){
		gst_base_sink_set_sync(GST_BASE_SINK(gstSink), true);
	}

	if(gstSink && string(gst_plugin_feature_get_name( GST_PLUGIN_FEATURE(gst_element_get_factory(gstSink))))=="appsink"){
		isAppSink = true;
	}else{
		isAppSink = false;
	}

	return startPipeline();
}

void ofGstUtils::setFrameByFrame(bool _bFrameByFrame){
	bFrameByFrame = _bFrameByFrame;
	if(gstSink){
		g_object_set (G_OBJECT (gstSink), "sync", !bFrameByFrame, (void*)NULL);
	}
}

bool ofGstUtils::isFrameByFrame(){
	return bFrameByFrame;
}

bool ofGstUtils::startPipeline(){

	bPaused 			= true;
	speed 				= 1.0f;


	if(gst_element_set_state (GST_ELEMENT(gstPipeline), GST_STATE_READY) ==	GST_STATE_CHANGE_FAILURE) {
		ofLogError("ofGstUtils") << "startPipeline(): unable to set pipeline to ready";
		return false;
	}
	if(gst_element_get_state (GST_ELEMENT(gstPipeline), NULL, NULL, 10 * GST_SECOND)==GST_STATE_CHANGE_FAILURE){
		ofLogError("ofGstUtils") << "startPipeline(): unable to get pipeline ready status";
		return false;
	}

	// pause the pipeline
	if(gst_element_set_state(GST_ELEMENT(gstPipeline), GST_STATE_PAUSED) ==	GST_STATE_CHANGE_FAILURE) {
		ofLogError("ofGstUtils") << "startPipeline(): unable to pause pipeline";
		return false;
	}

	// wait for paused state to query the duration
	if(!isStream){
		GstState state = GST_STATE_PAUSED;
		if(gst_element_get_state(gstPipeline,&state,NULL,2*GST_SECOND)==GST_STATE_CHANGE_FAILURE){
			ofLogError("ofGstUtils") << "startPipeline(): unable to get pipeline paused state";
			return false;
		}
		bPlaying = true;
		bLoaded = true;
	}

	bus = gst_pipeline_get_bus (GST_PIPELINE(gstPipeline));

	if(bus){
		gst_bus_add_watch (bus, (GstBusFunc) busFunction, this);
	}



	if(isAppSink){
		ofLogVerbose("ofGstUtils") << "startPipeline(): attaching callbacks";
		// set the appsink to not emit signals, we are using callbacks instead
		// and frameByFrame to get buffers by polling instead of callback
		g_object_set (G_OBJECT (gstSink), "emit-signals", FALSE, "sync", !bFrameByFrame, (void*)NULL);
		//gst_app_sink_set_drop(GST_APP_SINK(gstSink),1);
		//gst_app_sink_set_max_buffers(GST_APP_SINK(gstSink),2);

		if(!bFrameByFrame){
			GstAppSinkCallbacks gstCallbacks;
			gstCallbacks.eos = &on_eos_from_source;
			gstCallbacks.new_preroll = &on_new_preroll_from_source;
#if GST_VERSION_MAJOR==0
			gstCallbacks.new_buffer = &on_new_buffer_from_source;
#else
			gstCallbacks.new_sample = &on_new_buffer_from_source;
#endif

			gst_app_sink_set_callbacks(GST_APP_SINK(gstSink), &gstCallbacks, this, NULL);
		}

	}

	if(!isStream){
		setSpeed(1.0);
	}


	return true;
}

void ofGstUtils::play(){
	setPaused(false);

	//this is if we set the speed first but it only can be set when we are playing.
	if(!isStream) setSpeed(speed);
}

void ofGstUtils::setPaused(bool _bPause){
	bPaused = _bPause;
	//timeLastIdle = ofGetElapsedTimeMillis();
	if(bLoaded){
		if(bPlaying){
			if(bPaused){
				gst_element_set_state (gstPipeline, GST_STATE_PAUSED);
			}else{
				gst_element_set_state (gstPipeline, GST_STATE_PLAYING);
			}
		}else{
			GstState state = GST_STATE_PAUSED;
			gst_element_set_state (gstPipeline, state);
			gst_element_get_state(gstPipeline,&state,NULL,2*GST_SECOND);
			if(!bPaused){
				gst_element_set_state (gstPipeline, GST_STATE_PLAYING);
			}
			bPlaying = true;
		}
	}
}

void ofGstUtils::stop(){
	if(!bPlaying) return;
	GstState state = GST_STATE_PAUSED;
	if(!bPaused){
		gst_element_set_state (gstPipeline, state);
		gst_element_get_state(gstPipeline,&state,NULL,2*GST_SECOND);
	}
	state = GST_STATE_READY;
	gst_element_set_state (gstPipeline, state);
	gst_element_get_state(gstPipeline,&state,NULL,2*GST_SECOND);
	bPlaying = false;
	bPaused = true;
}

float ofGstUtils::getPosition(){
	if(gstPipeline){
		gint64 pos=0;
#if GST_VERSION_MAJOR==0
		GstFormat format=GST_FORMAT_TIME;
		if(!gst_element_query_position(GST_ELEMENT(gstPipeline),&format,&pos)){
			ofLogVerbose("ofGstUtils") << "getPosition(): couldn't query position";
			return -1;
		}
#else
		if(!gst_element_query_position(GST_ELEMENT(gstPipeline),GST_FORMAT_TIME,&pos)){
			ofLogVerbose("ofGstUtils") << "getPosition(): couldn't query position";
			return -1;
		}
#endif
		return (float)pos/(float)durationNanos;
	}else{
		return -1;
	}
}

float ofGstUtils::getSpeed(){
	return speed;
}

float ofGstUtils::getDuration(){
	return (float)getDurationNanos()/(float)GST_SECOND;
}

int64_t ofGstUtils::getDurationNanos(){
	GstFormat format = GST_FORMAT_TIME;

#if GST_VERSION_MAJOR==0
	if(!gst_element_query_duration(getPipeline(),&format,&durationNanos))
		ofLogWarning("ofGstUtils") << "getDurationNanos(): couldn't query time duration";
#else
	if(!gst_element_query_duration(getPipeline(),format,&durationNanos))
		ofLogWarning("ofGstUtils") << "getDurationNanos(): couldn't query time duration";
#endif
	return durationNanos;

}

bool  ofGstUtils::getIsMovieDone(){
	if(isAppSink){
		return gst_app_sink_is_eos(GST_APP_SINK(gstSink));
	}else{
		return bIsMovieDone;
	}
}

void ofGstUtils::setPosition(float pct){
	//pct = CLAMP(pct, 0,1);// check between 0 and 1;
	GstFormat format = GST_FORMAT_TIME;
	GstSeekFlags flags = (GstSeekFlags) (GST_SEEK_FLAG_ACCURATE | GST_SEEK_FLAG_FLUSH);
	gint64 pos = (guint64)((double)pct*(double)durationNanos);

	/*if(bPaused){
		seek_lock();
		gst_element_set_state (gstPipeline, GST_STATE_PLAYING);
		posChangingPaused=true;
		seek_unlock();
	}*/
	if(speed>0){
		if(!gst_element_seek(GST_ELEMENT(gstPipeline),speed, 	format,
				flags,
				GST_SEEK_TYPE_SET,
				pos,
				GST_SEEK_TYPE_SET,
				-1)) {
		ofLogWarning("ofGstUtils") << "setPosition(): unable to seek";
		}
	}else{
		if(!gst_element_seek(GST_ELEMENT(gstPipeline),speed, 	format,
				flags,
				GST_SEEK_TYPE_SET,
				0,
				GST_SEEK_TYPE_SET,
				pos)) {
		ofLogWarning("ofGstUtils") << "setPosition(): unable to seek";
		}
	}
}

void ofGstUtils::setVolume(float volume){
	gdouble gvolume = volume;
	g_object_set(G_OBJECT(gstPipeline), "volume", gvolume, (void*)NULL);
}

void ofGstUtils::setLoopState(ofLoopType state){
	loopMode = state;
}

void ofGstUtils::setSpeed(float _speed){
	GstFormat format = GST_FORMAT_TIME;
	GstSeekFlags flags = (GstSeekFlags) (GST_SEEK_FLAG_SKIP | GST_SEEK_FLAG_ACCURATE | GST_SEEK_FLAG_FLUSH);
	gint64 pos;

	if(_speed==0){
		gst_element_set_state (gstPipeline, GST_STATE_PAUSED);
		return;
	}
#if GST_VERSION_MAJOR==0
	if(!gst_element_query_position(GST_ELEMENT(gstPipeline),&format,&pos) || pos<0){
		//ofLogError("ofGstUtils") << "setSpeed(): couldn't query position";
		return;
	}
#else
	if(!gst_element_query_position(GST_ELEMENT(gstPipeline),format,&pos) || pos<0){
		//ofLogError("ofGstUtils") << "setSpeed(): couldn't query position";
		return;
	}
#endif

	speed = _speed;
	//pos = (float)gstData.lastFrame * (float)fps_d / (float)fps_n * GST_SECOND;

	if(!bPaused)
		gst_element_set_state (gstPipeline, GST_STATE_PLAYING);

	if(speed>0){
		if(!gst_element_seek(GST_ELEMENT(gstPipeline),speed, 	format,
				flags,
				GST_SEEK_TYPE_SET,
				pos,
				GST_SEEK_TYPE_SET,
				-1)) {
			ofLogWarning("ofGstUtils") << "setSpeed(): unable to change speed";
		}
	}else{
		if(!gst_element_seek(GST_ELEMENT(gstPipeline),speed, 	format,
				flags,
				GST_SEEK_TYPE_SET,
				0,
				GST_SEEK_TYPE_SET,
				pos)) {
			ofLogWarning("ofGstUtils") << "setSpeed(): unable to change speed";
		}
	}

	ofLogVerbose("ofGstUtils") << "setSpeed(): speed changed to " << speed;

}

void ofGstUtils::close(){
	if(bPlaying){
		stop();
	}
	if(bLoaded){
		gst_element_set_state(GST_ELEMENT(gstPipeline), GST_STATE_NULL);
		gst_element_get_state(gstPipeline,NULL,NULL,2*GST_SECOND);
		// gst_object_unref(gstSink); this crashes, why??

		gst_object_unref(gstPipeline);
		gstPipeline = NULL;
		gstSink = NULL;
	}

	bLoaded = false;
}

static string getName(GstState state){
	switch(state){
	case   GST_STATE_VOID_PENDING:
		return "void pending";
	case   GST_STATE_NULL:
		return "null";
	case   GST_STATE_READY:
		return "ready";
	case   GST_STATE_PAUSED:
		return "paused";
	case   GST_STATE_PLAYING:
		return "playing";
	default:
		return "";
	}
}

bool ofGstUtils::busFunction(GstBus * bus, GstMessage * message, ofGstUtils * gstUtils){
	return gstUtils->gstHandleMessage(bus,message);
}

bool ofGstUtils::gstHandleMessage(GstBus * bus, GstMessage * msg){
	if(appsink && appsink->on_message(msg)) return true;

		ofLogVerbose("ofGstUtils") << "gstHandleMessage(): got " << GST_MESSAGE_TYPE_NAME(msg)
			<< " message from " << GST_MESSAGE_SRC_NAME(msg);

	switch (GST_MESSAGE_TYPE (msg)) {

		case GST_MESSAGE_BUFFERING:
			gint pctBuffered;
			gst_message_parse_buffering(msg,&pctBuffered);
			ofLogVerbose("ofGstUtils") << "gstHandleMessage(): buffering " << pctBuffered;
			/*if(pctBuffered<100){
				gst_element_set_state (gstPipeline, GST_STATE_PAUSED);
			}else if(!bPaused){
				gst_element_set_state (gstPipeline, GST_STATE_PLAYING);
			}*/
		break;

#if GST_VERSION_MAJOR==0
		case GST_MESSAGE_DURATION:{
			GstFormat format=GST_FORMAT_TIME;
			gst_element_query_duration(gstPipeline,&format,&durationNanos);
		}break;
#else
		case GST_MESSAGE_DURATION_CHANGED:
			gst_element_query_duration(gstPipeline,GST_FORMAT_TIME,&durationNanos);
			break;

#endif

		case GST_MESSAGE_STATE_CHANGED:{
			GstState oldstate, newstate, pendstate;
			gst_message_parse_state_changed(msg, &oldstate, &newstate, &pendstate);
			if(isStream && newstate==GST_STATE_PAUSED && !bPlaying ){
				bLoaded = true;
				bPlaying = true;
				if(!bPaused){
					ofLogVerbose("ofGstUtils") << "gstHandleMessage(): setting stream pipeline to play";
					play();
				}
			}

			ofLogVerbose("ofGstUtils") << "gstHandleMessage(): " << GST_MESSAGE_SRC_NAME(msg) << " state changed from "
					<< getName(oldstate) << " to " << getName(newstate) << " (" + getName(pendstate) << ")";
		}break;

		case GST_MESSAGE_ASYNC_DONE:
			ofLogVerbose("ofGstUtils") << "gstHandleMessage(): async done";
		break;

		case GST_MESSAGE_ERROR: {
			GError *err;
			gchar *debug;
			gst_message_parse_error(msg, &err, &debug);

			ofLogVerbose("ofGstUtils") << "gstHandleMessage(): embedded video playback halted for plugin, module "
				<< gst_element_get_name(GST_MESSAGE_SRC (msg)) << "  reported: " << err->message;

			g_error_free(err);
			g_free(debug);

			gst_element_set_state(GST_ELEMENT(gstPipeline), GST_STATE_NULL);

		}break;

			case GST_MESSAGE_EOS:
				ofLogVerbose("ofGstUtils") << "gstHandleMessage(): end of the stream";
				bIsMovieDone = true;


			if(appsink && !isAppSink) appsink->on_eos();

			switch(loopMode){

				case OF_LOOP_NORMAL:{
					GstFormat format = GST_FORMAT_TIME;
					GstSeekFlags flags = (GstSeekFlags) (GST_SEEK_FLAG_FLUSH |GST_SEEK_FLAG_KEY_UNIT);
					gint64 pos;
#if GST_VERSION_MAJOR==0
					gst_element_query_position(GST_ELEMENT(gstPipeline),&format,&pos);
#else
					gst_element_query_position(GST_ELEMENT(gstPipeline),format,&pos);
#endif
					if(!gst_element_seek(GST_ELEMENT(gstPipeline),
										speed,
										format,
										flags,
										GST_SEEK_TYPE_SET,
										0,
										GST_SEEK_TYPE_SET,
										durationNanos)) {
						ofLogWarning("ofGstUtils") << "gstHandleMessage(): unable to seek";
					}
				}break;

				case OF_LOOP_PALINDROME:{
					GstFormat format = GST_FORMAT_TIME;
					GstSeekFlags flags = (GstSeekFlags) (GST_SEEK_FLAG_FLUSH |GST_SEEK_FLAG_KEY_UNIT);
					gint64 pos;
#if GST_VERSION_MAJOR==0
					gst_element_query_position(GST_ELEMENT(gstPipeline),&format,&pos);
#else
					gst_element_query_position(GST_ELEMENT(gstPipeline),format,&pos);
#endif
					float loopSpeed;
					if(pos>0)
						loopSpeed=-speed;
					else
						loopSpeed=speed;
					if(!gst_element_seek(GST_ELEMENT(gstPipeline),
										loopSpeed,
										GST_FORMAT_UNDEFINED,
										flags,
										GST_SEEK_TYPE_NONE,
										0,
										GST_SEEK_TYPE_NONE,
										0)) {
						ofLogWarning("ofGstUtils") << "gstHandleMessage(): unable to seek";
					}
				}break;

				default:
				break;
			}

		break;

		default:
			ofLogVerbose("ofGstUtils") << "gstHandleMessage(): unhandled message from " << GST_MESSAGE_SRC_NAME(msg);
		break;
	}
	return true;
}

GstElement 	* ofGstUtils::getPipeline(){
	return gstPipeline;
}

GstElement 	* ofGstUtils::getSink(){
	return gstSink;
}

GstElement 	* ofGstUtils::getGstElementByName(const string & name){
	return gst_bin_get_by_name(GST_BIN(gstPipeline),name.c_str());
}

void ofGstUtils::setSinkListener(ofGstAppSink * appsink_){
	appsink = appsink_;
}

unsigned long ofGstUtils::getMinLatencyNanos(){
	GstClockTime minlat=0, maxlat=0;
	GstQuery * q = gst_query_new_latency();
	if (gst_element_query (gstPipeline, q)) {
		 gboolean live;
		 gst_query_parse_latency (q, &live, &minlat, &maxlat);
	}
	gst_query_unref (q);
	return minlat;
}

unsigned long ofGstUtils::getMaxLatencyNanos(){
	GstClockTime minlat=0, maxlat=0;
	GstQuery * q = gst_query_new_latency();
	if (gst_element_query (gstPipeline, q)) {
		 gboolean live;
		 gst_query_parse_latency (q, &live, &minlat, &maxlat);
	}
	gst_query_unref (q);
	return maxlat;
}



//-------------------------------------------------
//----------------------------------------- videoUtils
//-------------------------------------------------



ofGstVideoUtils::ofGstVideoUtils(){
	bIsFrameNew					= false;
	bHavePixelsChanged			= false;
	bBackPixelsChanged			= false;
	buffer = 0;
	prevBuffer = 0;
#if GST_VERSION_MAJOR==1
	GstMapInfo initMapinfo		= {0,};
	mapinfo 					= initMapinfo;
#endif
}

ofGstVideoUtils::~ofGstVideoUtils(){
	close();
}

void ofGstVideoUtils::close(){
	ofGstUtils::close();
	Poco::ScopedLock<ofMutex> lock(mutex);
	pixels.clear();
	backPixels.clear();
	bIsFrameNew					= false;
	bHavePixelsChanged			= false;
	bBackPixelsChanged			= false;
#if GST_VERSION_MAJOR==0
	if(prevBuffer) gst_buffer_unref (prevBuffer);
	if(buffer) gst_buffer_unref (buffer);
#else
	if(prevBuffer) gst_sample_unref (prevBuffer);
	if(buffer) gst_sample_unref (buffer);
#endif
	prevBuffer = 0;
	buffer = 0;
}

bool ofGstVideoUtils::isFrameNew(){
	return bIsFrameNew;
}

unsigned char * ofGstVideoUtils::getPixels(){
	return pixels.getPixels();
}

ofPixelsRef ofGstVideoUtils::getPixelsRef(){
	return pixels;
}

void ofGstVideoUtils::update(){
	if (isLoaded()){
		if(!isFrameByFrame()){
			mutex.lock();
				bHavePixelsChanged = bBackPixelsChanged;
				if (bHavePixelsChanged){
					bBackPixelsChanged=false;
					pixels.swap(backPixels);
#if GST_VERSION_MAJOR==0
					if(prevBuffer) gst_buffer_unref (prevBuffer);
#else
					if(prevBuffer) gst_sample_unref (prevBuffer);
#endif
					prevBuffer = buffer;
				}

			mutex.unlock();
		}else{
#if GST_VERSION_MAJOR==0
			GstBuffer *buffer;

			//get the buffer from appsink
			if(isPaused()) buffer = gst_app_sink_pull_preroll (GST_APP_SINK (getSink()));
			else buffer = gst_app_sink_pull_buffer (GST_APP_SINK (getSink()));

			if(buffer){
				if(pixels.isAllocated()){
					if(prevBuffer) gst_buffer_unref (prevBuffer);
					pixels.setFromExternalPixels(GST_BUFFER_DATA (buffer),pixels.getWidth(),pixels.getHeight(),pixels.getNumChannels());
					prevBuffer = buffer;
					bHavePixelsChanged=true;
				}
			}
		}
#else
			GstBuffer *buffer;
			GstSample * sample;

			//get the buffer from appsink
			if(isPaused()){
				sample = gst_app_sink_pull_preroll (GST_APP_SINK (getSink()));
			}else{
				sample = gst_app_sink_pull_sample (GST_APP_SINK (getSink()));
			}
			buffer = gst_sample_get_buffer(sample);

			if(buffer){
				if(pixels.isAllocated()){
					if(prevBuffer) gst_sample_unref (prevBuffer);
					gst_buffer_map (buffer, &mapinfo, GST_MAP_READ);
					//TODO: stride = mapinfo.size / height;
					pixels.setFromExternalPixels(mapinfo.data,pixels.getWidth(),pixels.getHeight(),pixels.getNumChannels());
					prevBuffer = sample;
					bHavePixelsChanged=true;
					gst_buffer_unmap(buffer,&mapinfo);
				}
			}
		}
#endif
	}else{
		ofLogWarning("ofGstVideoUtils") << "update(): ofGstVideoUtils not loaded";
	}
	bIsFrameNew = bHavePixelsChanged;
	bHavePixelsChanged = false;
}

float ofGstVideoUtils::getHeight(){
	return pixels.getHeight();
}

float ofGstVideoUtils::getWidth(){
	return pixels.getWidth();
}

bool ofGstVideoUtils::setPipeline(string pipeline, int bpp, bool isStream, int w, int h){
	string caps;
#if GST_VERSION_MAJOR==0
	if(bpp==8)
		caps="video/x-raw-gray, depth=8, bpp=8";
	else if(bpp==32)
		caps="video/x-raw-rgb, depth=24, bpp=32, endianness=4321, red_mask=0xff0000, green_mask=0x00ff00, blue_mask=0x0000ff, alpha_mask=0x000000ff";
	else
		caps="video/x-raw-rgb, depth=24, bpp=24, endianness=4321, red_mask=0xff0000, green_mask=0x00ff00, blue_mask=0x0000ff, alpha_mask=0x000000ff";
#else
	if(bpp==8)
		caps="video/x-raw, format=GRAY8";
	else if(bpp==32)
		caps="video/x-raw, format=RGBA";
	else
		caps="video/x-raw, format=RGB";
#endif

	if(w!=-1 && h!=-1){
		caps+=", width=" + ofToString(w) + ", height=" + ofToString(h);
	}

	gchar* pipeline_string =
		g_strdup((pipeline + " ! appsink name=ofappsink caps=\"" + caps + "\"").c_str()); // caps=video/x-raw-rgb

	if((isStream && (w==-1 || h==-1)) || allocate(w,h,bpp)){
		return setPipelineWithSink(pipeline_string,"ofappsink",isStream);
	}else{
		return false;
	}
}

bool ofGstVideoUtils::allocate(int w, int h, int _bpp){
	pixels.allocate(w,h,_bpp/8);
	backPixels.allocate(w,h,_bpp/8);
	prevBuffer = 0;
	pixels.set(0);

	bHavePixelsChanged = pixels.isAllocated();
	return pixels.isAllocated();
}

#if GST_VERSION_MAJOR==0
GstFlowReturn ofGstVideoUtils::preroll_cb(GstBuffer * _buffer){
	guint size = GST_BUFFER_SIZE (_buffer);
	if(pixels.isAllocated() && pixels.getWidth()*pixels.getHeight()*pixels.getBytesPerPixel()!=(int)size){
		ofLogError("ofGstVideoUtils") << "preproll_cb(): error preroll buffer size: " << size
			<< "!= init size: " << pixels.getWidth()*pixels.getHeight()*pixels.getBytesPerPixel();
		gst_buffer_unref (_buffer);
		return GST_FLOW_ERROR;
	}
	mutex.lock();
	if(bBackPixelsChanged && buffer) gst_buffer_unref (buffer);
	if(pixels.isAllocated()){
		buffer = _buffer;
		backPixels.setFromExternalPixels(GST_BUFFER_DATA (buffer),pixels.getWidth(),pixels.getHeight(),pixels.getNumChannels());
		eventPixels.setFromExternalPixels(GST_BUFFER_DATA (buffer),pixels.getWidth(),pixels.getHeight(),pixels.getNumChannels());
		bBackPixelsChanged=true;
		mutex.unlock();
		ofNotifyEvent(prerollEvent,eventPixels);
	}else{
		if(isStream && appsink){
			appsink->on_stream_prepared();
		}else{
			ofLogError("ofGstVideoUtils") << "preroll_cb(): received a preroll without allocation";
		}
		mutex.unlock();
	}
	return ofGstUtils::preroll_cb(_buffer);
}
#else
GstFlowReturn ofGstVideoUtils::preroll_cb(GstSample * sample){
	GstBuffer * _buffer = gst_sample_get_buffer(sample);
	gst_buffer_map (_buffer, &mapinfo, GST_MAP_READ);
	guint size = mapinfo.size;
	if(pixels.isAllocated() && pixels.getWidth()*pixels.getHeight()*pixels.getBytesPerPixel()!=(int)size){
		ofLogError("ofGstVideoUtils") << "preproll_cb(): error preroll buffer size: " << size
			<< "!= init size: " << pixels.getWidth()*pixels.getHeight()*pixels.getBytesPerPixel();
		gst_sample_unref (sample);
		return GST_FLOW_ERROR;
	}
	mutex.lock();
	if(bBackPixelsChanged && buffer) gst_sample_unref (buffer);
	buffer = sample;
	if(pixels.isAllocated()){
		backPixels.setFromExternalPixels(mapinfo.data,pixels.getWidth(),pixels.getHeight(),pixels.getNumChannels());
		eventPixels.setFromExternalPixels(mapinfo.data,pixels.getWidth(),pixels.getHeight(),pixels.getNumChannels());
		bBackPixelsChanged=true;
		mutex.unlock();
		ofNotifyEvent(prerollEvent,eventPixels);
	}else{
		if(isStream && appsink){
			appsink->on_stream_prepared();
		}else{
			ofLogWarning("ofGstVideoUtils") << "preroll_cb(): received a preroll without allocation";
		}
		mutex.unlock();
	}
	gst_buffer_unmap(_buffer,&mapinfo);
	return ofGstUtils::preroll_cb(sample);
}
#endif

#if GST_VERSION_MAJOR==0
GstFlowReturn ofGstVideoUtils::buffer_cb(GstBuffer * _buffer){

	guint size;

	size = GST_BUFFER_SIZE (_buffer);
	if(pixels.isAllocated() && pixels.getWidth()*pixels.getHeight()*pixels.getBytesPerPixel()!=(int)size){
		ofLogError("ofGstVideoUtils") << "buffer_cb(): error on new buffer, buffer size: " << size
			<< " != init size: " << pixels.getWidth()*pixels.getHeight()*pixels.getBytesPerPixel();
		gst_buffer_unref (_buffer);
		return GST_FLOW_ERROR;
	}
	mutex.lock();
	if(bBackPixelsChanged && buffer) gst_buffer_unref (buffer);
	if(pixels.isAllocated()){
		buffer = _buffer;
		backPixels.setFromExternalPixels(GST_BUFFER_DATA (buffer),pixels.getWidth(),pixels.getHeight(),pixels.getNumChannels());
		eventPixels.setFromExternalPixels(GST_BUFFER_DATA (buffer),pixels.getWidth(),pixels.getHeight(),pixels.getNumChannels());
		bBackPixelsChanged=true;
		mutex.unlock();
		ofNotifyEvent(bufferEvent,eventPixels);
	}else{
		if(isStream && appsink){
			appsink->on_stream_prepared();
		}else{
			ofLogWarning("ofGSstVideoUtils") << "buffer_cb(): received a preroll without allocation";
		}
		mutex.unlock();
	}

	return ofGstUtils::buffer_cb(buffer);
}
#else
GstFlowReturn ofGstVideoUtils::buffer_cb(GstSample * sample){
	GstBuffer * _buffer = gst_sample_get_buffer(sample);
	gst_buffer_map (_buffer, &mapinfo, GST_MAP_READ);
	guint size = mapinfo.size;
	if(pixels.isAllocated() && pixels.getWidth()*pixels.getHeight()*pixels.getBytesPerPixel()!=(int)size){
		ofLogError("ofGstVideoUtils") << "buffer_cb(): error on new buffer, buffer size: " << size
			<< "!= init size: " << pixels.getWidth()*pixels.getHeight()*pixels.getBytesPerPixel();
		gst_sample_unref (sample);
		return GST_FLOW_ERROR;
	}
	mutex.lock();
	if(bBackPixelsChanged && buffer) gst_sample_unref (buffer);
	buffer = sample;
	if(pixels.isAllocated()){
		backPixels.setFromExternalPixels(mapinfo.data,pixels.getWidth(),pixels.getHeight(),pixels.getNumChannels());
		eventPixels.setFromExternalPixels(mapinfo.data,pixels.getWidth(),pixels.getHeight(),pixels.getNumChannels());
		bBackPixelsChanged=true;
		mutex.unlock();
		ofNotifyEvent(bufferEvent,eventPixels);
	}else{
		if(isStream && appsink){
			appsink->on_stream_prepared();
		}else{
			ofLogWarning("ofGstVideoUtils") << "buffer_cb(): received a preroll without allocation";
		}
		mutex.unlock();
	}
	gst_buffer_unmap(_buffer,&mapinfo);
	return ofGstUtils::buffer_cb(buffer);
}
#endif

void ofGstVideoUtils::eos_cb(){
	ofGstUtils::eos_cb();
	ofEventArgs args;
	ofNotifyEvent(eosEvent,args);
}

#endif
