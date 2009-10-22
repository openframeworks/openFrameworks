#include "ofUCUtils.h"
#include "ofConstants.h"

#ifdef TARGET_LINUX

#include <stdio.h>
#include <stdint.h>
#include "ofUtils.h"


// ugly hack to make this work with
// new ffmpeg include path in ubuntu jaunty
// with -MMD flag in the compiler
// this will throw a warning instead of an error
// comment out the two lines that doesn't work for you
// to avoid the warnings
extern "C"
{
#include <avformat.h>
#include <swscale.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}


#define FOURCC(a,b,c,d) (unsigned int)((((unsigned int)a))+(((unsigned int)b)<<8)+(((unsigned int)c)<<16)+(((unsigned int)d)<<24))

typedef struct PixelFormatTag {
	PixelFormat pix_fmt;
    unsigned int fourcc;
} PixelFormatTag;

static const PixelFormatTag pixelFormatTags[] = {
	/* Planar formats */
    { PIX_FMT_YUV420P, 	FOURCC('I', '4', '2', '0') },
    { PIX_FMT_YUV420P, 	FOURCC('I', 'Y', 'U', 'V') },
    { PIX_FMT_YUV420P, 	FOURCC('Y', 'V', '1', '2') },
    { PIX_FMT_YUV410P, 	FOURCC('Y', 'U', 'V', '9') },
    { PIX_FMT_YUV411P, 	FOURCC('Y', '4', '1', 'B') },
    { PIX_FMT_YUV422P, 	FOURCC('Y', '4', '2', 'B') },
    { PIX_FMT_GRAY8,   	FOURCC('Y', '8', '0', '0') },
    { PIX_FMT_GRAY8,   	FOURCC(' ', ' ', 'Y', '8') },

    //v4l
    { PIX_FMT_GRAY8,   	FOURCC('G', 'R', 'E', 'Y') },
    //{ PIX_FMT_... 	FOURCC('H', '2', '4', '0') },	//HI420 BT848, seems to not exist in ffmpeg
    { PIX_FMT_RGB565,  	FOURCC('R', 'G', 'B', '6') },	//RGB565
    { PIX_FMT_RGB555 , 	FOURCC('R', 'G', 'B', '5') },	//RGB15
    { PIX_FMT_RGB32,   	FOURCC('R', 'G', 'B', '4') },	//RGB32
    { PIX_FMT_YUYV422, 	FOURCC('Y', 'U', 'Y', 'V') },
    { PIX_FMT_UYYVYY411,FOURCC('Y', '4', '1', '1') },	//YUV411 is this correct??
    //FOURCC( 'R', 'A', 'W', ' ' );						//BT848, seems to not exist in ffmpeg
    { PIX_FMT_YUV422P, 	FOURCC('Y', '4', '2', 'P') },	//YUV422P
    { PIX_FMT_YUV411P, 	FOURCC('4', '1', '1', 'P') },	//YUV411P
    { PIX_FMT_YUV420P, 	FOURCC('Y', 'U', '1', '2') },

    //v4l2
    { PIX_FMT_UYVY422, 	FOURCC('U', 'Y', 'V', 'Y') },
    { PIX_FMT_YUV411P, 	FOURCC('Y', '4', '1', 'P') },
    //{ PIX_FMT_YVU410P, FOURCC('Y', 'V', 'U', '9') },
    { PIX_FMT_YUV410P, 	FOURCC('Y', 'U', 'V', '9') }, 	// is this correct or YUV410 non-planar
    { PIX_FMT_YUV422P, 	FOURCC('4', '2', '2', 'P') },
    { PIX_FMT_YUV411P, 	FOURCC('4', '1', '1', 'P') },
    { PIX_FMT_NV12,    	FOURCC('N', 'V', '1', '2') },
    { PIX_FMT_NV21,    	FOURCC('N', 'V', '2', '1') },
    //{ PIX_FMT_YUV422,  	FOURCC('Y', 'Y', 'U', 'V') },
    { PIX_FMT_RGB32_1, 	FOURCC('R', 'G', 'B', '1') },	// is this correct?
    { PIX_FMT_RGB555,  	FOURCC('R', 'G', 'B', 'O') },
    { PIX_FMT_RGB565,  	FOURCC('R', 'G', 'B', 'P') },
    //{ PIX_FMT_RGB555X,  FOURCC('R', 'G', 'B', 'Q') },
    //{ PIX_FMT_RGB565X,  FOURCC('R', 'G', 'B', 'R') },
    { PIX_FMT_BGR32,   	FOURCC('B', 'G', 'R', '4') },

    //vid21394
    { PIX_FMT_YUYV422, 	FOURCC('Y', 'U', 'Y', '2') },	// is this correct?
    { PIX_FMT_GRAY8,   	FOURCC('Y', '8', '0', '0') },	// is this correct?

    /* dcam */
    { PIX_FMT_YUV444P, 	FOURCC('Y', '4', '4', '4') }, 	// is this correct or non-planar?
    { PIX_FMT_GRAY16,  	FOURCC('Y', '1', '6', '0') }, 	// is this correct?
    { PIX_FMT_RGB24,   	FOURCC('R', 'G', 'B', ' ') },

    /* Packed formats */
    { PIX_FMT_YUYV422, 	FOURCC('Y', 'U', 'Y', '2') },
    { PIX_FMT_YUYV422, 	FOURCC('Y', '4', '2', '2') },
    { PIX_FMT_UYVY422, 	FOURCC('U', 'Y', 'V', 'Y') },
    { PIX_FMT_GRAY8,   	FOURCC('G', 'R', 'E', 'Y') },
    { PIX_FMT_RGB555,  	FOURCC('R', 'G', 'B', 15) },
    { PIX_FMT_BGR555,  	FOURCC('B', 'G', 'R', 15) },
    { PIX_FMT_RGB565,  	FOURCC('R', 'G', 'B', 16) },
    { PIX_FMT_BGR565,  	FOURCC('B', 'G', 'R', 16) },
    { PIX_FMT_BGR24,   	FOURCC('B', 'G', 'R', '3') },
    { PIX_FMT_RGB24,   	FOURCC('R', 'G', 'B', '3') },

    /* quicktime */
    { PIX_FMT_UYVY422, 	FOURCC('2', 'v', 'u', 'y') },
    { PIX_FMT_UYVY422, 	FOURCC('A', 'V', 'U', 'I') },

    { PIX_FMT_NONE, 0 },
};


//--------------------------------------------------------------------
PixelFormat fourcc_to_pix_fmt( unsigned int fourcc)
{
	const PixelFormatTag * tags = pixelFormatTags;
	while (tags->pix_fmt >= 0) {
		if (tags->fourcc == fourcc)
			return tags->pix_fmt;
        tags++;
    }
    return PIX_FMT_NONE;
}

//--------------------------------------------------------------------
void new_frame_cb (unicap_event_t event, unicap_handle_t handle,
    	unicap_data_buffer_t * buffer, void *usr_data){

	((ofUCUtils*)usr_data)->new_frame(buffer);
}


//--------------------------------------------------------------------
ofUCUtils::ofUCUtils(){

	verbose			= false;
	bUCFrameNew		= false;
	pixels			= NULL;
	src				= NULL;
	dst				= NULL;
	deviceReady		= false;
	doConversion	= false;
	pthread_mutex_init(&capture_mutex,NULL);
}


//--------------------------------------------------------------------
ofUCUtils::~ofUCUtils(){

	close_unicap();
	pthread_mutex_destroy(&capture_mutex);
}


//--------------------------------------------------------------------
bool ofUCUtils::open_device(int d) {

	if (!SUCCESS(unicap_enumerate_devices (NULL, &device, d))) {
		ofLog(OF_LOG_ERROR,"ofUCUtils: Error selecting device %d", d);
		return false;
	} else {

		if (!SUCCESS(unicap_open (&handle, &device))) {
			ofLog(OF_LOG_ERROR,"ofUCUtils: Error opening device %d: %s", d,	device.identifier);
			return false;
		}
		deviceReady = true;
	}
	ofLog(OF_LOG_NOTICE,"ofUCUtils: Using device %s",device.device);
	ofLog(OF_LOG_NOTICE,"ofUCUtils: Using module %s",device.vendor_name);

	return true;
}



//--------------------------------------------------------------------
char * ofUCUtils::device_identifier(void){

	return device.identifier;

}



//--------------------------------------------------------------------
// If a 24 bit video format is founded this is the preferred one, if not, the first
// returned by unicap is selected.
//
// Then it tries to set the desired width and height, if these fails, tries find the
// nearest size or to set the default width and height.
//
// On V4L devices 24 bit format is always BGR, so it needs conversion.
// On some V4L devices using non-default width/heigth it reports BGR but returns RGB.
// ffmpeg color conversion
void ofUCUtils::set_format(int w, int h) {
	if(!deviceReady)
		return;
	d_width=w;
	d_height=h;
	unicap_format_t formats[MAX_FORMATS];
	int format_count;
	unicap_status_t status = STATUS_SUCCESS;
	int rgb24 = -1;

	ofLog(OF_LOG_NOTICE,"ofUCUtils : Available formats for this device:");
	for (format_count = 0; SUCCESS (status) && (format_count < MAX_FORMATS); format_count++) {
		status = unicap_enumerate_formats (handle, NULL, &formats[format_count], format_count);
		if (SUCCESS (status)) {
			if (formats[format_count].bpp == 24) {
				rgb24 = format_count;
			}
			ofLog(OF_LOG_NOTICE,
					"ofUCUtils : %d: %s, min size: %dx%d, max size:%dx%d, default size: %dx%d",
					format_count, formats[format_count].identifier,
					formats[format_count].min_size.width,
					formats[format_count].min_size.height,
					formats[format_count].max_size.width,
					formats[format_count].max_size.height,
					formats[format_count].size.width,
					formats[format_count].size.height);
			ofLog(OF_LOG_VERBOSE,"ofUCUtils: available sizes for this format:");
			for(int i=0; i<formats[format_count].size_count;i++){
				ofLog(OF_LOG_VERBOSE,"          %dx%d",formats[format_count].sizes[i].width,formats[format_count].sizes[i].height);
			}
		}
	}

	if (format_count > 0) {
		int selected_format = 0;
		if (rgb24 != -1)
			selected_format = rgb24;
		else{
			for(selected_format=0;selected_format<format_count;selected_format++){
				format = formats[selected_format];
				if(fourcc_to_pix_fmt(format.fourcc)!=-1)
					break;
			}
		}
		format = formats[selected_format];

		src_pix_fmt=fourcc_to_pix_fmt(format.fourcc);
		if( src_pix_fmt==-1){
			ofLog(OF_LOG_ERROR,"ofUCUtils : Format not suported\n");
			return;
		}

		bool exactMatch  = false;
		int sizeDiff = 99999999;
		int mostAproxSize = -1;

		for(int i=0; i<format.size_count;i++){
			if(format.sizes[i].width == w && format.sizes[i].height==h){
				exactMatch=true;
				format.size.width  = format.sizes[i].width;
				format.size.height = format.sizes[i].height;
				break;
			}else{
				if(abs(format.sizes[i].width-w)+abs(format.sizes[i].height-h)<sizeDiff){
					sizeDiff=abs(format.sizes[i].width-w)+abs(format.sizes[i].height-h);
					mostAproxSize=i;
				}
			}
		}
		if(!exactMatch && mostAproxSize!=-1){
			format.size.width  = format.sizes[mostAproxSize].width;
			format.size.height = format.sizes[mostAproxSize].height;

			ofLog(OF_LOG_WARNING, "ofUCUtils : Can't set video format %s, with size %dx%d, will use %dx%d",
							format.identifier, w, h,
							format.size.width, format.size.height);
		}else if(format.size_count==0){
			int defaultFormatWidth = format.size.width;
			int defaultFormatHeight = format.size.height;

			format.size.width  = w;
			format.size.height = h;

			ofLog(OF_LOG_WARNING, "ofUCUtils : Can't recognize supported video sizes for %s, trying with requested size: %i,%i",
										format.identifier, format.size.width, format.size.height);

			if ( !SUCCESS ( unicap_set_format (handle, &format) ) ) {
				format.size.width  = defaultFormatWidth;
				format.size.height = defaultFormatHeight;

				ofLog(OF_LOG_WARNING, "ofUCUtils : Can't set requested size, trying with format defaults: %i,%i",
						defaultFormatWidth, defaultFormatHeight);
			}else{
				exactMatch=true; //if we can set the requested size -> exactMatch
			}
			ofLog(OF_LOG_WARNING, "ofUCUtils : If this doesn't work try using the reported default size in initGrabber:",
					defaultFormatWidth, defaultFormatHeight);
		}
		if ( !SUCCESS ( unicap_set_format (handle, &format) ) ) {
			ofLog(OF_LOG_ERROR, "ofUCUtils : Failed to set alternative video format!");
			return;
		}
		ofLog(OF_LOG_NOTICE,"ofUCUtils : Selected format: %s, with size %dx%d\n", format.identifier,
				format.size.width, format.size.height);



		if(src_pix_fmt!=PIX_FMT_RGB24 || !exactMatch){
			doConversion = true;
			src=new AVPicture;
			avpicture_alloc(src,(PixelFormat)src_pix_fmt,format.size.width,format.size.height);
			dst=new AVPicture;
			avpicture_alloc(dst,PIX_FMT_RGB24,d_width,d_height);

			toRGB_convert_ctx = sws_getContext(
							format.size.width, format.size.height, (PixelFormat)src_pix_fmt,
							d_width, d_height, PIX_FMT_RGB24,
							VIDEOGRABBER_RESIZE_FLAGS, NULL, NULL, NULL);


			ofLog(OF_LOG_NOTICE,"ofUCUtils: Converting to RGB24 (%i,%i)\n",w,h);

			pixels=new unsigned char[d_width*d_height*3];
		}

	   if( !SUCCESS( unicap_get_format( handle, &format ) ) )
	   {
		   ofLog(OF_LOG_ERROR, "can't get format" );
		   return;
	   }

		format.buffer_type = UNICAP_BUFFER_TYPE_SYSTEM;

	   if( !SUCCESS( unicap_set_format( handle, &format ) ) )
	   {
		   ofLog(OF_LOG_WARNING, "ofUCUtils: Failed to activate SYSTEM_BUFFERS" );
	   }
	}
}


//--------------------------------------------------------------------
void ofUCUtils::start_capture() {

	if(!deviceReady)
		return;

	int status = STATUS_SUCCESS;
	if (!SUCCESS ( status = unicap_register_callback (handle, UNICAP_EVENT_NEW_FRAME, (unicap_callback_t) new_frame_cb, (void *) this) ) )
		ofLog(OF_LOG_ERROR,"ofUCUtils: error registering callback");
	if (!SUCCESS ( status = unicap_start_capture (handle) ) )
		ofLog(OF_LOG_ERROR,"ofUCUtils: error starting capture: %i,%i",status,STATUS_INVALID_HANDLE);
}


//--------------------------------------------------------------------
void ofUCUtils::queryUC_imageProperties(void) {

	unicap_property_t property;
	ofLog(OF_LOG_NOTICE,"ofUCUtils : Video settings:");
	const int PPTY_TYPE_MAPPED_FLOAT=UNICAP_PROPERTY_TYPE_UNKNOWN + 1;
	int status = STATUS_SUCCESS;
	int ppty_type;
	for (int i=0; i<MAX_PROPERTIES; i++) {
		status = unicap_enumerate_properties(handle, NULL, &property, i);
		if ( !SUCCESS( status ))
			return;

		status = unicap_get_property(handle, &property );
		if ( !SUCCESS( status )) {
			ofLog(OF_LOG_ERROR,"ofUCUtils : Error getting %s value\n", property.identifier);
			return;
		}

		ppty_type = property.type;
		switch (ppty_type) {
		case PPTY_TYPE_MAPPED_FLOAT:
		case UNICAP_PROPERTY_TYPE_RANGE:
		case UNICAP_PROPERTY_TYPE_VALUE_LIST:
			if (property.value>0 && property.value<1) {
				ofLog(OF_LOG_NOTICE,"\t%s: 1/%.0f \n", property.identifier, 1/property.value);
			} else {
				ofLog(OF_LOG_NOTICE,"\t%s: %.2f \n", property.identifier, property.value);
			}
			break;
		case UNICAP_PROPERTY_TYPE_MENU:
			printf("\t%s: %s \n", property.identifier, property.menu_item);
			break;
		default:
			break;
		}

	}

}

//--------------------------------------------------------------------
void ofUCUtils::new_frame (unicap_data_buffer_t * buffer)
{
	if(!deviceReady)
		return;

	if(doConversion){
		avpicture_fill(src,buffer->data,src_pix_fmt,format.size.width,format.size.height);

		if(sws_scale(toRGB_convert_ctx,
			src->data, src->linesize, 0, buffer->format.size.height,
			dst->data, dst->linesize)<0)
				ofLog(OF_LOG_ERROR,"ofUCUtils: can't convert colorspaces");

		lock_buffer();
			avpicture_layout(dst,PIX_FMT_RGB24,d_width,d_height,pixels,d_width*d_height*3);

	}else{
		lock_buffer();
			pixels=buffer->data;
	}
	bUCFrameNew = true;
	unlock_buffer();
}


//--------------------------------------------------------------------
bool ofUCUtils::getFrameUC(unsigned char ** _pixels) {
	if(bUCFrameNew){
		lock_buffer();
		bUCFrameNew = false;
		memcpy(*_pixels,pixels,d_width*d_height*3);
		//*_pixels=pixels;
		unlock_buffer();
		return true;
	}else{
		return false;
	}
}

//--------------------------------------------------------------------
void ofUCUtils::listUCDevices() {

	int status = STATUS_SUCCESS;
	int dev_count;
	unicap_device_t devices[MAX_DEVICES];
	printf("listing available capture devices\n");
	printf("---\n");
	for (dev_count = 0; SUCCESS (status) && (dev_count < MAX_DEVICES); dev_count++) {
		status = unicap_enumerate_devices (NULL, &devices[dev_count], dev_count);
		if (SUCCESS (status))
			printf ("Video device %d: %s\n", dev_count,
					devices[dev_count].identifier);
	}
	printf("---\n");

}

void ofUCUtils::close_unicap() {

	if(!deviceReady)
		return;

	unicap_stop_capture(handle);
	bUCFrameNew=false;

	if( src_pix_fmt != PIX_FMT_RGB24 ){

		if ( dst != NULL ){
			avpicture_free(dst);
			delete dst;
		}

		if ( pixels != NULL ) {
			delete[] pixels;
		}

		if ( src != NULL ){
			//avpicture_free(src);
			delete src;
		}

	}
	deviceReady=false;

}


//--------------------------------------------------------------------
void ofUCUtils::lock_buffer(){
	pthread_mutex_lock( &capture_mutex );
}

//--------------------------------------------------------------------
void ofUCUtils::unlock_buffer(){
	pthread_mutex_unlock( &capture_mutex );
}

#endif
