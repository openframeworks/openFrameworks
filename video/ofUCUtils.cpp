#include "ofUCUtils.h"
#include <stdio.h>
#include <stdint.h>

extern "C"
{
#include <avcodec.h>
}

#define FOURCC(a,b,c,d) (unsigned int)((((unsigned int)a))+(((unsigned int)b)<<8)+(((unsigned int)c)<<16)+(((unsigned int)d)<<24))

typedef struct PixelFormatTag {
    int pix_fmt;
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
    { PIX_FMT_YUV422,  	FOURCC('Y', 'Y', 'U', 'V') },
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

    { -1, 0 },
};



//--------------------------------------------------------------------
int fourcc_to_pix_fmt( unsigned int fourcc)
{
	const PixelFormatTag * tags = pixelFormatTags;
	while (tags->pix_fmt >= 0) {
		if (tags->fourcc == fourcc)
			return tags->pix_fmt;
        tags++;
    }
    return -1;
}


//--------------------------------------------------------------------
ofUCUtils::ofUCUtils(){

	verbose			= false;
	bUCFrameNew		= false;
	returned_buffer	= NULL;
	src				= NULL;
	dst				= NULL;
}


//--------------------------------------------------------------------
ofUCUtils::~ofUCUtils(){

	unicap_stop_capture(handle);
	if (buffer.data != NULL) {
		free(buffer.data);
	}
	if (src != NULL){
		avpicture_free(src);
		delete src;
	}
	if (dst != NULL){
		avpicture_free(dst);
		delete dst;
	}
}


//--------------------------------------------------------------------
void ofUCUtils::open_device(int d) {

	if (!SUCCESS(unicap_enumerate_devices (NULL, &device, d))) {
		printf ("Unicap : Error selecting device %d\n", d);
	} else {
		if (!SUCCESS(unicap_open (&handle, &device))) {
			printf ("Unicap : Error opening device %d: %s\n", d,
					device.identifier);
		}
	}
	if(verbose){
	    printf("Unicap : Using device %s \n",device.device);
	    printf("Unicap : Using module %s\n",device.vendor_name);
	}

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

	unicap_format_t formats[MAX_FORMATS];
	int format_count;
	unicap_status_t status = STATUS_SUCCESS;
	int rgb24 = -1;

	if(verbose) printf("Unicap : Available formats for this device:\n");
	for (format_count = 0; SUCCESS (status) && (format_count < MAX_FORMATS); format_count++) {
		status = unicap_enumerate_formats (handle, NULL, &formats[format_count], format_count);
		if (SUCCESS (status)) {
			if (formats[format_count].bpp == 24) {
				rgb24 = format_count;
			}
			if(verbose) printf (
					"Unicap : %d: %s, min size: %dx%d, max size:%dx%d, default size: %dx%d\n",
					format_count, formats[format_count].identifier,
					formats[format_count].min_size.width,
					formats[format_count].min_size.height,
					formats[format_count].max_size.width,
					formats[format_count].max_size.height,
					formats[format_count].size.width,
					formats[format_count].size.height);
		}
	}

	if (format_count > 0) {
		int selected_format = 0;
		if (rgb24 != -1)
			selected_format = rgb24;
		format = formats[selected_format];

		bool sizeFounded = true;
		bool exactMatch  = false;
		if(w == format.size.width && h == format.size.height){
			exactMatch = true;
		}else if(w <= format.min_size.width && h <= format.min_size.height){
			format.size.width  = format.min_size.width;
			format.size.height = format.min_size.height;
		}else if(w >= format.max_size.width && h >= format.max_size.height){
			format.size.width  = format.max_size.width;
			format.size.height = format.max_size.height;
		}else{
			sizeFounded=false;
		}

		if(sizeFounded){
			if(verbose && !exactMatch)
				printf ("Unicap : Can't set video format %s, with size %dx%d\n",
								format.identifier, w, h);

			if ( !SUCCESS ( unicap_set_format (handle, &format) ) ) {
				printf ("Unicap : Failed to set alternative video format!\n");
				return;
			}
		}else{
			format.size.width  = w;
			format.size.height = h;

			//Try selected size
			if (!SUCCESS (unicap_set_format (handle, &format))) {
				printf ("Unicap : Can't set video format %s, with size %dx%d\n",
						format.identifier, w, h);


				// If selected size doesn't work try to find a supported one
				unicap_format_t format_spec;
				unicap_void_format(&format_spec);

				int nearW				= 9999999;
				int nearH				= 9999999;


				//Try with unicap reported sizes
				if(format.size_count > 0){
					if(verbose)printf ("Unicap : Available sizes: %d\n",format.size_count);

					for(int i = 0; i < format.size_count; i++){
						if(verbose) printf ("%d,%d\n",format.sizes[i].width,format.sizes[i].height);
						if(abs(w-format.sizes[i].width)<abs(w-nearW)){
							nearW = format.sizes[i].width;
							nearH = format.sizes[i].height;
						}
					}
					format.size.width  = nearW;
		            format.size.height = nearH;

		        //Try with stepping
				}else if(format.h_stepping > 1 || format.v_stepping > 1){
					//This is how many diff sizes are available for the format
		            int stepX = format.h_stepping;
		            int stepY = format.v_stepping;
		            for(int x = format.min_size.x; x <= format.max_size.x; x+= stepX)
		            {
		            	if( abs(w-x) < abs(w-nearW) ){
		            		nearW = x;
		            	}
		            }

		            for(int y = format.min_size.y; y <= format.max_size.y; y+= stepY)
		            {
		            	if( abs(h-y) < abs(h-nearH) ){
		            		nearH = y;
		            	}
		            }
		            format.size.width  = nearW;
		            format.size.height = nearH;
				}

				//Try to set founded size
				sizeFounded = SUCCESS ( unicap_set_format (handle, &format) );

				//If none of the above work, try default size
				if(!sizeFounded){
	       			if ( !SUCCESS( unicap_enumerate_formats( handle, &format_spec, &format, selected_format ) ) ) {
						printf("Unicap : Failed to get alternative video format\n");
						return;
					}

					if ( !SUCCESS ( unicap_set_format (handle, &format) ) ) {
						printf ("Unicap : Failed to set alternative video format!\n");
						return;
					}
				}
			}

		}
		if(verbose) printf("Unicap : Selected format: %s, with size %dx%d\n", format.identifier,
				format.size.width, format.size.height);

		src_pix_fmt=fourcc_to_pix_fmt(format.fourcc);
		if( src_pix_fmt==-1){
			printf("Unicap : Format not suported\n");
			return;
		}

		if(src_pix_fmt!=PIX_FMT_RGB24){
			src=new AVPicture;
			avpicture_alloc(src,src_pix_fmt,format.size.width,format.size.height);
			dst=new AVPicture;
			avpicture_alloc(dst,PIX_FMT_RGB24,format.size.width,format.size.height);
			printf("Converting to RGB24");
		}
	}
}


//--------------------------------------------------------------------
void ofUCUtils::start_capture() {

	 // Allocate memory for the image buffer
	if ( !(buffer.data = (unsigned char *)malloc(format.buffer_size) )) {
		printf("Unicap : Failed to allocate %d bytes\n", format.buffer_size);
		return;
	}
	buffer.buffer_size = format.buffer_size;
	if ( !SUCCESS( unicap_start_capture( handle ) )) {
		printf("Unicap : Failed to start capture\n");
	}

}


//--------------------------------------------------------------------
void ofUCUtils::queryUC_imageProperties(void) {

	unicap_property_t property;
	printf("Unicap : Video settings:\n");
	const int PPTY_TYPE_MAPPED_FLOAT=UNICAP_PROPERTY_TYPE_UNKNOWN + 1;
	int status = STATUS_SUCCESS;
	int ppty_type;
	for (int i=0; i<MAX_PROPERTIES; i++) {
		status = unicap_enumerate_properties(handle, NULL, &property, i);
		if ( !SUCCESS( status ))
			return;

		status = unicap_get_property(handle, &property );
		if ( !SUCCESS( status )) {
			printf("Unicap : Error getting %s value\n", property.identifier);
			return;
		}

		ppty_type = property.type;
		switch (ppty_type) {
		case PPTY_TYPE_MAPPED_FLOAT:
		case UNICAP_PROPERTY_TYPE_RANGE:
		case UNICAP_PROPERTY_TYPE_VALUE_LIST:
			if (property.value>0 && property.value<1) {
				printf("\t%s: 1/%.0f \n", property.identifier, 1/property.value);
			} else {
				printf("\t%s: %.2f \n", property.identifier, property.value);
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
bool ofUCUtils::getFrameUC(unsigned char ** _pixels) {

	if ( !SUCCESS( unicap_queue_buffer( handle, &buffer ) )) {
		printf("Unicap : Failed to queue a buffer\n");
		return false;
	}
	/*
	 Wait until the image buffer is ready
	 */
	if ( !SUCCESS( unicap_wait_buffer( handle, &returned_buffer ) )) {
		printf("Unicap : Failed to wait for buffer\n");
		return false;
	}

	if(src_pix_fmt!=PIX_FMT_RGB24){
		avpicture_fill(src,returned_buffer->data,src_pix_fmt,format.size.width,format.size.height);
		img_convert(dst,PIX_FMT_RGB24,src,src_pix_fmt,format.size.width,format.size.height);
		avpicture_layout(dst,PIX_FMT_RGB24,format.size.width,format.size.height,*_pixels,format.size.width*format.size.height*3);
	}else{
		*_pixels = returned_buffer->data;
	}
	return true;

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

	unicap_stop_capture(handle);
	if (buffer.data != NULL) {
		free(buffer.data);
	}

	if (src != NULL){
		avpicture_free(src);
		delete src;
	}
	if (dst != NULL){
		avpicture_free(dst);
		delete dst;
	}
}


//--------------------------------------------------------------------
int ofUCUtils::getUC_Height(void) {

	return format.size.height;

}


//--------------------------------------------------------------------
int ofUCUtils::getUC_Width(void) {

	return format.size.width;

}

