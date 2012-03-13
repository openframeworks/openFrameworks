/* GStreamer
 * Copyright (C) <1999> Erik Walthinsen <omega@cse.ogi.edu>
 * Library       <2001> Thomas Vander Stichele <thomas@apestaart.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <gst/gst.h>

#ifndef __GST_AUDIO_AUDIO_H__
#define __GST_AUDIO_AUDIO_H__

G_BEGIN_DECLS

/* For people that are looking at this source: the purpose of these defines is
 * to make GstCaps a bit easier, in that you don't have to know all of the
 * properties that need to be defined. you can just use these macros. currently
 * (8/01) the only plugins that use these are the passthrough, speed, volume,
 * adder, and [de]interleave plugins. These are for convenience only, and do not
 * specify the 'limits' of GStreamer. you might also use these definitions as a
 * base for making your own caps, if need be.
 *
 * For example, to make a source pad that can output streams of either mono
 * float or any channel int:
 *
 *  template = gst_pad_template_new
 *    ("sink", GST_PAD_SINK, GST_PAD_ALWAYS,
 *    gst_caps_append(gst_caps_new ("sink_int",  "audio/x-raw-int",
 *                                  GST_AUDIO_INT_PAD_TEMPLATE_PROPS),
 *                    gst_caps_new ("sink_float", "audio/x-raw-float",
 *                                  GST_AUDIO_FLOAT_PAD_TEMPLATE_PROPS)),
 *    NULL);
 *
 *  sinkpad = gst_pad_new_from_template(template, "sink");
 *
 * Andy Wingo, 18 August 2001
 * Thomas, 6 September 2002 */

/* conversion macros */
/**
 * GST_FRAMES_TO_CLOCK_TIME:
 * @frames: sample frames
 * @rate: sampling rate
 * 
 * Calculate clocktime from sample @frames and @rate.
 */
#define GST_FRAMES_TO_CLOCK_TIME(frames, rate) \
  ((GstClockTime) gst_util_uint64_scale_round (frames, GST_SECOND, rate))

/**
 * GST_CLOCK_TIME_TO_FRAMES:
 * @clocktime: clock time
 * @rate: sampling rate
 * 
 * Calculate frames from @clocktime and sample @rate.
 */
#define GST_CLOCK_TIME_TO_FRAMES(clocktime, rate) \
  gst_util_uint64_scale_round (clocktime, rate, GST_SECOND)

/**
 * GST_AUDIO_DEF_RATE:
 * 
 * Standard sampling rate used in consumer audio.
 */
#define GST_AUDIO_DEF_RATE 44100

/**
 * GST_AUDIO_INT_PAD_TEMPLATE_CAPS:
 * 
 * Template caps for integer audio. Can be used when defining a 
 * #GstStaticPadTemplate
 */
#define GST_AUDIO_INT_PAD_TEMPLATE_CAPS \
  "audio/x-raw-int, " \
  "rate = (int) [ 1, MAX ], " \
  "channels = (int) [ 1, MAX ], " \
  "endianness = (int) { LITTLE_ENDIAN, BIG_ENDIAN }, " \
  "width = (int) { 8, 16, 24, 32 }, " \
  "depth = (int) [ 1, 32 ], " \
  "signed = (boolean) { true, false }"

/**
 * GST_AUDIO_INT_STANDARD_PAD_TEMPLATE_CAPS:
 * 
 * Template caps for 16bit integer stereo audio in native byte-order.
 * Can be used when defining a #GstStaticPadTemplate
 */
#define GST_AUDIO_INT_STANDARD_PAD_TEMPLATE_CAPS \
  "audio/x-raw-int, " \
  "rate = (int) [ 1, MAX ], " \
  "channels = (int) 2, " \
  "endianness = (int) BYTE_ORDER, " \
  "width = (int) 16, " \
  "depth = (int) 16, " \
  "signed = (boolean) true"

/**
 * GST_AUDIO_FLOAT_PAD_TEMPLATE_CAPS:
 * 
 * Template caps for float audio. Can be used when defining a 
 * #GstStaticPadTemplate
 */
#define GST_AUDIO_FLOAT_PAD_TEMPLATE_CAPS \
  "audio/x-raw-float, " \
  "rate = (int) [ 1, MAX ], " \
  "channels = (int) [ 1, MAX ], " \
  "endianness = (int) { LITTLE_ENDIAN , BIG_ENDIAN }, " \
  "width = (int) { 32, 64 }"

/**
 * GST_AUDIO_FLOAT_STANDARD_PAD_TEMPLATE_CAPS:
 * 
 * Template caps for 32bit float mono audio in native byte-order.
 * Can be used when defining a #GstStaticPadTemplate
 */
#define GST_AUDIO_FLOAT_STANDARD_PAD_TEMPLATE_CAPS \
  "audio/x-raw-float, " \
  "width = (int) 32, " \
  "rate = (int) [ 1, MAX ], " \
  "channels = (int) 1, " \
  "endianness = (int) BYTE_ORDER"

/*
 * this library defines and implements some helper functions for audio
 * handling
 */

/* get byte size of audio frame (based on caps of pad */
int      gst_audio_frame_byte_size      (GstPad* pad);

/* get length in frames of buffer */
long     gst_audio_frame_length         (GstPad* pad, GstBuffer* buf);

GstClockTime gst_audio_duration_from_pad_buffer (GstPad * pad, GstBuffer * buf);

/* check if the buffer size is a whole multiple of the frame size */
gboolean gst_audio_is_buffer_framed     (GstPad* pad, GstBuffer* buf);

/* functions useful for _getcaps functions */
/**
 * GstAudioFieldFlag:
 * @GST_AUDIO_FIELD_RATE: add rate field to caps
 * @GST_AUDIO_FIELD_CHANNELS: add channels field to caps
 * @GST_AUDIO_FIELD_ENDIANNESS: add endianness field to caps
 * @GST_AUDIO_FIELD_WIDTH: add width field to caps
 * @GST_AUDIO_FIELD_DEPTH: add depth field to caps
 * @GST_AUDIO_FIELD_SIGNED: add signed field to caps
 *
 * Do not use anymore.
 *
 * Deprecated: use gst_structure_set() directly
 */
#ifndef GST_DISABLE_DEPRECATED
typedef enum {
  GST_AUDIO_FIELD_RATE          = (1 << 0),
  GST_AUDIO_FIELD_CHANNELS      = (1 << 1),
  GST_AUDIO_FIELD_ENDIANNESS    = (1 << 2),
  GST_AUDIO_FIELD_WIDTH         = (1 << 3),
  GST_AUDIO_FIELD_DEPTH         = (1 << 4),
  GST_AUDIO_FIELD_SIGNED        = (1 << 5)
} GstAudioFieldFlag;
#endif

#ifndef GST_DISABLE_DEPRECATED
void gst_audio_structure_set_int (GstStructure *structure, GstAudioFieldFlag flag);
#endif /* GST_DISABLE_DEPRECATED */

GstBuffer *gst_audio_buffer_clip (GstBuffer *buffer, GstSegment *segment, gint rate, gint frame_size);

G_END_DECLS

#endif /* __GST_AUDIO_AUDIO_H__ */
