#include "ofGstUtils.h"
#ifndef TARGET_ANDROID
#include "ofUtils.h"
#include <gst/app/gstappsink.h>
#include <gst/video/video.h>
#include <gst/video/gstvideometa.h>

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

GMainLoop * ofGstUtils::getGstMainLoop(){
	return mainLoop->getMainLoop();
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
	closing 					= false;

	busWatchID					= 0;

#if GLIB_MINOR_VERSION<32
	if(!g_thread_supported()){
		g_thread_init(NULL);
	}
#endif

	if(!gst_inited){
#ifdef TARGET_WIN32
		string gst_path = g_getenv("GSTREAMER_1_0_ROOT_X86");
		putenv(("GST_PLUGIN_PATH_1_0=" + ofFilePath::join(gst_path, "lib\\gstreamer-1.0") + ";.").c_str());
#endif
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
	if(appsink && !isAppSink) appsink->on_eos();
	if(closing){
		eosMutex.lock();
		eosCondition.signal();
		eosMutex.unlock();
	}
}

bool ofGstUtils::setPipelineWithSink(string pipeline, string sinkname, bool isStream){
	ofGstUtils::startGstMainLoop();

	GError * error = NULL;
	gstPipeline = gst_parse_launch (pipeline.c_str(), &error);

	ofLogNotice("ofGstUtils") << "setPipelineWithSink(): gstreamer pipeline: " << pipeline;

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

	return true;
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

	GstBus * bus = gst_pipeline_get_bus (GST_PIPELINE(gstPipeline));

	if(bus){
		busWatchID = gst_bus_add_watch (bus, (GstBusFunc) busFunction, this);
	}

	gst_object_unref(bus);



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
	//if(!isStream) setSpeed(speed);
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
	if(!bLoaded) return;
	GstState state;

	if(!bPaused){
		state = GST_STATE_PAUSED;
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
		if(!bIsMovieDone && !bPaused && !isStream){
			eosMutex.lock();
			closing = true;
			gst_element_send_event(gstPipeline,gst_event_new_eos());
			try{
				eosCondition.wait(eosMutex,5000);
			}catch(const Poco::TimeoutException & e){
				ofLogWarning("ofGstUtils") << "didn't received EOS in 5s, closing pipeline anyway";
			}
			eosMutex.unlock();
			closing = false;
		}
	}
	stop();

	if(bLoaded){
		gst_element_set_state(GST_ELEMENT(gstPipeline), GST_STATE_NULL);
		gst_element_get_state(gstPipeline,NULL,NULL,2*GST_SECOND);

		if(busWatchID!=0) g_source_remove(busWatchID);

		gst_object_unref(gstPipeline);
		gstPipeline = NULL;
		gstSink = NULL;
	}

	bLoaded = false;
}

/*static string getName(GstState state){
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
}*/

bool ofGstUtils::busFunction(GstBus * bus, GstMessage * message, ofGstUtils * gstUtils){
	return gstUtils->gstHandleMessage(bus,message);
}

bool ofGstUtils::gstHandleMessage(GstBus * bus, GstMessage * msg){
	if(appsink && appsink->on_message(msg)) return true;

		/*ofLogVerbose("ofGstUtils") << "gstHandleMessage(): got " << GST_MESSAGE_TYPE_NAME(msg)
			<< " message from " << GST_MESSAGE_SRC_NAME(msg);*/

	switch (GST_MESSAGE_TYPE (msg)) {

		case GST_MESSAGE_BUFFERING:
			gint pctBuffered;
			gst_message_parse_buffering(msg,&pctBuffered);
			//ofLogVerbose("ofGstUtils") << "gstHandleMessage(): buffering " << pctBuffered;
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
					//ofLogVerbose("ofGstUtils") << "gstHandleMessage(): setting stream pipeline to play";
					play();
				}
			}

			/*ofLogVerbose("ofGstUtils") << "gstHandleMessage(): " << GST_MESSAGE_SRC_NAME(msg) << " state changed from "
					<< getName(oldstate) << " to " << getName(newstate) << " (" + getName(pendstate) << ")";*/
		}break;

		case GST_MESSAGE_ASYNC_DONE:
			//ofLogVerbose("ofGstUtils") << "gstHandleMessage(): async done";
		break;

		case GST_MESSAGE_ERROR: {
			GError *err;
			gchar *debug;
			gst_message_parse_error(msg, &err, &debug);

			/*ofLogVerbose("ofGstUtils") << "gstHandleMessage(): embedded video playback halted for plugin, module "
				<< gst_element_get_name(GST_MESSAGE_SRC (msg)) << "  reported: " << err->message;*/

			g_error_free(err);
			g_free(debug);

			gst_element_set_state(GST_ELEMENT(gstPipeline), GST_STATE_NULL);

		}break;

			case GST_MESSAGE_EOS:{
				ofLogVerbose("ofGstUtils") << "gstHandleMessage(): end of the stream";
				bool isClosing = closing;
				eos_cb();

				if(isClosing){
					busWatchID = 0;
					return false;
				}

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

		}break;

		default:
			//ofLogVerbose("ofGstUtils") << "gstHandleMessage(): unhandled message from " << GST_MESSAGE_SRC_NAME(msg);
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

uint64_t ofGstUtils::getMinLatencyNanos(){
	GstClockTime minlat=0, maxlat=0;
	GstQuery * q = gst_query_new_latency();
	if (gst_element_query (gstPipeline, q)) {
		 gboolean live;
		 gst_query_parse_latency (q, &live, &minlat, &maxlat);
	}
	gst_query_unref (q);
	return minlat;
}

uint64_t ofGstUtils::getMaxLatencyNanos(){
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
	internalPixelFormat			= OF_PIXELS_RGB;
}

ofGstVideoUtils::~ofGstVideoUtils(){
	close();
}

void ofGstVideoUtils::close(){
	ofGstUtils::close();
	Poco::ScopedLock<ofMutex> lock(mutex);
	pixels.clear();
	backPixels.clear();
	eventPixels.clear();
	bIsFrameNew					= false;
	bHavePixelsChanged			= false;
	bBackPixelsChanged			= false;
#if GST_VERSION_MAJOR==0
	if(prevBuffer) gst_buffer_unref (prevBuffer);
	if(buffer && prevBuffer!=buffer) gst_buffer_unref (buffer);
#else
	if(prevBuffer) gst_sample_unref (prevBuffer);
	if(buffer && prevBuffer!=buffer) gst_sample_unref (buffer);
#endif
	prevBuffer = 0;
	buffer = 0;
}

bool ofGstVideoUtils::isInitialized(){
	return isLoaded();
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
					swap(pixels,backPixels);
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


#if GST_VERSION_MAJOR>0
string	ofGstVideoUtils::getGstFormatName(ofPixelFormat format){
	switch(format){
	case OF_PIXELS_GRAY:
		return "GRAY8";
	case OF_PIXELS_RGBA:
		return "RGBA";
	case OF_PIXELS_BGRA:
		return "BGRA";
	case OF_PIXELS_RGB565:
		return "RGB16";
	case OF_PIXELS_NV12:
		return "NV12";
	case OF_PIXELS_NV21:
		return "NV21";
	case OF_PIXELS_YV12:
		return "YV12";
	case OF_PIXELS_I420:
		return "I420";
	case OF_PIXELS_YUY2:
		return "YUY2";
	case OF_PIXELS_RGB:
		return "RGB";
	case OF_PIXELS_BGR:
		return "BGR";
	default:
		return "UNKNOWN";
	}
}

GstVideoFormat	ofGstVideoUtils::getGstFormat(ofPixelFormat format){
	switch(format){
	case OF_PIXELS_GRAY:
		return GST_VIDEO_FORMAT_GRAY8;

	case OF_PIXELS_RGB:
		return GST_VIDEO_FORMAT_RGB;

	case OF_PIXELS_BGR:
		return GST_VIDEO_FORMAT_BGR;

	case OF_PIXELS_RGBA:
		return GST_VIDEO_FORMAT_RGBA;

	case OF_PIXELS_BGRA:
		return GST_VIDEO_FORMAT_BGRA;

	case OF_PIXELS_RGB565:
		return GST_VIDEO_FORMAT_RGB16;

	case OF_PIXELS_NV12:
		return GST_VIDEO_FORMAT_NV12;

	case OF_PIXELS_NV21:
		return GST_VIDEO_FORMAT_NV21;

	case OF_PIXELS_YV12:
		return GST_VIDEO_FORMAT_YV12;

	case OF_PIXELS_I420:
		return GST_VIDEO_FORMAT_I420;

	case OF_PIXELS_YUY2:
		return GST_VIDEO_FORMAT_YUY2;

	case OF_PIXELS_UNKNOWN:
	case OF_PIXELS_GRAY_ALPHA:
	case OF_PIXELS_Y:
	case OF_PIXELS_U:
	case OF_PIXELS_V:
	case OF_PIXELS_UV:
	case OF_PIXELS_VU:
	default:
		return GST_VIDEO_FORMAT_UNKNOWN;
	}
}

ofPixelFormat ofGstVideoUtils::getOFFormat(GstVideoFormat format){
	switch(format){
	case GST_VIDEO_FORMAT_GRAY8:
		return OF_PIXELS_GRAY;

	case GST_VIDEO_FORMAT_RGB:
		return OF_PIXELS_RGB;

	case GST_VIDEO_FORMAT_BGR:
		return OF_PIXELS_BGR;

	case GST_VIDEO_FORMAT_RGBA:
		return OF_PIXELS_RGBA;

	case GST_VIDEO_FORMAT_BGRA:
		return OF_PIXELS_BGRA;

	case GST_VIDEO_FORMAT_RGB16:
		return OF_PIXELS_RGB565;

	case GST_VIDEO_FORMAT_NV12:
		return OF_PIXELS_NV12;

	case GST_VIDEO_FORMAT_NV21:
		return OF_PIXELS_NV21;

	case GST_VIDEO_FORMAT_YV12:
		return OF_PIXELS_YV12;

	case GST_VIDEO_FORMAT_I420:
		return OF_PIXELS_I420;

	case GST_VIDEO_FORMAT_YUY2:
		return OF_PIXELS_YUY2;

	default:
		ofLogError() << "non supported format " << format;
		return OF_PIXELS_UNKNOWN;
	}
}
#endif

bool ofGstVideoUtils::setPipeline(string pipeline, ofPixelFormat pixelFormat, bool isStream, int w, int h){
	string caps;
#if GST_VERSION_MAJOR==0
	switch(pixelFormat){
	case OF_PIXELS_MONO:
		caps="video/x-raw-gray, depth=8, bpp=8";
		break;
	case OF_PIXELS_RGBA:
		caps="video/x-raw-rgb, depth=24, bpp=32, endianness=4321, red_mask=0xff0000, green_mask=0x00ff00, blue_mask=0x0000ff, alpha_mask=0x000000ff";
		break;
	case OF_PIXELS_BGRA:
		caps="video/x-raw-rgb, depth=24, bpp=32, endianness=4321, red_mask=0x0000ff, green_mask=0x00ff00, blue_mask=0xff0000, alpha_mask=0x000000ff";
		break;
	case OF_PIXELS_RGB:
	default:
		caps="video/x-raw-rgb, depth=24, bpp=24, endianness=4321, red_mask=0xff0000, green_mask=0x00ff00, blue_mask=0x0000ff, alpha_mask=0x000000ff";
		break;
	}
#else
	if(pixelFormat!=OF_PIXELS_NATIVE){
		caps="video/x-raw, format="+getGstFormatName(pixelFormat);
	}else{
		caps = "video/x-raw,format={RGBA,BGRA,RGB,BGR,RGB16,GRAY8,YV12,I420,NV12,NV21,YUY2}";
	}
#endif

	if(w!=-1 && h!=-1){
		caps+=", width=" + ofToString(w) + ", height=" + ofToString(h);
	}

	string pipeline_string =
		pipeline + " ! appsink name=ofappsink caps=\"" + caps + "\""; // caps=video/x-raw-rgb

	internalPixelFormat = pixelFormat;
	if((isStream && (w==-1 || h==-1)) || pixelFormat==OF_PIXELS_NATIVE || allocate(w,h,pixelFormat)){
		return setPipelineWithSink(pipeline_string,"ofappsink",isStream);
	}else{
		return false;
	}
}

bool ofGstVideoUtils::setPixelFormat(ofPixelFormat pixelFormat){
	internalPixelFormat = pixelFormat;
	return true;
}

ofPixelFormat ofGstVideoUtils::getPixelFormat(){
	return internalPixelFormat;
}

bool ofGstVideoUtils::allocate(int w, int h, ofPixelFormat pixelFormat){
	Poco::ScopedLock<ofMutex> lock(mutex);
	if(pixelFormat!=internalPixelFormat){
		ofLogNotice("ofGstVideoPlayer") << "allocating with " << w << "x" << h << " " << getGstFormatName(pixelFormat);
	}
	if(bBackPixelsChanged){
		pixels = backPixels;
	}else{
		pixels.allocate(w,h,pixelFormat);
		backPixels.allocate(w,h,pixelFormat);
		pixels.set(0);
		backPixels.set(0);

		bHavePixelsChanged = false;
		bBackPixelsChanged = true;
	}
	prevBuffer = 0;
	internalPixelFormat = pixelFormat;
	return pixels.isAllocated();
}

static GstVideoInfo getVideoInfo(GstSample * sample){
    GstCaps *caps = gst_sample_get_caps(sample);
    GstVideoInfo vinfo;
    gst_video_info_init (&vinfo);
    gst_video_info_from_caps (&vinfo, caps);
    gst_caps_unref(caps);
    return vinfo;
}

#if GST_VERSION_MAJOR==0
GstFlowReturn ofGstVideoUtils::process_frame(GstSample * sample){
	guint size = GST_BUFFER_SIZE (_buffer);
	int stride = 0;
	if(pixels.isAllocated() && pixels.getTotalBytes()!=(int)size){
        stride = gst_video_format_get_row_stride( GST_VIDEO_FORMAT_RGB,0, pixels.getWidth());
        if(stride == (pixels.getWidth() * pixels.getHeight() *  pixels.getBytesPerPixel())) {
            ofLogError("ofGstVideoUtils") << "buffer_cb(): error on new buffer, buffer size: " << size << "!= init size: " << pixels.getTotalBytes();
            gst_buffer_unref (_buffer);
            return GST_FLOW_ERROR;
        }
	}
	mutex.lock();
	if(bBackPixelsChanged && buffer) gst_buffer_unref (buffer);
	if(pixels.isAllocated()){
		buffer = _buffer;
        if(stride > 0) {
            backPixels.setFromAlignedPixels(GST_BUFFER_DATA (buffer),pixels.getWidth(),pixels.getHeight(),pixels.getPixelFormat(),stride);
        }
        else {
            backPixels.setFromExternalPixels(GST_BUFFER_DATA (buffer),pixels.getWidth(),pixels.getHeight(),pixels.getPixelFormat());
            eventPixels.setFromExternalPixels(GST_BUFFER_DATA (buffer),pixels.getWidth(),pixels.getHeight(),pixels.getPixelFormat());
        }
		bBackPixelsChanged=true;
		mutex.unlock();
        if(stride == 0) {
        	ofNotifyEvent(prerollEvent,eventPixels);
        }
	}else{
		if(isStream && appsink){
			appsink->on_stream_prepared();
		}else{
			ofLogError("ofGstVideoUtils") << "preroll_cb(): received a preroll without allocation";
		}
		mutex.unlock();
	}
	return GST_FLOW_OK;
}
#else
GstFlowReturn ofGstVideoUtils::process_sample(GstSample * sample){
	GstBuffer * _buffer = gst_sample_get_buffer(sample);

	/* TODO: get GL and EGL buffers.
	GstVideoGLTextureUploadMeta *upload_meta = gst_buffer_get_video_gl_texture_upload_meta (_buffer);
	if(upload_meta){
		cout << "got gl upload meta" << endl;
		return GST_FLOW_OK;
	}*/

	gst_buffer_map (_buffer, &mapinfo, GST_MAP_READ);
	guint size = mapinfo.size;

	int stride = 0;
	if(pixels.isAllocated() && pixels.getTotalBytes()!=(int)size){
		GstVideoInfo vinfo = getVideoInfo(sample);
		stride = vinfo.stride[0];

		if(stride == (pixels.getWidth() * pixels.getBytesPerPixel())) {
			ofLogError("ofGstVideoUtils") << "buffer_cb(): error on new buffer, buffer size: " << size << "!= init size: " << pixels.getTotalBytes();
			gst_sample_unref (sample);
			return GST_FLOW_ERROR;
		}
	}
	mutex.lock();
	if(bBackPixelsChanged && buffer) gst_sample_unref (buffer);
	buffer = sample;

	if(pixels.isAllocated()){
		if(stride > 0) {
			backPixels.setFromAlignedPixels(mapinfo.data,pixels.getWidth(),pixels.getHeight(),pixels.getPixelFormat(),stride);
		} else {
			backPixels.setFromExternalPixels(mapinfo.data,pixels.getWidth(),pixels.getHeight(),pixels.getPixelFormat());
			eventPixels.setFromExternalPixels(mapinfo.data,pixels.getWidth(),pixels.getHeight(),pixels.getPixelFormat());
		}

		bBackPixelsChanged=true;
		mutex.unlock();
		if(stride == 0) {
			ofNotifyEvent(prerollEvent,eventPixels);
		}
	}else{
		mutex.unlock();
		if(appsink){
			appsink->on_stream_prepared();
		}else{
			GstVideoInfo vinfo = getVideoInfo(sample);
			allocate(vinfo.width,vinfo.height,getOFFormat(vinfo.finfo->format));
		}
	}
	gst_buffer_unmap(_buffer, &mapinfo);
	return GST_FLOW_OK;
}
#endif

#if GST_VERSION_MAJOR==0
GstFlowReturn ofGstVideoUtils::preroll_cb(GstBuffer * _buffer){
	GstFlowReturn ret = process_buffer(_buffer);
	if(ret==GST_FLOW_OK){
		return ofGstUtils::preroll_cb(_buffer);
	}else{
		return ret;
	}
}
#else
GstFlowReturn ofGstVideoUtils::preroll_cb(GstSample * sample){
	GstFlowReturn ret = process_sample(sample);
	if(ret==GST_FLOW_OK){
		return ofGstUtils::preroll_cb(sample);
	}else{
		return ret;
	}
}
#endif

#if GST_VERSION_MAJOR==0
GstFlowReturn ofGstVideoUtils::buffer_cb(GstBuffer * _buffer){
	GstFlowReturn ret = process_buffer(_buffer);
	if(ret==GST_FLOW_OK){
		return ofGstUtils::buffer_cb(_buffer);
	}else{
		return ret;
	}
}
#else
GstFlowReturn ofGstVideoUtils::buffer_cb(GstSample * sample){
	GstFlowReturn ret = process_sample(sample);
	if(ret==GST_FLOW_OK){
		return ofGstUtils::buffer_cb(buffer);
	}else{
		return ret;
	}
}
#endif

void ofGstVideoUtils::eos_cb(){
	ofGstUtils::eos_cb();
	ofEventArgs args;
	ofNotifyEvent(eosEvent,args);
}

#endif
