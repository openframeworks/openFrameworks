/*
 * ofGstVideoGrabber.cpp
 *
 *  Created on: 17/01/2011
 *      Author: arturo
 */

#include "ofGstVideoGrabber.h"


//-------------------------------------------------
//----------------------------------------- grabber
//-------------------------------------------------

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

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35)
	#include <linux/videodev2.h>
	#define VIDIOCGCAP              _IOR('v',1,struct video_capability)     /* Get capabilities */

	struct video_capability
	{
			 char name[32];
			 int type;
			 int channels;   /* Num channels */
			 int audios;     /* Num audio devices */
			 int maxwidth;   /* Supported width */
			 int maxheight;  /* And height */
			 int minwidth;   /* Supported width */
			 int minheight;  /* And height */
	};
#else
	#include <linux/videodev.h>
#endif


#endif

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
			}else if(webcam_device.video_formats[i].mimetype != "video/x-raw-yuv" && webcam_device.video_formats[i].mimetype != "video/x-raw-rgb" && new_framerate == curr_framerate){
				ofLog(OF_LOG_VERBOSE,"non compressed format with same framerate, replacing existing format\n");
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
	gst_element_set_state (pipeline, GST_STATE_READY);
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

ofGstVideoGrabber::ofGstVideoGrabber(){
	bIsCamera			= false;
	camData.bInited		= false;

	deviceID 			= 0;
	attemptFramerate	= -1;
	internalPixelFormat	= OF_PIXELS_RGB;
}

ofGstVideoGrabber::~ofGstVideoGrabber(){

}

void ofGstVideoGrabber::setPixelFormat(ofPixelFormat pixelFormat){
	internalPixelFormat = pixelFormat;
}

void ofGstVideoGrabber::setVerbose(bool bVerbose){
	//if(bVerbose) ofLogSetTopicLogLevel("ofGstVideoGrabber",OF_LOG_VERBOSE);
	//else ofLogResetTopicLogLevel("ofGstVideoGrabber");
}

void ofGstVideoGrabber::listDevices(){
	if(!camData.bInited) get_video_devices(camData);
	for(unsigned i=0; i<camData.webcam_devices.size(); i++){
		cout << "device " << i << ": " + camData.webcam_devices[i].video_device + ": " + camData.webcam_devices[i].product_name << endl;
	}
}

void ofGstVideoGrabber::setDeviceID(int id){
	if(!camData.bInited) get_video_devices(camData);
	if(int(camData.webcam_devices.size())>id){
		deviceID = id;
	}else{
		ofLog(OF_LOG_WARNING,"ofGstUtils: selected device doesn't exist, selecting device 0");
	}
}

ofGstVideoFormat & ofGstVideoGrabber::selectFormat(int w, int h, int desired_framerate){
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

bool ofGstVideoGrabber::initGrabber(int w, int h){
	if(!camData.bInited) get_video_devices(camData);

	if(camData.webcam_devices.size()==0){
		ofLog(OF_LOG_ERROR,"ofGstUtils: no devices found exiting without initializing");
		return false;
	}

	ofGstVideoFormat & format = selectFormat(w, h, attemptFramerate);
	ofLog(OF_LOG_NOTICE,"ofGstUtils: selected device: " + camData.webcam_devices[deviceID].product_name);
	ofLog(OF_LOG_NOTICE,"ofGstUtils: selected format: " + ofToString(format.width) + "x" + ofToString(format.height) + " " + format.mimetype + " framerate: " + ofToString(format.choosen_framerate.numerator) + "/" + ofToString(format.choosen_framerate.denominator));

	bIsCamera = true;


	const char * decodebin = "";
	if(format.mimetype == "video/x-raw-bayer")
		decodebin = "bayer2rgb !";
	else if(format.mimetype != "video/x-raw-yuv" && format.mimetype != "video/x-raw-rgb")
		decodebin = "decodebin2 !";

	const char * scale = "ffmpegcolorspace ";
	if( w!=format.width || h!=format.height )	scale = "ffvideoscale method=2 !";


	string format_str_pipeline = string("%s name=video_source device=%s ! ") +
								 "%s,width=%d,height=%d,framerate=%d/%d ! " +
								 "%s %s ";

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

	int bpp;
	switch(internalPixelFormat){
	case OF_PIXELS_MONO:
		bpp = 8;
		break;
	case OF_PIXELS_RGB:
		bpp = 24;
		break;
	case OF_PIXELS_RGBA:
	case OF_PIXELS_BGRA:
		bpp = 32;
		break;
	default:
		bpp=24;
		break;
	}


	if(	videoUtils.setPipeline(pipeline_string,bpp,false,w,h) ){
		videoUtils.play();
		return true;
	}else{
		return false;
	}
}

void ofGstVideoGrabber::setDesiredFrameRate(int framerate){
	attemptFramerate = framerate;
}

ofGstVideoUtils * ofGstVideoGrabber::getGstVideoUtils(){
	return &videoUtils;
}


void ofGstVideoGrabber::update(){
	videoUtils.update();
}

bool ofGstVideoGrabber::isFrameNew(){
	return videoUtils.isFrameNew();
}

unsigned char * ofGstVideoGrabber::getPixels(){
	return videoUtils.getPixels();
}

ofPixelsRef ofGstVideoGrabber::getPixelsRef(){
	return videoUtils.getPixelsRef();
}

float ofGstVideoGrabber::getHeight(){
	return videoUtils.getHeight();
}

float ofGstVideoGrabber::getWidth(){
	return videoUtils.getWidth();
}

void ofGstVideoGrabber::close(){
	videoUtils.close();
}
