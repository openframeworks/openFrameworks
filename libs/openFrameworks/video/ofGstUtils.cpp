#include "ofGstUtils.h"
#include "ofUtils.h"
#include <gst/app/gstappsink.h>
#include <gst/video/video.h>

#include <glib-object.h>
#include <glib.h>
#include <algorithm>

#ifdef TARGET_LINUX
// not needed any more, keeping it for compatibility with previous version
#define LIBUDEV_I_KNOW_THE_API_IS_SUBJECT_TO_CHANGE

extern "C" {
	#include <libudev.h>
}


/* for ioctl query */
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev.h>

#endif

static bool plugin_registered = false;
static bool gst_inited = false;
//------------------------------------
void ofGstDataLock(ofGstVideoData * data){
	pthread_mutex_lock( &(data->buffer_mutex) );
}

//------------------------------------
void ofGstDataUnlock(ofGstVideoData * data){
	pthread_mutex_unlock( &(data->buffer_mutex) );
}



// called when the appsink notifies us that there is a new buffer ready for
// processing

static GstFlowReturn on_new_buffer_from_source (GstAppSink * elt, void * data)
{
  guint size;
  GstBuffer *buffer;

  ofGstVideoData * gstData = (ofGstVideoData *) data;

  //get the buffer from appsink
  buffer = gst_app_sink_pull_buffer (GST_APP_SINK (elt));

  size = GST_BUFFER_SIZE (buffer);

  ofGstDataLock(gstData);
	  if(gstData->pixels){
		  memcpy (gstData->pixels, GST_BUFFER_DATA (buffer), size);
		  gstData->bHavePixelsChanged=true;
	  }
  ofGstDataUnlock(gstData);


  /// we don't need the appsink buffer anymore
  gst_buffer_unref (buffer);

  return GST_FLOW_OK;
}

static GstFlowReturn on_new_preroll_from_source (GstAppSink * elt, void * data)
{
  guint size;
  GstBuffer *buffer;

  ofGstVideoData * gstData = (ofGstVideoData *) data;

  //get the buffer from appsink
  buffer = gst_app_sink_pull_preroll (GST_APP_SINK (elt));

  size = GST_BUFFER_SIZE (buffer);
  /*if(size < data->width*data->height*3){
	  gst_buffer_unref (buffer);
	  return;
  }*/
  ofGstDataLock(gstData);
	  if(gstData->pixels){
		  memcpy (gstData->pixels, GST_BUFFER_DATA (buffer), size);

			  gstData->bHavePixelsChanged=true;

	  }
  ofGstDataUnlock(gstData);


  /// we don't need the appsink buffer anymore
  gst_buffer_unref (buffer);

  return GST_FLOW_OK;
}

void on_eos_from_source (GstAppSink * elt, void * data){

}

static gboolean appsink_plugin_init (GstPlugin * plugin)
{
  gst_element_register (plugin, "appsink", GST_RANK_NONE, GST_TYPE_APP_SINK);

  return TRUE;
}


static void get_video_devices (ofGstCamData & cam_data)
{
#ifdef TARGET_LINUX
	int fd, ok;

	struct udev * my_udev;
	struct udev_enumerate * enumerate;
	struct udev_list_entry * list;
	struct udev_list_entry * entry;

	my_udev = udev_new();
	enumerate = udev_enumerate_new(my_udev);
	udev_enumerate_scan_devices(enumerate);
	list = udev_enumerate_get_list_entry(enumerate);

	/*udev_list_entry_foreach(entry,list){
		const char * name = udev_list_entry_get_name(entry);
		struct udev_device * device = udev_device_new_from_syspath(my_udev, name);
		const char * subsystem = udev_device_get_subsystem(device);
		if(strcmp(subsystem,"video4linux")==0){
			num_devices++;
		}
	}*/

	ofLog (OF_LOG_NOTICE, "Probing devices with udev...");

	/* Initialize webcam structures */
	udev_list_entry_foreach(entry,list){
		const char * name = udev_list_entry_get_name(entry);
		struct udev_device * device = udev_device_new_from_syspath(my_udev, name);
		string subsystem = udev_device_get_subsystem(device);

		if(subsystem != "video4linux") continue;
		const char  *gstreamer_src, *product_name;
		struct v4l2_capability  v2cap;
		struct video_capability v1cap;
		string vendor_id;
		string product_id;

		const char * dev_node = udev_device_get_devnode(device);
		struct udev_list_entry * properties = udev_device_get_properties_list_entry(device);
		struct udev_list_entry * property;
		udev_list_entry_foreach(property,properties){
			const char * name = udev_list_entry_get_name(property);

			if(strcmp(name,"ID_VENDOR_ID")==0){
				vendor_id = udev_list_entry_get_value(property);
			}

			if(strcmp(name,"ID_MODEL_ID")==0){
				product_id = udev_list_entry_get_value(property);
			}

		}


		ofLog (OF_LOG_NOTICE, "Found device " + vendor_id + ":" + product_id + ", getting capabilities...");

		/* vbi devices support capture capability too, but cannot be used,
		 * so detect them by device name */
		if (strstr (dev_node, "vbi"))
		{
			ofLog (OF_LOG_WARNING, "Skipping vbi device: %s", dev_node);
			continue;
		}


		if ((fd = open (dev_node, O_RDONLY | O_NONBLOCK)) < 0)
		{
			ofLog (OF_LOG_WARNING, "Failed to open %s: %s", dev_node, strerror (errno));
			continue;
		}

		ok = ioctl (fd, VIDIOC_QUERYCAP, &v2cap);
		if (ok < 0)
		{
			ok = ioctl (fd, VIDIOCGCAP, &v1cap);
			if (ok < 0)
			{
				ofLog (OF_LOG_WARNING, "Error while probing v4l capabilities for %s: %s",
						dev_node, strerror (errno));
				close (fd);
				continue;
			}
			ofLog (OF_LOG_NOTICE,"Detected v4l device: %s", v1cap.name);
			ofLog (OF_LOG_NOTICE,"Device type: %d", v1cap.type);
			gstreamer_src = "v4lsrc";
			product_name  = v1cap.name;
		}
		else
		{
			guint cap = v2cap.capabilities;
			ofLog (OF_LOG_NOTICE,"Detected v4l2 device: %s", v2cap.card);
			ofLog (OF_LOG_NOTICE,"Driver: %s, version: %d", v2cap.driver, v2cap.version);
			/* g_print ("Bus info: %s\n", v2cap.bus_info); */ /* Doesn't seem anything useful */
			ofLog (OF_LOG_NOTICE,"Capabilities: 0x%08X", v2cap.capabilities);
			if (!(cap & V4L2_CAP_VIDEO_CAPTURE))
			{
			  ofLog (OF_LOG_NOTICE,"Device %s seems to not have the capture capability, (radio tuner?)\n"
					 "Removing it from device list.", dev_node);
			close (fd);
			continue;
			}
			gstreamer_src = "v4l2src";
			product_name  = (char *) v2cap.card;
		}


		ofGstDevice gst_device;
		gst_device.video_device = dev_node;
		gst_device.gstreamer_src = gstreamer_src;
		gst_device.product_name = product_name;
		cam_data.webcam_devices.push_back(gst_device);
		/*cam_data.webcam_devices[cam_data.num_webcam_devices].video_device      = dev_node;
		cam_data.webcam_devices[cam_data.num_webcam_devices].gstreamer_src     = gstreamer_src;
		cam_data.webcam_devices[cam_data.num_webcam_devices].product_name      = product_name;
		cam_data.webcam_devices[cam_data.num_webcam_devices].num_video_formats = 0;
		cam_data.webcam_devices[cam_data.num_webcam_devices].supported_resolutions =
		  g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);
		cam_data.num_webcam_devices++;*/

		close (fd);
	}

	cam_data.bInited=true;
#endif
}


static void get_supported_framerates (ofGstVideoFormat &video_format, GstStructure &structure)
{
	const GValue *framerates;
	ofGstFramerate framerate;
	framerates = gst_structure_get_value (&structure, "framerate");
	if (GST_VALUE_HOLDS_FRACTION (framerates)){
		framerate.numerator   = gst_value_get_fraction_numerator (framerates);
		framerate.denominator = gst_value_get_fraction_denominator (framerates);
		video_format.framerates.push_back(framerate);
		ofLog(OF_LOG_NOTICE,"%d/%d ", framerate.numerator,
						framerate.denominator);
	}else if (GST_VALUE_HOLDS_LIST (framerates)){
		int num_framerates = gst_value_list_get_size (framerates);
		for (int i = 0; i < num_framerates; i++){
			const GValue *value = gst_value_list_get_value (framerates, i);
			framerate.numerator   = gst_value_get_fraction_numerator (value);
			framerate.denominator = gst_value_get_fraction_denominator (value);
			video_format.framerates.push_back(framerate);
			ofLog(OF_LOG_NOTICE,"%d/%d ", framerate.numerator,
							framerate.denominator);
		}
	}else if (GST_VALUE_HOLDS_FRACTION_RANGE (framerates)){
		int           numerator_min, denominator_min, numerator_max, denominator_max;
		const GValue *fraction_range_min;
		const GValue *fraction_range_max;

		fraction_range_min = gst_value_get_fraction_range_min (framerates);
		numerator_min      = gst_value_get_fraction_numerator (fraction_range_min);
		denominator_min    = gst_value_get_fraction_denominator (fraction_range_min);

		fraction_range_max = gst_value_get_fraction_range_max (framerates);
		numerator_max      = gst_value_get_fraction_numerator (fraction_range_max);
		denominator_max    = gst_value_get_fraction_denominator (fraction_range_max);

		ofLog(OF_LOG_NOTICE,"from %d/%d to %d/%d", numerator_min,
				denominator_max, numerator_max, denominator_min);

		for (int i = numerator_min; i <= numerator_max; i++){
			for (int j = denominator_min; j <= denominator_max; j++){
				framerate.numerator = i;
				framerate.denominator = j;
				video_format.framerates.push_back(framerate);
			}
		}
	}else{
		ofLog (OF_LOG_WARNING,"unknown GValue type %s for framerates", G_VALUE_TYPE_NAME (framerates));
	}
}

static void find_framerate (ofGstVideoFormat &format, int desired_framerate = -1)
{
	int framerate_numerator   = 1;
	int framerate_denominator = 1;
	float framerate = -1;
	float prevFramerate = -1;
	// find nearest to desired framerate
	if(desired_framerate!=-1){
		float diff = 9999;
		for(unsigned i=0; i<format.framerates.size(); i++){
			framerate = (float)format.framerates[i].numerator / (float)format.framerates[i].denominator;
			if( fabs((float)desired_framerate - framerate) < diff){
				diff = fabs((float)desired_framerate - framerate );
				framerate_numerator   = format.framerates[i].numerator;
				framerate_denominator = format.framerates[i].denominator;
			}
		}
	// find fastest
	}else{
		for (unsigned i = 0; i < format.framerates.size(); i++){
			prevFramerate = framerate;
			framerate = (float)format.framerates[i].numerator / (float)format.framerates[i].denominator;
			if (framerate > prevFramerate ){
				framerate_numerator   = format.framerates[i].numerator;
				framerate_denominator = format.framerates[i].denominator;
			}
		}
	}

	format.choosen_framerate.numerator = framerate_numerator;
	format.choosen_framerate.denominator = framerate_denominator;
}

static int find_resolution(ofGstDevice &webcam_device, int width, int height){
	for(unsigned i=0; i<webcam_device.video_formats.size(); i++){
		if(webcam_device.video_formats[i].width == width && webcam_device.video_formats[i].height== height)
			return i;
	}
	return -1;
}


static void add_video_format (ofGstDevice &webcam_device,
  ofGstVideoFormat &video_format, GstStructure &format_structure, int desired_framerate)
{

	ofLog(OF_LOG_NOTICE,"%s %d x %d framerates:",
				video_format.mimetype.c_str(), video_format.width,
				video_format.height);
	get_supported_framerates (video_format, format_structure);
	find_framerate (video_format, desired_framerate);

	int i = find_resolution(webcam_device,video_format.width, video_format.height);

	if (i!=-1) { // Resolution already added ?
		float new_framerate = (float)video_format.choosen_framerate.numerator /
								 video_format.choosen_framerate.denominator;
		float curr_framerate = (float)webcam_device.video_formats[i].choosen_framerate.numerator /
								webcam_device.video_formats[i].choosen_framerate.denominator;
		if (desired_framerate == -1){
			if(new_framerate > curr_framerate) {
				ofLog(OF_LOG_VERBOSE,"higher framerate replacing existing format\n");
				webcam_device.video_formats[i] = video_format;
			}else{
				ofLog(OF_LOG_VERBOSE,"already added, skipping\n");
			}
		}else{
			if(fabs(new_framerate - desired_framerate) < fabs(curr_framerate - desired_framerate) ){
				ofLog(OF_LOG_VERBOSE,"more similar framerate replacing existing format\n");
				webcam_device.video_formats[i] = video_format;
			}else{
				ofLog(OF_LOG_VERBOSE,"already added, skipping\n");
			}
		}

		return;
	}

	webcam_device.video_formats.push_back(video_format);
}


// TODO: gets formats for cameras, when a format returns a range it gets
// in steps /2 and *2 from min to max and max to min, for format7 it should be free to get any size
static void get_supported_video_formats (ofGstDevice &webcam_device, GstCaps &caps, int desired_framerate)
{

	int num_structures;

	num_structures = gst_caps_get_size (&caps);
	for (int i = 0; i < num_structures; i++){
		GstStructure *structure;
		const GValue *width, *height;
		structure = gst_caps_get_structure (&caps, i);

		width  = gst_structure_get_value (structure, "width");
		height = gst_structure_get_value (structure, "height");

		if (G_VALUE_HOLDS_INT (width)){
			ofGstVideoFormat video_format;

			video_format.mimetype = gst_structure_get_name (structure);
			gst_structure_get_int (structure, "width", &(video_format.width));
			gst_structure_get_int (structure, "height", &(video_format.height));
			add_video_format(webcam_device, video_format, *structure, desired_framerate);
		}else if (GST_VALUE_HOLDS_INT_RANGE (width)){
			int min_width, max_width, min_height, max_height;
			int cur_width, cur_height;

			min_width  = gst_value_get_int_range_min (width);
			max_width  = gst_value_get_int_range_max (width);
			min_height = gst_value_get_int_range_min (height);
			max_height = gst_value_get_int_range_max (height);

			cur_width  = min_width;
			cur_height = min_height;
			while (cur_width <= max_width && cur_height <= max_height){
				ofGstVideoFormat video_format;

				video_format.mimetype = gst_structure_get_name (structure);
				video_format.width    = cur_width;
				video_format.height   = cur_height;
				add_video_format(webcam_device, video_format, *structure, desired_framerate);
				cur_width  *= 2;
				cur_height *= 2;
			}

			cur_width  = max_width;
			cur_height = max_height;
			while (cur_width > min_width && cur_height > min_height){
				ofGstVideoFormat video_format;

				video_format.mimetype = gst_structure_get_name (structure);
				video_format.width    = cur_width;
				video_format.height   = cur_height;
				add_video_format(webcam_device, video_format, *structure, desired_framerate);
				cur_width  /= 2;
				cur_height /= 2;
			}
		}else{
			ofLog(OF_LOG_ERROR, "unknown GValue type %s, for resolution width", G_VALUE_TYPE_NAME (width));
		}
	}
}

static void get_device_data (ofGstDevice &webcam_device, int desired_framerate)
{
    string pipeline_desc = webcam_device.gstreamer_src + " name=source device=" +
            webcam_device.video_device + " ! fakesink";

    GError * err = NULL;
    GstElement * pipeline = gst_parse_launch (pipeline_desc.c_str(), &err);
    if ((pipeline == NULL) || (err != NULL)){
    	if (err){
    		ofLog(OF_LOG_ERROR, "ofGstUtils: error getting device data: %s", err->message);
    		g_error_free (err);
    	}else{
    		ofLog(OF_LOG_ERROR, "ofGstUtils: error getting device data, cannot get pipeline");
    	}
    	if(pipeline)
    		gst_object_unref (pipeline);
    	return;
    }

	// TODO: try to lower seconds,
    // Start the pipeline and wait for max. 10 seconds for it to start up
	gst_element_set_state (pipeline, GST_STATE_PLAYING);
	GstStateChangeReturn ret = gst_element_get_state (pipeline, NULL, NULL, 10 * GST_SECOND);

	// Check if any error messages were posted on the bus
	GstBus * bus = gst_element_get_bus (pipeline);
	GstMessage * msg = gst_bus_poll (bus, GST_MESSAGE_ERROR, 0);
	gst_object_unref (bus);

	if ((msg == NULL) && (ret == GST_STATE_CHANGE_SUCCESS)){
		gst_element_set_state (pipeline, GST_STATE_PAUSED);

		GstElement *src = gst_bin_get_by_name (GST_BIN (pipeline), "source");
		char       *name;
		g_object_get (G_OBJECT (src), "device-name", &name, (void*)NULL);

		ofLog(OF_LOG_VERBOSE, "Device: %s (%s)\n", name==NULL?"":name, webcam_device.video_device.c_str());
		GstPad     *pad  = gst_element_get_pad (src, "src");
		GstCaps    *caps = gst_pad_get_caps (pad);
		gst_object_unref (pad);

		get_supported_video_formats (webcam_device, *caps, desired_framerate);

		gst_caps_unref (caps);
	}else if(msg){
		gchar *debug;
		gst_message_parse_error(msg, &err, &debug);

		ofLog(OF_LOG_ERROR, "ofGstUtils: error getting device data; module %s reported: %s",
			  gst_element_get_name(GST_MESSAGE_SRC (msg)), err->message);

		g_error_free(err);
		g_free(debug);
	}
	gst_element_set_state (pipeline, GST_STATE_NULL);
	gst_object_unref (pipeline);

}

ofGstUtils::ofGstUtils() {
	bLoaded 					= false;
	width 						= 0;
	height						= 0;
	speed 						= 1;
	bStarted					= false;
	pixels						= NULL;
	nFrames						= 0;
	bPaused						= false;

	gstPipeline					= NULL;
	bIsFrameNew					= false;
	loopMode					= OF_LOOP_NONE;

	durationNanos				= 0;
	bIsMovieDone				= false;
	posChangingPaused			= 0;
	bIsStream					= false;
	bIsCamera					= false;
	bIsCustomWithSink			= false;

	bFrameByFrame 				= false;

	gstData.durationNanos		= 0;
	gstData.nFrames				= 0;
	gstData.speed				= speed;
	gstData.bHavePixelsChanged	= false;
	bHavePixelsChanged			= false;

	camData.bInited				= false;

	deviceID = 0;

	gstSink						= NULL;

	pthread_mutex_init(&(gstData.buffer_mutex),NULL);
	pthread_mutex_init(&seek_mutex,NULL);

	if(!g_thread_supported()){
		g_thread_init(NULL);
	}
	if(!gst_inited){
		gst_init (NULL, NULL);
		gst_inited=true;
		ofLog(OF_LOG_VERBOSE,"ofGstUtils: gstreamer inited");
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

	if (pixels != NULL){
		delete[] pixels;
	}
}

bool ofGstUtils::loadMovie(string name){
	bpp 				= 24;
	bLoaded      		= false;
	bPaused 			= true;
	speed 				= 1.0f;
	bHavePixelsChanged 	= false;
	if( name.find( "://",0 ) == string::npos){
		name 			= "file://"+ofToDataPath(name,true);
		bIsStream		= false;
	}else{
		bIsStream		= true;
	}
	ofLog(OF_LOG_VERBOSE,"loading "+name);

	gstData.loop		= g_main_loop_new (NULL, FALSE);



	gstPipeline = gst_element_factory_make("playbin","player");
	g_object_set(G_OBJECT(gstPipeline), "uri", name.c_str(), (void*)NULL);

	// create the oF appsink for video rgb without sync to clock
	gstSink = gst_element_factory_make("appsink", NULL);
	GstCaps *caps = gst_caps_new_simple("video/x-raw-rgb",
										"bpp", G_TYPE_INT, 24,
										//"depth", G_TYPE_INT, 24,
										/*"endianness",G_TYPE_INT,4321,
										"red_mask",G_TYPE_INT,0xff0000,
										"green_mask",G_TYPE_INT,0x00ff00,
										"blue_mask",G_TYPE_INT,0x0000ff,*/


										NULL);
	gst_app_sink_set_caps(GST_APP_SINK(gstSink), caps);
	gst_caps_unref(caps);
	gst_base_sink_set_sync(GST_BASE_SINK(gstSink), false);

	g_object_set (G_OBJECT(gstPipeline),"video-sink",gstSink,(void*)NULL);


	GstElement *audioSink = gst_element_factory_make("gconfaudiosink", NULL);
	g_object_set (G_OBJECT(gstPipeline),"audio-sink",audioSink,(void*)NULL);

	return startPipeline();;

}

void ofGstUtils::listDevices(){
	if(!camData.bInited) get_video_devices(camData);
	for(unsigned i=0; i<camData.webcam_devices.size(); i++){
		cout << "device " << i << ": " + camData.webcam_devices[i].video_device + ": " + camData.webcam_devices[i].product_name << endl;
	}
}

void ofGstUtils::setDeviceID(unsigned id){
	if(!camData.bInited) get_video_devices(camData);
	if(camData.webcam_devices.size()>id){
		deviceID = id;
	}else{
		ofLog(OF_LOG_WARNING,"ofGstUtils: selected device doesn't exist, selecting device 0");
	}
}

ofGstVideoFormat & ofGstUtils::selectFormat(int w, int h, int desired_framerate){
	int minDiff=999999;
	int mostSimilarFormat=0;

	get_device_data (camData.webcam_devices[deviceID], desired_framerate);

	for(unsigned i=0; i<camData.webcam_devices[deviceID].video_formats.size(); i++){
		if(camData.webcam_devices[deviceID].video_formats[i].width==w && camData.webcam_devices[deviceID].video_formats[i].height==h){
			mostSimilarFormat = i;
			break;
		}
		int diff = abs(camData.webcam_devices[deviceID].video_formats[i].width + camData.webcam_devices[deviceID].video_formats[i].height - w - h);
		if(diff<minDiff){
			minDiff = diff;
			mostSimilarFormat = i;
		}
	}
	camData.webcam_devices[deviceID].current_format = mostSimilarFormat;
	return camData.webcam_devices[deviceID].video_formats[mostSimilarFormat];
}

bool ofGstUtils::initGrabber(int w, int h, int framerate){
	bpp = 24;
	if(!camData.bInited) get_video_devices(camData);

	if(camData.webcam_devices.size()==0){
		ofLog(OF_LOG_ERROR,"ofGstUtils: no devices found exiting without initializing");
		return false;
	}

	ofGstVideoFormat & format = selectFormat(w, h, framerate);

	ofLog(OF_LOG_NOTICE,"ofGstUtils: selected format: " + ofToString(format.width) + "x" + ofToString(format.height) + " " + format.mimetype + " framerate: " + ofToString(format.choosen_framerate.numerator) + "/" + ofToString(format.choosen_framerate.denominator));

	bIsCamera = true;
	bHavePixelsChanged 	= false;

	width = w;
	height = h;

	gstData.loop		= g_main_loop_new (NULL, FALSE);


	const char * decodebin = "";
	if(format.mimetype != "video/x-raw-yuv" && format.mimetype != "video/x-raw-rgb")
		decodebin = "decodebin !";

	const char * scale = "ffmpegcolorspace !";
	if( w!=format.width || h!=format.height )	scale = "ffvideoscale method=2 !";


	string format_str_pipeline = string("%s name=video_source device=%s ! ") +
								 "%s,width=%d,height=%d,framerate=%d/%d ! " +
								 "%s %s " +
								 "appsink name=sink  caps=\"video/x-raw-rgb, width=%d, height=%d, bpp=24\"";

	gchar* pipeline_string =g_strdup_printf (
				      format_str_pipeline.c_str(),
				      camData.webcam_devices[deviceID].gstreamer_src.c_str(),
				      camData.webcam_devices[deviceID].video_device.c_str(),
				      format.mimetype.c_str(),
				      format.width,
				      format.height,
				      format.choosen_framerate.numerator,
				      format.choosen_framerate.denominator,
				      decodebin, scale,
				      w,h);

	ofLog(OF_LOG_NOTICE, "gstreamer pipeline: %s", pipeline_string);

	GError * error = NULL;
	gstPipeline = gst_parse_launch (pipeline_string, &error);

	gstSink = gst_bin_get_by_name(GST_BIN(gstPipeline),"sink");

	gst_base_sink_set_sync(GST_BASE_SINK(gstSink), true);


	if(startPipeline()){
		play();
		return true;
	}else{
		return false;
	}
}


bool ofGstUtils::setPipeline(string pipeline, int bpp, bool isStream, int w, int h){
	this->bpp = bpp;
	bHavePixelsChanged 	= false;
	bIsStream = isStream;

	gstData.loop		= g_main_loop_new (NULL, FALSE);

	if(w!=-1 && h!=-1){
		width=w;
		height=h;
	}

	string caps;
	if(bpp==8)
		caps="video/x-raw-gray, depth=8, bpp=8";
	else if(bpp==32)
		caps="video/x-raw-rgb, depth=32, bpp=32";
	else
		caps="video/x-raw-rgb, depth=24, bpp=24";

	gchar* pipeline_string =
		g_strdup((pipeline + " ! appsink name=sink caps=\"" + caps + "\"").c_str()); // caps=video/x-raw-rgb

	GError * error = NULL;
	gstPipeline = gst_parse_launch (pipeline_string, &error);

	ofLog(OF_LOG_NOTICE, "gstreamer pipeline: %s", pipeline_string);
	if(error!=NULL){
		ofLog(OF_LOG_ERROR,"couldnt create pipeline: " + string(error->message));
		return false;
	}


	gstSink = gst_bin_get_by_name(GST_BIN(gstPipeline),"sink");

	gst_base_sink_set_sync(GST_BASE_SINK(gstSink), true);


	return startPipeline();
}

bool ofGstUtils::setPipelineWithSink(string pipeline){
	bHavePixelsChanged 	= false;
	bIsCustomWithSink	= true;

	gstData.loop		= g_main_loop_new (NULL, FALSE);

	gchar* pipeline_string =
		g_strdup((pipeline).c_str());

	GError * error = NULL;
	gstPipeline = gst_parse_launch (pipeline_string, &error);

	ofLog(OF_LOG_NOTICE, "gstreamer pipeline: %s", pipeline_string);
	if(error!=NULL){
		ofLog(OF_LOG_ERROR,"couldnt create pipeline: " + string(error->message));
		return false;
	}

	gstSink = gst_bin_get_by_name(GST_BIN(gstPipeline),"sink");

	gst_base_sink_set_sync(GST_BASE_SINK(gstSink), true);


	return startPipeline();
}

void ofGstUtils::setFrameByFrame(bool _bFrameByFrame){
	bFrameByFrame = _bFrameByFrame;
	if(gstSink){
		g_object_set (G_OBJECT (gstSink), "sync", !bFrameByFrame, (void*)NULL);
	}

}

bool ofGstUtils::startPipeline(){
	gstData.pipeline=gstPipeline;

	// pause the pipeline
	if(gst_element_set_state(GST_ELEMENT(gstPipeline), GST_STATE_PAUSED) ==
	   GST_STATE_CHANGE_FAILURE) {
		ofLog(OF_LOG_ERROR, "GStreamer: unable to set pipeline to paused\n");

		return false;
	}

	bool ret = false;

	if(!bIsStream){
		ofGstDataLock(&gstData);
		ret = allocate();
		ofGstDataUnlock(&gstData);

	}else{
		ret = true;
	}

	if(gstSink && !bIsCustomWithSink){
		// set the appsink to emit signals to get eos and errors
		g_object_set (G_OBJECT (gstSink), "emit-signals", FALSE, "sync", !bFrameByFrame, (void*)NULL);
		/*g_signal_connect (gstSink, "new-buffer", G_CALLBACK (on_new_buffer_from_source), &gstData);
		g_signal_connect (gstSink, "new-preroll", G_CALLBACK (on_new_preroll_from_source), &gstData);*/

		if(!bFrameByFrame){
			GstAppSinkCallbacks gstCallbacks;
			gstCallbacks.eos = &on_eos_from_source;
			gstCallbacks.new_preroll = &on_new_preroll_from_source;
			gstCallbacks.new_buffer = &on_new_buffer_from_source;

			gst_app_sink_set_callbacks(GST_APP_SINK(gstSink), &gstCallbacks, &gstData, NULL);
		}

	}


	setSpeed(1.0);

	return ret;
}

bool ofGstUtils::allocate(){
	// wait for paused state to query the duration
	if(!bIsStream){
		GstState state = GST_STATE_PAUSED;
		gst_element_get_state(gstPipeline,&state,NULL,2*GST_SECOND);
	}
	if(!bIsCamera){
		GstFormat format=GST_FORMAT_TIME;
		if(!gst_element_query_duration(gstPipeline,&format,&durationNanos))
			ofLog(OF_LOG_WARNING,"GStreamer: cannot query time duration");

		gstData.durationNanos = durationNanos;
		gstData.nFrames		  = 0;
	}

	// query width, height, fps and do data allocation
	if (bIsCamera || (width!=0 && height!=0)) {
		pixels=new unsigned char[width*height*bpp/8];
		gstData.pixels=new unsigned char[width*height*bpp/8];
		memset(pixels,0,width*height*bpp/8);
		memset(gstData.pixels,0,width*height*bpp/8);
		gstData.width = width;
		gstData.height = height;
		gstData.totalsize = 0;
		gstData.lastFrame = 0;
	}else if(gstSink!=NULL && !bIsCustomWithSink){
		if(GstPad* pad = gst_element_get_static_pad(gstSink, "sink")){
			if(gst_video_get_size(GST_PAD(pad), &width, &height)){
				pixels=new unsigned char[width*height*bpp/8];
				gstData.pixels=new unsigned char[width*height*bpp/8];
				memset(pixels,0,width*height*bpp/8);
				memset(gstData.pixels,0,width*height*bpp/8);
				gstData.width = width;
				gstData.height = height;
				gstData.totalsize = 0;
				gstData.lastFrame = 0;
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
				gstData.nFrames = (float)(durationNanos / GST_SECOND) * (float)fps_n/(float)fps_d;
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


	bLoaded = true;
	bHavePixelsChanged = true;
	bStarted = true;
	return bLoaded;
}

bool ofGstUtils::isFrameNew(){
	return bIsFrameNew;
}

unsigned char * ofGstUtils::getPixels(){
	return pixels;
}

void ofGstUtils::update(){
	gstHandleMessage();
	if (bLoaded == true){
		if(!bFrameByFrame){
			ofGstDataLock(&gstData);

				bHavePixelsChanged = gstData.bHavePixelsChanged;
				if (bHavePixelsChanged){
					gstData.bHavePixelsChanged=false;
					bIsMovieDone = false;
					memcpy(pixels,gstData.pixels,width*height*bpp/8);
				}

			ofGstDataUnlock(&gstData);
		}else{
			GstBuffer *buffer;


			//get the buffer from appsink
			if(bPaused) buffer = gst_app_sink_pull_preroll (GST_APP_SINK (gstSink));
			else buffer = gst_app_sink_pull_buffer (GST_APP_SINK (gstSink));

			if(buffer){
				guint size = GST_BUFFER_SIZE (buffer);
				if(pixels){
					memcpy (pixels, GST_BUFFER_DATA (buffer), size);
					bHavePixelsChanged=true;
				}
				/// we don't need the appsink buffer anymore
				gst_buffer_unref (buffer);
			}
		}
	}
	bIsFrameNew = bHavePixelsChanged;
	bHavePixelsChanged = false;
}

void ofGstUtils::play(){
	bPlaying = true;
	setPaused(false);
}

void ofGstUtils::setPaused(bool _bPause){
	bPaused = _bPause;
	//timeLastIdle = ofGetElapsedTimeMillis();
	if(bLoaded){
		if(bPaused)
			gst_element_set_state (gstPipeline, GST_STATE_PAUSED);
		else
			gst_element_set_state (gstPipeline, GST_STATE_PLAYING);
	}
}

int	ofGstUtils::getCurrentFrame(){
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

int	ofGstUtils::getTotalNumFrames(){
	return gstData.nFrames;
}

void ofGstUtils::firstFrame(){
	setFrame(0);
}

void ofGstUtils::nextFrame(){
	gint64 currentFrame = getCurrentFrame();
	if(currentFrame!=-1) setFrame(currentFrame + 1);
}

void ofGstUtils::previousFrame(){
	gint64 currentFrame = getCurrentFrame();
	if(currentFrame!=-1) setFrame(currentFrame - 1);
}

int ofGstUtils::getHeight(){
	return height;
}

int ofGstUtils::getWidth(){
	return width;
}

float ofGstUtils::getPosition(){
	if(gstPipeline){
		gint64 pos=0;
		GstFormat format=GST_FORMAT_TIME;
		if(!gst_element_query_position(GST_ELEMENT(gstPipeline),&format,&pos)){
			ofLog(OF_LOG_ERROR,"GStreamer: cannot query position");
			return -1;
		}
		return (float)pos/(float)durationNanos;
	}else{
		return -1;
	}
}

float ofGstUtils::getSpeed(){
	return speed;
}

float ofGstUtils::getDuration(){
	return (float)durationNanos/(float)GST_SECOND;
}

bool  ofGstUtils::getIsMovieDone(){
	return bIsMovieDone;
}

void ofGstUtils::setPosition(float pct){
	//pct = CLAMP(pct, 0,1);// check between 0 and 1;
	GstFormat format = GST_FORMAT_TIME;
	GstSeekFlags flags = (GstSeekFlags) (GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE);
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
		ofLog(OF_LOG_WARNING,"GStreamer: unable to seek");
		}
	}else{
		if(!gst_element_seek(GST_ELEMENT(gstPipeline),speed, 	format,
				flags,
				GST_SEEK_TYPE_SET,
				0,
				GST_SEEK_TYPE_SET,
				pos)) {
		ofLog(OF_LOG_WARNING,"GStreamer: unable to seek");
		}
	}
}

void ofGstUtils::setVolume(int volume){
	gdouble gvolume = CLAMP(volume,0,10);
	g_object_set(G_OBJECT(gstPipeline), "volume", gvolume, (void*)NULL);
}

void ofGstUtils::setLoopState(int state){
	loopMode = state;
}

void ofGstUtils::setSpeed(float _speed){


	GstFormat format = GST_FORMAT_TIME;
	GstSeekFlags flags = (GstSeekFlags) (GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_SKIP | GST_SEEK_FLAG_ACCURATE);
	gint64 pos;

	if(_speed==0){
		gst_element_set_state (gstPipeline, GST_STATE_PAUSED);
		return;
	}

	if(!gst_element_query_position(GST_ELEMENT(gstPipeline),&format,&pos) || pos<0){
		//ofLog(OF_LOG_ERROR,"GStreamer: cannot query position");
		return;
	}

	speed 				= _speed;
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
		ofLog(OF_LOG_WARNING,"GStreamer: unable to change speed");
		}
	}else{
		if(!gst_element_seek(GST_ELEMENT(gstPipeline),speed, 	format,
				flags,
				GST_SEEK_TYPE_SET,
				0,
				GST_SEEK_TYPE_SET,
				pos)) {
		ofLog(OF_LOG_WARNING,"GStreamer: unable to change speed");
		}
	}

	ofLog(OF_LOG_VERBOSE,"Gstreamer: speed change to %f", speed);

}
void ofGstUtils::setFrame(int frame){ // frame 0 = first frame...
	float pct = (float)frame / (float)gstData.nFrames;
	setPosition(pct);
}

void ofGstUtils::close(){
	if(bLoaded){
		gst_element_set_state(GST_ELEMENT(gstPipeline), GST_STATE_NULL);
		//gst_object_unref(gstSink);
		gst_object_unref(gstPipeline);
	}

	bLoaded = false;
}

void ofGstUtils::seek_lock(){
	 pthread_mutex_lock( &seek_mutex );
}

void ofGstUtils::seek_unlock(){
	pthread_mutex_unlock( &seek_mutex );
}

string getName(GstState state){
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

void ofGstUtils::gstHandleMessage(){
	GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(gstPipeline));
	while(gst_bus_have_pending(bus)) {
		GstMessage* msg = gst_bus_pop(bus);

		ofLog(OF_LOG_VERBOSE,"GStreamer: Got %s message", GST_MESSAGE_TYPE_NAME(msg));

		switch (GST_MESSAGE_TYPE (msg)) {

			case GST_MESSAGE_BUFFERING:
				gint pctBuffered;
				gst_message_parse_buffering(msg,&pctBuffered);
				ofLog(OF_LOG_VERBOSE,"GStreamer: buffering %i\%", pctBuffered);
				if(bIsStream && !bLoaded){
					ofGstDataLock(&gstData);
					allocate();
					ofGstDataUnlock(&gstData);
				}
				if(pctBuffered<100){
					gst_element_set_state (gstPipeline, GST_STATE_PAUSED);
				}else if(!bPaused){
					gst_element_set_state (gstPipeline, GST_STATE_PLAYING);
				}
			break;

			case GST_MESSAGE_DURATION:{
				GstFormat format=GST_FORMAT_TIME;
				//if(!
						gst_element_query_duration(gstPipeline,&format,&durationNanos);
					//	)
					//ofLog(OF_LOG_WARNING,"GStreamer: cannot query duration");
			}break;

			case GST_MESSAGE_STATE_CHANGED:{
				GstState oldstate, newstate, pendstate;
				gst_message_parse_state_changed(msg, &oldstate, &newstate, &pendstate);
				if(!bLoaded){
					ofGstDataLock(&gstData);
					allocate();
					ofGstDataUnlock(&gstData);
				}
				gstData.pipelineState=newstate;
				/*seek_lock();
				if(posChangingPaused && newstate==GST_STATE_PLAYING){
					gst_element_set_state (gstPipeline, GST_STATE_PAUSED);
					posChangingPaused=false;
				}
				seek_unlock();*/

				ofLog(OF_LOG_VERBOSE,"GStreamer: state changed from " + getName(oldstate) + " to " + getName(newstate) + " (" + getName(pendstate) + ")");
			}break;

			case GST_MESSAGE_ASYNC_DONE:
				gstData.speed=speed;
				ofLog(OF_LOG_VERBOSE,"GStreamer: async done");
			break;

			case GST_MESSAGE_ERROR: {
				GError *err;
				gchar *debug;
				gst_message_parse_error(msg, &err, &debug);

				ofLog(OF_LOG_ERROR, "GStreamer Plugin: Embedded video playback halted; module %s reported: %s",
					  gst_element_get_name(GST_MESSAGE_SRC (msg)), err->message);

				g_error_free(err);
				g_free(debug);

				gst_element_set_state(GST_ELEMENT(gstPipeline), GST_STATE_NULL);

			}break;

			case GST_MESSAGE_EOS:
				ofLog(OF_LOG_VERBOSE,"GStreamer: end of the stream.");
				bIsMovieDone = true;

				switch(loopMode){

					case OF_LOOP_NORMAL:{
						GstFormat format = GST_FORMAT_TIME;
						GstSeekFlags flags = (GstSeekFlags) (GST_SEEK_FLAG_FLUSH |GST_SEEK_FLAG_KEY_UNIT);
						gint64 pos;
						gst_element_query_position(GST_ELEMENT(gstPipeline),&format,&pos);

						float loopSpeed;
						if(pos>0)
							loopSpeed=-speed;
						else
							loopSpeed=speed;
						if(!gst_element_seek(GST_ELEMENT(gstPipeline),
											speed,
											format,
											flags,
											GST_SEEK_TYPE_SET,
											0,
											GST_SEEK_TYPE_SET,
											durationNanos)) {
							ofLog(OF_LOG_WARNING,"GStreamer: unable to seek");
						}
					}break;

					case OF_LOOP_PALINDROME:{
						GstFormat format = GST_FORMAT_TIME;
						GstSeekFlags flags = (GstSeekFlags) (GST_SEEK_FLAG_FLUSH |GST_SEEK_FLAG_KEY_UNIT);
						gint64 pos;
						gst_element_query_position(GST_ELEMENT(gstPipeline),&format,&pos);
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
							ofLog(OF_LOG_WARNING,"GStreamer: unable to seek");
						}
					}break;
				}

			break;

			default:
				ofLog(OF_LOG_VERBOSE,"GStreamer: unhandled message");
			break;
		}
		gst_message_unref(msg);
	}

	gst_object_unref(GST_OBJECT(bus));
}
