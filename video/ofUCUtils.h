#ifndef OFUCUTILS_H_
#define OFUCUTILS_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unicap.h>
#include <unistd.h>
#include <string.h>

struct AVPicture;

//#include <gtk/gtk.h>
//#include <unicapgtk.h>

#define MAX_DEVICES 64
#define MAX_FORMATS 64
#define MAX_PROPERTIES 64



class ofUCUtils{
	
public:
	
	ofUCUtils();
	~ofUCUtils();
	
	void set_format (int w, int h);
	void open_device (int d);
	void start_capture(void);
	void queryUC_imageProperties(void);
	bool getFrameUC(unsigned char ** _pixels);
	void listUCDevices();
	void close_unicap();
	int getUC_Height(void);
	int getUC_Width(void);
	char * device_identifier(void);

	bool verbose;
	
private:	
	bool 					bUCFrameNew;
	unicap_data_buffer_t 	buffer;
	unicap_data_buffer_t *	returned_buffer;
	unicap_device_t 		device;
	unicap_format_t 		format;
	unicap_handle_t 		handle;
	
	AVPicture * 			src;
	AVPicture * 			dst;
	int 					src_pix_fmt;
};

#endif /*OFUCUTILS_H_*/
