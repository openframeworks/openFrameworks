#ifndef OFUCUTILS_H_
#define OFUCUTILS_H_

#include "ofConstants.h"

#ifdef TARGET_LINUX

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unicap.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

struct AVPicture;


#define MAX_DEVICES 64
#define MAX_FORMATS 64
#define MAX_PROPERTIES 64
#define VIDEOGRABBER_RESIZE_FLAGS SWS_X

class ofUCUtils{

public:
	ofUCUtils();
	virtual ~ofUCUtils();

	void set_format (int w, int h);
	bool open_device (int d);
	void start_capture(void);
	void queryUC_imageProperties(void);
	bool getFrameUC(unsigned char ** _pixels);
	void listUCDevices();
	void close_unicap();
	char * device_identifier(void);
	void new_frame (unicap_data_buffer_t * buffer);

	bool verbose;

private:
	bool 					bUCFrameNew;
	unsigned char *			pixels;
	bool					deviceReady;

	unicap_device_t 		device;
	unicap_format_t 		format;
	unicap_handle_t 		handle;

	AVPicture * 			src;
	AVPicture * 			dst;
	int 					src_pix_fmt;
	int						d_width, d_height;
	struct SwsContext *		toRGB_convert_ctx;

	bool					doConversion;

	pthread_mutex_t			capture_mutex;
	void 					lock_buffer();
	void 					unlock_buffer();
};

#endif

#endif /*OFUCUTILS_H_*/
