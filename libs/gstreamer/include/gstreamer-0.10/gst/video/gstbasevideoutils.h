/* GStreamer
 * Copyright (C) 2008 David Schleef <ds@schleef.org>
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

#ifndef _GST_BASE_VIDEO_UTILS_H_
#define _GST_BASE_VIDEO_UTILS_H_

#ifndef GST_USE_UNSTABLE_API
#warning "The base video utils API is unstable and may change in future."
#warning "You can define GST_USE_UNSTABLE_API to avoid this warning."
#endif

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/base/gstadapter.h>

G_BEGIN_DECLS

typedef struct _GstVideoState GstVideoState;
typedef struct _GstVideoFrame GstVideoFrame;

struct _GstVideoState
{
  GstVideoFormat format;
  int width, height;
  int fps_n, fps_d;
  int par_n, par_d;

  gboolean have_interlaced;
  gboolean interlaced;
  gboolean top_field_first;

  int clean_width, clean_height;
  int clean_offset_left, clean_offset_top;

  int bytes_per_picture;

  //GstSegment segment;

  int picture_number;
  GstBuffer *codec_data;

};

struct _GstVideoFrame
{
  GstClockTime decode_timestamp;
  GstClockTime presentation_timestamp;
  GstClockTime presentation_duration;

  gint system_frame_number;
  gint decode_frame_number;
  gint presentation_frame_number;

  int distance_from_sync;
  gboolean is_sync_point;
  gboolean is_eos;

  GstBuffer *sink_buffer;
  GstBuffer *src_buffer;

  int field_index;
  int n_fields;

  void *coder_hook;
  GstClockTime deadline;

  gboolean force_keyframe;
};

gboolean gst_base_video_rawvideo_convert (GstVideoState *state,
    GstFormat src_format, gint64 src_value,
    GstFormat * dest_format, gint64 *dest_value);
gboolean gst_base_video_encoded_video_convert (GstVideoState *state,
    GstFormat src_format, gint64 src_value,
    GstFormat * dest_format, gint64 *dest_value);

GstClockTime gst_video_state_get_timestamp (const GstVideoState *state,
    GstSegment *segment, int frame_number);

GstBuffer *gst_adapter_get_buffer (GstAdapter *adapter);

G_END_DECLS

#endif

