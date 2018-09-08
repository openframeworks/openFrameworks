/*
 * This file is part of the OpenKinect Project. http://www.openkinect.org
 *
 * Copyright (c) 2010-2011 individual OpenKinect contributors. See the CONTRIB
 * file for details.
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

#include <stdint.h>

#include "libfreenect.h"
#include "libfreenect_registration.h"
#include "libfreenect_audio.h"

#ifdef __ELF__
  #define FN_INTERNAL	__attribute__ ((visibility ("hidden")))
#else
  #define FN_INTERNAL
#endif


typedef void (*fnusb_iso_cb)(freenect_device *dev, uint8_t *buf, int len);

#include "usb_libusb10.h"

// needed to set the led state for non 1414 devices
FN_INTERNAL int fnusb_set_led_alt(libusb_device_handle * dev, freenect_context * ctx, freenect_led_options state);

struct _freenect_context {
	freenect_loglevel log_level;
	freenect_log_cb log_cb;
	fnusb_ctx usb;
	freenect_device_flags enabled_subdevices;
	freenect_device *first;
	int zero_plane_res;
    
    // if you want to load firmware from memory rather than disk
    unsigned char *     fn_fw_nui_ptr;
    unsigned int        fn_fw_nui_size;

    unsigned char *     fn_fw_k4w_ptr;
    unsigned int        fn_fw_k4w_size;
};

#define LL_FATAL FREENECT_LOG_FATAL
#define LL_ERROR FREENECT_LOG_ERROR
#define LL_WARNING FREENECT_LOG_WARNING
#define LL_NOTICE FREENECT_LOG_NOTICE
#define LL_INFO FREENECT_LOG_INFO
#define LL_DEBUG FREENECT_LOG_DEBUG
#define LL_SPEW FREENECT_LOG_SPEW
#define LL_FLOOD FREENECT_LOG_FLOOD


#ifdef _WIN32
#include <stdarg.h>
#include <stdio.h>
void fn_log(freenect_context *ctx, freenect_loglevel level, const char *fmt, ...);
#else
void fn_log(freenect_context *ctx, freenect_loglevel level, const char *fmt, ...) __attribute__ ((format (printf, 3, 4)));
#endif

#define FN_LOG(level, ...) fn_log(ctx, level, __VA_ARGS__)

#define FN_FATAL(...) FN_LOG(LL_FATAL, __VA_ARGS__)
#define FN_ERROR(...) FN_LOG(LL_ERROR, __VA_ARGS__)
#define FN_WARNING(...) FN_LOG(LL_WARNING, __VA_ARGS__)
#define FN_NOTICE(...) FN_LOG(LL_NOTICE, __VA_ARGS__)
#define FN_INFO(...) FN_LOG(LL_INFO, __VA_ARGS__)
#define FN_DEBUG(...) FN_LOG(LL_DEBUG, __VA_ARGS__)
#define FN_SPEW(...) FN_LOG(LL_SPEW, __VA_ARGS__)
#define FN_FLOOD(...) FN_LOG(LL_FLOOD, __VA_ARGS__)

#ifdef FN_BIGENDIAN
static inline uint16_t fn_le16(uint16_t d)
{
	return (d<<8) | (d>>8);
}
static inline uint32_t fn_le32(uint32_t d)
{
	return (d<<24) | ((d<<8)&0xFF0000) | ((d>>8)&0xFF00) | (d>>24);
}
static inline int16_t fn_le16s(int16_t s)
{
	// reinterpret cast to unsigned, use the normal fn_le16, and then reinterpret cast back
	union {
		int16_t s;
		uint16_t u;
	} conversion_union;
	conversion_union.s = s;
	conversion_union.u = fn_le16(conversion_union.u);
	return conversion_union.s;
}
static inline int32_t fn_le32s(int32_t s)
{
	// reinterpret cast to unsigned, use the normal fn_le32, and then reinterpret cast back
	union {
		int32_t s;
		uint32_t u;
	} conversion_union;
	conversion_union.s = s;
	conversion_union.u = fn_le32(conversion_union.u);
	return conversion_union.s;
}
#else
#define fn_le16(x) (x)
#define fn_le32(x) (x)
#define fn_le16s(x) (x)
#define fn_le32s(x) (x)
#endif

#define DEPTH_PKTSIZE 1760
#define VIDEO_PKTSIZE 1920

#define DEPTH_PKTDSIZE (DEPTH_PKTSIZE-12)
#define VIDEO_PKTDSIZE (VIDEO_PKTSIZE-12)

#define VID_MICROSOFT 0x45e
#define PID_NUI_AUDIO 0x02ad
#define PID_NUI_CAMERA 0x02ae
#define PID_NUI_MOTOR 0x02b0
#define PID_K4W_CAMERA 0x02bf
#define PID_KV2_CAMERA 0x02d9 // This identifies the Kinect v2 (XBox One). Use https://github.com/OpenKinect/libfreenect2 instead.

// For K4W: first pid is what it starts out as,
// second is how it appears with lastest firmware from SDK,
// third is from beta SDK firmware ( which is what is unpacked by the fw script and doesn't support motor control )
#define PID_K4W_AUDIO 0x02be
#define PID_K4W_AUDIO_ALT_1 0x02c3
#define PID_K4W_AUDIO_ALT_2 0x02bb

typedef struct {
	int running;
	uint8_t flag;
	int synced;
	uint8_t seq;
	int got_pkts;
	int pkt_num;
	int pkts_per_frame;
	int pkt_size;
	int frame_size;
	int last_pkt_size;
	int valid_pkts;
	unsigned int lost_pkts;
	int valid_frames;
	int variable_length;
	uint32_t last_timestamp;
	uint32_t timestamp;
	int split_bufs;
	void *lib_buf;
	void *usr_buf;
	uint8_t *raw_buf;
	void *proc_buf;
} packet_stream;

typedef struct {
	int running;

	freenect_sample_51* audio_out_ring; // TODO: implement sending user-provided data in callbacks
	int ring_reader_idx; // Index in audio_out_ring of the last sent sample
	int ring_writer_idx; // Index in audio_out_ring of the next sample we haven't received from the client yet

	uint16_t out_window;
	uint8_t out_seq;
	uint8_t out_counter_within_window;
	uint16_t out_weird_timestamp;
	uint8_t out_window_parity;

	uint16_t in_window;
	uint16_t last_seen_window[10];
	uint8_t in_counter;
	int32_t* mic_buffer[4];
	int16_t* cancelled_buffer;
	void* in_unknown;

	// TODO: timestamps
} audio_stream;

typedef struct {
	uint32_t magic;    // 0x80000080
	uint16_t channel;  // Values between 0x1 and 0xa indicate audio channel
	uint16_t len;      // packet length
	uint16_t window;   // timestamp
	uint16_t unknown;  // ???
	int32_t samples[]; // Size depends on len
} audio_in_block;

typedef struct {
	uint16_t window;       // Kinda like a timestamp.
	uint8_t seq;           // Values from 0x00 to 0x7f
	uint8_t weird;         // Has an odd cyclic behavior.
	freenect_sample_51 samples[6];  // Audio samples - 6 samples per transfer
} audio_out_block;

struct _freenect_device {
	freenect_context *parent;
	freenect_device *next;
	void *user_data;

	// Cameras
	fnusb_dev usb_cam;
	fnusb_isoc_stream depth_isoc;
	fnusb_isoc_stream video_isoc;

	freenect_depth_cb depth_cb;
	freenect_video_cb video_cb;
	freenect_chunk_cb depth_chunk_cb;
	freenect_chunk_cb video_chunk_cb;
	freenect_video_format video_format;
	freenect_depth_format depth_format;
	freenect_resolution video_resolution;
	freenect_resolution depth_resolution;

	int cam_inited;
	uint16_t cam_tag;

	packet_stream depth;
	packet_stream video;

	// Registration
	freenect_registration registration;

	// Audio
	fnusb_dev usb_audio;
	fnusb_isoc_stream audio_out_isoc;
	fnusb_isoc_stream audio_in_isoc;

	freenect_audio_in_cb audio_in_cb;
	freenect_audio_out_cb audio_out_cb;

	audio_stream audio;
	uint32_t audio_tag;

	// Motor
	fnusb_dev usb_motor;
	freenect_raw_tilt_state raw_state;
    
	int device_does_motor_control_with_audio;
	int motor_control_with_audio_enabled;
};
