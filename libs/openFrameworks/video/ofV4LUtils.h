/*
 * Revised openFrameworks v4L code
 * 
 * This version uses openCV conversions of YUV420/YUV420P and YUV411P colour models, 
 * therefore the license agreement must accompany this file.
 * Dec 9 2007
 * Pierre Proske
 */

////////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//

#ifndef _V4L_UTILS
#define _V4L_UTILS

#include "ofConstants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <time.h>
//#include <asm/types.h>          /* for videodev2.h */
#include <linux/videodev.h>

#define HAVE_V4L	1

/* V4L structures */
static struct video_capability capability;
static struct video_window     window;
static struct video_picture    imageProperties; 
static struct video_mbuf       mbuf;
static struct video_mmap       vmmap;

static int 					deviceHandle;    
static int 					frameIndex;    
static bool					bFirstFrame;
static unsigned char 			*bigbuf;	

/* V4L functions */
bool openV4L_device(const char *device_name);
void initialiseV4L_device(const char *device_name);
bool initV4L(int width, int height,const char *devname);
bool queryV4L_imageProperties(void);
bool setV4L_palette(int palette, int depth);
bool setV4L_imageProperties(void);
bool setV4L_videoSize(int width, int height);
bool getFrameV4L(unsigned char * pixels);
void closeV4L(void);
bool mmapV4L(void);
int getV4L_Height();
int getV4L_Width();

/*
 * Turn a YUV4:2:0 block into an RGB block
 *
 * Video4Linux order convention-- rgb[0] is blue, rgb[1] is green, rgb[2] is red.
 *
 * Y values are given for all 4 pixels, but the U (Pb)
 * and V (Pr) are assumed constant over the 2x2 block.
 *
 * To avoid floating point arithmetic, the color conversion
 * coefficients are scaled into 16.16 fixed-point integers.
 * They were determined as follows:
 *
 *  double brightness = 1.0;  (0->black; 1->full scale) 
 *  double saturation = 1.0;  (0->greyscale; 1->full color)
 *  double fixScale = brightness * 256 * 256;
 *  int rvScale = (int)(1.402 * saturation * fixScale);
 *  int guScale = (int)(-0.344136 * saturation * fixScale);
 *  int gvScale = (int)(-0.714136 * saturation * fixScale);
 *  int buScale = (int)(1.772 * saturation * fixScale);
 *  int yScale = (int)(fixScale);   
 */

/* LIMIT: convert a 16.16 fixed-point value to a byte, with clipping. */
#define LIMIT(x) ((x)>0xffffff?0xff: ((x)<=0xffff?0:((x)>>16)))

static inline void
move_420_block(int yTL, int yTR, int yBL, int yBR, int u, int v, int rowPixels, unsigned char * rgb)
{
    const int rvScale = 91881;
    const int guScale = -22553;
    const int gvScale = -46801;
    const int buScale = 116129;
    const int yScale  = 65536;
    int r, g, b;

    g = guScale * u + gvScale * v;
    r = rvScale * v;
    b = buScale * u;

    yTL *= yScale; yTR *= yScale;
    yBL *= yScale; yBR *= yScale;

    /* Write out top two pixels */
    rgb[0] = LIMIT(b+yTL); rgb[1] = LIMIT(g+yTL);
    rgb[2] = LIMIT(r+yTL);

    rgb[3] = LIMIT(b+yTR); rgb[4] = LIMIT(g+yTR);
    rgb[5] = LIMIT(r+yTR);

    /* Skip down to next line to write out bottom two pixels */
    rgb += 3 * rowPixels;
    rgb[0] = LIMIT(b+yBL); rgb[1] = LIMIT(g+yBL);
    rgb[2] = LIMIT(r+yBL);

    rgb[3] = LIMIT(b+yBR); rgb[4] = LIMIT(g+yBR);
    rgb[5] = LIMIT(r+yBR);
}

static inline void
move_411_block(int yTL, int yTR, int yBL, int yBR, int u, int v, 
           int rowPixels, unsigned char * rgb)
{
    const int rvScale = 91881;
    const int guScale = -22553;
    const int gvScale = -46801;
    const int buScale = 116129;
    const int yScale  = 65536;
    int r, g, b;

    g = guScale * u + gvScale * v;
    r = rvScale * v;
    b = buScale * u;

    yTL *= yScale; yTR *= yScale;
    yBL *= yScale; yBR *= yScale;

    /* Write out top two first pixels */
    rgb[0] = LIMIT(b+yTL); rgb[1] = LIMIT(g+yTL);
    rgb[2] = LIMIT(r+yTL);

    rgb[3] = LIMIT(b+yTR); rgb[4] = LIMIT(g+yTR);
    rgb[5] = LIMIT(r+yTR);

    /* Write out top two last pixels */
    rgb += 6;
    rgb[0] = LIMIT(b+yBL); rgb[1] = LIMIT(g+yBL);
    rgb[2] = LIMIT(r+yBL);

    rgb[3] = LIMIT(b+yBR); rgb[4] = LIMIT(g+yBR);
    rgb[5] = LIMIT(r+yBR);
}

// Consider a YUV420P image of 8x2 pixels.
//
// A plane of Y values    A B C D E F G H
//                        I J K L M N O P
//
// A plane of U values    1   2   3   4 
// A plane of V values    1   2   3   4 ....
//
// The U1/V1 samples correspond to the ABIJ pixels.
//     U2/V2 samples correspond to the CDKL pixels.
//
/* Converts from planar YUV420P to RGB24. */
static void 
yuv420p_to_rgb24(int width, int height, unsigned char *pIn0, unsigned char *pOut0)
{
    const int numpix = width * height;
    const int bytes = 24 >> 3;
    int i, j, y00, y01, y10, y11, u, v;
    unsigned char *pY = pIn0;
    unsigned char *pU = pY + numpix;
    unsigned char *pV = pU + numpix / 4;
    unsigned char *pOut = pOut0;

    for (j = 0; j <= height - 2; j += 2) {
        for (i = 0; i <= width - 2; i += 2) {
            y00 = *pY;
            y01 = *(pY + 1);
            y10 = *(pY + width);
            y11 = *(pY + width + 1);
            u = (*pU++) - 128;
            v = (*pV++) - 128;

            move_420_block(y00, y01, y10, y11, u, v,
                       width, pOut);
    
            pY += 2;
            pOut += 2 * bytes;

        }
        pY += width;
        pOut += width * bytes;
    }
}

// Consider a YUV411P image of 8x2 pixels.
//
// A plane of Y values as before.
//
// A plane of U values    1       2
//                        3       4
//
// A plane of V values    1       2
//                        3       4
//
// The U1/V1 samples correspond to the ABCD pixels.
//     U2/V2 samples correspond to the EFGH pixels.
//
/* Converts from planar YUV411P to RGB24. */
/* untested... */
static void 
yuv411p_to_rgb24(int width, int height,
           unsigned char *pIn0, unsigned char *pOut0)
{
    const int numpix = width * height;
    const int bytes = 24 >> 3;
    int i, j, y00, y01, y10, y11, u, v;
    unsigned char *pY = pIn0;
    unsigned char *pU = pY + numpix;
    unsigned char *pV = pU + numpix / 4;
    unsigned char *pOut = pOut0;

    for (j = 0; j <= height; j++) {
        for (i = 0; i <= width - 4; i += 4) {
            y00 = *pY;
            y01 = *(pY + 1);
            y10 = *(pY + 2);
            y11 = *(pY + 3);
            u = (*pU++) - 128;
            v = (*pV++) - 128;

            move_411_block(y00, y01, y10, y11, u, v,
                       width, pOut);
    
            pY += 4;
            pOut += 4 * bytes;

        }
    }
}


static void errno_exit (const char *s)
{
	printf ("%s error %d, %s\n",s, errno, strerror (errno));
	exit (EXIT_FAILURE);
}

static int xioctl (int deviceHandle, int request, void *arg)
{
	int r;

	do r = ioctl (deviceHandle, request, arg);
	while (-1 == r && EINTR == errno);

	return r;
}

bool openV4L_device(const char *device_name)
{
	struct stat st; 

	if (stat (device_name, &st) == -1) {
			printf("V4L : Cannot identify '%s': %d, %s\n", device_name, errno, strerror (errno));
			exit (EXIT_FAILURE);
	}

	if (!S_ISCHR (st.st_mode)) {
			printf("V4L : %s is no device\n", device_name);
			exit (EXIT_FAILURE);
	}

	deviceHandle = open(device_name, O_RDWR | O_NONBLOCK, 0);

	if (deviceHandle == -1) {
		printf("V4L : Cannot open '%s': %d, %s\n", device_name, errno, strerror (errno));
		close(deviceHandle);
		exit (EXIT_FAILURE);
	}
	
	return true;
}

bool queryV4L_imageProperties(void) {

	memset(&imageProperties, 0, sizeof(struct video_picture));	// clear v4l image properties
	
	if (xioctl(deviceHandle, VIDIOCGPICT, &imageProperties) == -1) {
		printf("V4L : Unable to determine image properties\n");
		errno_exit ("VIDIOCGPICT");
	}			

	printf("V4L : Brightness\t= %u\n",imageProperties.brightness);
	printf("V4L : Hue\t\t= %u\n",imageProperties.hue);	
	printf("V4L : Colour\t\t= %u\n",imageProperties.colour);
	printf("V4L : Depth\t\t= %u bits\n",imageProperties.depth);
	printf("V4L : Palette\t\t= ");
	
	switch(imageProperties.palette) {
		case VIDEO_PALETTE_GREY: // 1
			printf("VIDEO_PALETTE_GREY\n");
			break;
		case VIDEO_PALETTE_HI240: // 2
			printf("VIDEO_PALETTE_HI240\n");
			break;
		case VIDEO_PALETTE_RGB565:  // 3
			printf("VIDEO_PALETTE_RGB565\n");
			break;	
		case VIDEO_PALETTE_RGB24: // 4
			printf("VIDEO_PALETTE_RGB24\n");
			break;			
		case VIDEO_PALETTE_RGB32: // 5
			printf("VIDEO_PALETTE_RGB32\n");
			break;
		case VIDEO_PALETTE_RGB555:	// 6
			printf("VIDEO_PALETTE_RGB555\n");
			break;	
		case VIDEO_PALETTE_YUV422: // 7
			printf("VIDEO_PALETTE_YUV422\n");
			break;		
		case VIDEO_PALETTE_YUYV: // 8
			printf("VIDEO_PALETTE_YUYV\n");
			break;	
		case VIDEO_PALETTE_UYVY: // 9
			printf("VIDEO_PALETTE_UYVY\n");
			break;			
		case VIDEO_PALETTE_YUV420: // 10
			printf("VIDEO_PALETTE_YUV420\n");
			break;		
		case VIDEO_PALETTE_YUV411: // 11
			printf("VIDEO_PALETTE_YUV411\n");
			break;	
		case VIDEO_PALETTE_RAW:	// 12
			printf("VIDEO_PALETTE_RAW\n");
			break;	
		case VIDEO_PALETTE_YUV422P:	// 13
			printf("VIDEO_PALETTE_YUV422P\n");
			break;
		case VIDEO_PALETTE_YUV411P:	// 14
			printf("VIDEO_PALETTE_YUV411P\n");
			break;	
		case  VIDEO_PALETTE_YUV420P: // 15		
			printf("VIDEO_PALETTE_YUV420P\n");
			break;
		case  VIDEO_PALETTE_YUV410P: // 16		
			printf("VIDEO_PALETTE_YUV410P\n");
			break;			
		default:
			printf(" Couldn't read palette\n");
			break;
	}
	return true;
}

bool setV4L_palette(int palette, int depth) {
	imageProperties.palette = palette;
	imageProperties.depth = depth;
  
	if (xioctl(deviceHandle, VIDIOCSPICT, &imageProperties) < 0) {
		printf("V4L : Failed to set image properties : %d, %s\n", errno, strerror (errno));
		return false;
	}
	if ((imageProperties.palette == palette) && (imageProperties.depth == depth)) {
		return true;
	}
	return false;
}

bool setV4L_imageProperties(void) {

	if (setV4L_palette(VIDEO_PALETTE_RGB24, 24)) {
		printf("V4L : Changed current palette to VIDEO_PALETTE_RGB24\n");
	}
	else 
	if (setV4L_palette(VIDEO_PALETTE_YUV420, 16)) {
		printf("V4L : Changed current palette to VIDEO_PALETTE_YUV420\n");
	}
	else 
	if (setV4L_palette(VIDEO_PALETTE_YUV420P, 16)) {
		printf("V4L : Changed current palette to VIDEO_PALETTE_YUV420P\n");
	}
	else 
	if (setV4L_palette(VIDEO_PALETTE_YUV411P, 16)) {
		printf("V4L : Changed current palette to VIDEO_PALETTE_YUV411P\n");
	}	
	else {
		printf("V4L : ERROR : Unable to change to a suitable palette\n");
		return false;
	}
	return true;
}

void initialiseV4L_device(const char *device_name) {

	if (xioctl(deviceHandle, VIDIOCGCAP, &capability) == -1) {
		if (errno == EINVAL) {
			printf("V4L : %s is no V4L device\n",device_name);
			exit (EXIT_FAILURE);
		} else {
			errno_exit ("VIDIOC_QUERYCAP");
		}
	}

	if (!(capability.type & VID_TYPE_CAPTURE)) {
		printf("V4L : %s is no video capture device\n", device_name);
		exit (EXIT_FAILURE);
	} else {
		printf("V4L : Name = '%s'\n",capability.name);	
		printf("V4L : Dimensions (%i x %i) - (%i x %i)\n", capability.minwidth, capability.minheight, capability.maxwidth,capability.maxheight);	
		printf("V4L : Capability :\n");
		if (capability.type & VID_TYPE_CAPTURE      ) printf(" - CAPTURE\n");
		if (capability.type & VID_TYPE_TUNER        ) printf(" - TUNER\n");
		if (capability.type & VID_TYPE_TELETEXT     ) printf(" - TELETEXT\n");
		if (capability.type & VID_TYPE_OVERLAY      ) printf(" - OVERLAY\n");
		if (capability.type & VID_TYPE_CHROMAKEY    ) printf(" - CHROMAKEY\n");
		if (capability.type & VID_TYPE_CLIPPING     ) printf(" - CLIPPING\n");
		if (capability.type & VID_TYPE_FRAMERAM     ) printf(" - FRAMERAM\n");
		if (capability.type & VID_TYPE_SCALES       ) printf(" - SCALES\n");
		if (capability.type & VID_TYPE_MONOCHROME   ) printf(" - MONOCHROME\n");
		if (capability.type & VID_TYPE_SUBCAPTURE   ) printf(" - SUBCAPTURE\n");
	}

	queryV4L_imageProperties();
	setV4L_imageProperties();
}

void writePPM(unsigned char *data, char *filename, int width, int height) {
  int num;
  int size = width * height * 3;
  FILE *fp = fopen(filename, "w");

  if (!fp) {printf("V4L : cannot open file for writing!\n");}

  fprintf(fp, "P6\n%d %d\n%d\n", width, height, 255);
  num = fwrite((void *) data, 1, (size_t) size, fp);

  if (num != size) {printf("V4L : cannot write image data to file\n");}

  fclose(fp);
}

void swap_rgb24(unsigned char *mem, int n)
{
  unsigned char c;
  unsigned char *p = mem;
  int i = n;
  while(--i) {
    c = p[0];
    p[0] = p[2];
    p[2] = c;
    p += 3;
  }
}

bool getFrameV4L(unsigned char * pixels) {

	/* Check space for grabbing RGB data */
	if (pixels == NULL) {
		printf("V4L : ERROR : pixel data not not allocated.\n");
		return false;
	}
	
	if (bFirstFrame) { 
		bFirstFrame = false;
		/* Queue all available buffers for capture */
		for (frameIndex = 0;frameIndex < (mbuf.frames);++frameIndex) {
			vmmap.frame  = frameIndex;
			vmmap.width  = window.width;
			vmmap.height = window.height;
			vmmap.format = imageProperties.palette;
			
			if (xioctl(deviceHandle, VIDIOCMCAPTURE, &vmmap) == -1) {
				printf("V4L : ERROR: Could not make initial capture\n");
				return false;
			}
		}   
		/* reset frame index*/
		frameIndex = 0;   		
	}   
	 
	/* Sync to video */
	if (xioctl(deviceHandle, VIDIOCSYNC, &frameIndex) == -1) {
		printf("V4L : ERROR: VIDIOCSYNC failed. %s\n", strerror(errno));
	}
	
	/* Save current frame */
	switch(imageProperties.palette) {
		case VIDEO_PALETTE_RGB24:
	  	{
	  		int imagesize = window.width*window.height*(imageProperties.depth/8);
			memcpy((unsigned char*)pixels, (unsigned char*)(bigbuf + mbuf.offsets[frameIndex]),imagesize);
			swap_rgb24(pixels, window.width * window.height);
		}
		break;
		case VIDEO_PALETTE_YUV420P:			
		{
			// convert yuv420p_to_rgb24
			yuv420p_to_rgb24(window.width,window.height,(unsigned char*)(bigbuf + mbuf.offsets[frameIndex]),(unsigned char*) pixels);
			swap_rgb24(pixels, window.width * window.height);
		}
		break;
		case VIDEO_PALETTE_YUV420:			
		{
			// convert yuv420_to_rgb24....yuv420 is the same as yuv420p from what I've gathered...
			yuv420p_to_rgb24(window.width,window.height,(unsigned char*)(bigbuf + mbuf.offsets[frameIndex]),(unsigned char*) pixels);
			swap_rgb24(pixels, window.width * window.height);
		}
		break;
		case VIDEO_PALETTE_YUV411P:			
		{
			// convert yuv411p_to_rgb24
			yuv411p_to_rgb24(window.width,window.height,(unsigned char*)(bigbuf + mbuf.offsets[frameIndex]),(unsigned char*) pixels);
			swap_rgb24(pixels, window.width * window.height);
		}
		break;				
	  default:
		printf("V4L : ERROR: Cannot convert from palette %d to RGB\n",imageProperties.palette);
		return false;
	}	
	
	vmmap.frame  = frameIndex;
	vmmap.width  = window.width;
	vmmap.height = window.height;
	vmmap.format = imageProperties.palette;

	/* Capture video */
	if (xioctl (deviceHandle, VIDIOCMCAPTURE, &vmmap) == -1) {
		printf("V4L : ERROR: Could not capture...\n");
		return false;
	}

	++frameIndex;
	if (frameIndex == mbuf.frames) {
		frameIndex = 0;
	}
	
	return true;
}

bool setV4L_videoSize(int w, int h) {
  
	if (!(capability.type & VID_TYPE_CAPTURE))  {
		printf("V4L : Not a capture device! Exiting...\n");
		return false;
	}
	
	if (xioctl(deviceHandle, VIDIOCGWIN, &window) < 0) {
		printf("V4L : Could not get video size : %d, %s\n", errno, strerror (errno));
		return false;
	}
		
	if (w > capability.maxwidth) w = capability.maxwidth;
	if (h > capability.maxheight) h = capability.maxheight;

	window.width = w;
	window.height= h;
	window.x = window.y = window.chromakey = window.flags = 0;

	if (xioctl(deviceHandle, VIDIOCSWIN, &window) < 0) {
		printf("V4L : ERROR : Could not set video size : %d, %s\n", errno, strerror (errno));
		return false;
	}
	/* Get the video capabilities to check that the set worked */
	if (xioctl(deviceHandle, VIDIOCGWIN, &window) < 0) {
		printf("V4L : Could not get video size : %d, %s\n", errno, strerror (errno));
		return false;
	}	
	printf("V4L : Video size changed to width=%i height=%i\n",window.width,window.height); 	

	return true;
}

bool mmapV4L(void) {

	if (xioctl(deviceHandle, VIDIOCGMBUF, &mbuf) < 0) {
		printf("V4L : Could not use mmap : %d, %s\n", errno, strerror (errno));
	} 				
    
	/* Get memory mapped io */
	/* create a large buffer that can hold up to the maximum frames at the maximum resolution*/
	bigbuf = (unsigned char*) mmap(0, mbuf.size, PROT_READ | PROT_WRITE, MAP_SHARED, deviceHandle, 0);

	if (bigbuf == MAP_FAILED) { 
		printf("V4L : Could not use mmap buffer.\n");
		closeV4L();
	}

	return true;
}

void closeV4L(void) {
	printf("V4L : Shutting down....\n");
	close(deviceHandle);		// Close device
    munmap(bigbuf, mbuf.size); 	// Un-mmap memory
}

bool initV4L(int width, int height, const char *devname) {
	bFirstFrame	= true;
	bigbuf = NULL;
			
	openV4L_device(devname);
	initialiseV4L_device(devname);
	if(!setV4L_videoSize(width,height))
		exit(1);		

	mmapV4L();
	
	return true;
}

int getV4L_Height(void) {
	if(window.height)
		return window.height;	
}

int getV4L_Width(void) {
	if(window.width)
		return window.width;	
}

#endif

