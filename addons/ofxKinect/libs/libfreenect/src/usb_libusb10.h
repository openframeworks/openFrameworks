/*
 * This file is part of the OpenKinect Project. http://www.openkinect.org
 *
 * Copyright (c) 2010 individual OpenKinect contributors. See the CONTRIB file
 * for details.
 *
 * This code is licensed to you under the terms of the Apache License, version
 * 2.0, or, at your option, the terms of the GNU General Public License,
 * version 2.0. See the APACHE20 and GPL2 files for the text of the licenses,
 * or the following URLs:
 * http://www.apache.org/licenses/LICENSE-2.0
 * http://www.gnu.org/licenses/gpl-2.0.txt
 *
 * If you redistribute this file in source form, modified or unmodified, you
 * may:
 *   1) Leave this header intact and distribute it under the same terms,
 *      accompanying it with the APACHE20 and GPL20 files, or
 *   2) Delete the Apache 2.0 clause and accompany it with the GPL2 file, or
 *   3) Delete the GPL v2 clause and accompany it with the APACHE20 file
 * In all cases you must keep the copyright notice intact and include a copy
 * of the CONTRIB file.
 *
 * Binary distributions must follow the binary distribution requirements of
 * either License.
 */

#pragma once

#include "libfreenect.h"
#include <libusb.h>

// There are a few rules: PKTS_PER_XFER * NUM_XFERS <= 1000, PKTS_PER_XFER % 8 == 0.
#if defined(__APPLE__)
  #define DEPTH_PKTBUF 2048
  #define VIDEO_PKTBUF 2048

  #define PKTS_PER_XFER 128
  #define NUM_XFERS 4
#else
  #define DEPTH_PKTBUF 1920
  #define VIDEO_PKTBUF 1920

  #if defined(_WIN32)
    #define PKTS_PER_XFER 32
    #define NUM_XFERS 8
  #else
    #define PKTS_PER_XFER 16
    #define NUM_XFERS 16
  #endif
#endif

typedef struct {
	libusb_context *ctx;
	int should_free_ctx;
} fnusb_ctx;

typedef struct {
	freenect_device *parent; //so we can go up from the libusb userdata
	libusb_device_handle *dev;
	int device_dead; // set to 1 when the underlying libusb_device_handle vanishes (ie, Kinect was unplugged)
	int VID;
	int PID;
} fnusb_dev;

typedef struct {
	fnusb_dev *parent; //so we can go up from the libusb userdata
	struct libusb_transfer **xfers;
	uint8_t *buffer;
	fnusb_iso_cb cb;
	int num_xfers;
	int pkts;
	int len;
	int dead;
	int dead_xfers;
} fnusb_isoc_stream;

int fnusb_num_devices(freenect_context *ctx);
int fnusb_list_device_attributes(freenect_context *ctx, struct freenect_device_attributes** attribute_list);

int fnusb_init(fnusb_ctx *ctx, freenect_usb_context *usb_ctx);
int fnusb_shutdown(fnusb_ctx *ctx);
int fnusb_process_events(fnusb_ctx *ctx);
int fnusb_process_events_timeout(fnusb_ctx *ctx, struct timeval* timeout);

int fnusb_open_subdevices(freenect_device *dev, int index);
int fnusb_close_subdevices(freenect_device *dev);

int fnusb_start_iso(fnusb_dev *dev, fnusb_isoc_stream *strm, fnusb_iso_cb cb, unsigned char endpoint, int xfers, int pkts, int len);
int fnusb_stop_iso(fnusb_dev *dev, fnusb_isoc_stream *strm);
int fnusb_get_max_iso_packet_size(fnusb_dev *dev, unsigned char endpoint, int default_size);

int fnusb_control(fnusb_dev *dev, uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, uint8_t *data, uint16_t wLength);
int fnusb_bulk(fnusb_dev *dev, uint8_t endpoint, uint8_t *data, int len, int *transferred);
int fnusb_num_interfaces(fnusb_dev *dev);
