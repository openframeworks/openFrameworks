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

#ifndef _GST_BASE_VIDEO_PARSE_H_
#define _GST_BASE_VIDEO_PARSE_H_

#ifndef GST_USE_UNSTABLE_API
#warning "GstBaseVideoParse is unstable API and may change in future."
#warning "You can define GST_USE_UNSTABLE_API to avoid this warning."
#endif

#include <gst/video/gstbasevideocodec.h>
#include <gst/video/gstbasevideoutils.h>

G_BEGIN_DECLS

#define GST_TYPE_BASE_VIDEO_PARSE \
  (gst_base_video_parse_get_type())
#define GST_BASE_VIDEO_PARSE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_BASE_VIDEO_PARSE,GstBaseVideoParse))
#define GST_BASE_VIDEO_PARSE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_BASE_VIDEO_PARSE,GstBaseVideoParseClass))
#define GST_BASE_VIDEO_PARSE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj),GST_TYPE_BASE_VIDEO_PARSE,GstBaseVideoParseClass))
#define GST_IS_BASE_VIDEO_PARSE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_BASE_VIDEO_PARSE))
#define GST_IS_BASE_VIDEO_PARSE_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_BASE_VIDEO_PARSE))

/**
 * GST_BASE_VIDEO_PARSE_SINK_NAME:
 *
 * The name of the templates for the sink pad.
 */
#define GST_BASE_VIDEO_PARSE_SINK_NAME    "sink"
/**
 * GST_BASE_VIDEO_PARSE_SRC_NAME:
 *
 * The name of the templates for the source pad.
 */
#define GST_BASE_VIDEO_PARSE_SRC_NAME     "src"

/**
 * GST_BASE_VIDEO_PARSE_FLOW_NEED_DATA:
 *
 */
#define GST_BASE_VIDEO_PARSE_FLOW_NEED_DATA GST_FLOW_CUSTOM_SUCCESS

typedef struct _GstBaseVideoParse GstBaseVideoParse;
typedef struct _GstBaseVideoParseClass GstBaseVideoParseClass;

struct _GstBaseVideoParse
{
  GstBaseVideoCodec base_video_codec;

  /*< private >*/
  GstAdapter *input_adapter;
  GstAdapter *output_adapter;

  int reorder_depth;

  gboolean have_sync;
  gboolean discont;
  gboolean started;

  GstVideoFrame *current_frame;
  GstVideoState state;
  GstSegment segment;
  int distance_from_sync;

  gboolean sink_clipping;

  guint64 presentation_frame_number;
  guint64 system_frame_number;

  GstCaps *caps;
  gboolean set_output_caps;

  GstClockTime last_timestamp;

  gint64 timestamp_offset;
};

struct _GstBaseVideoParseClass
{
  GstBaseVideoCodecClass base_video_codec_class;

  gboolean (*start) (GstBaseVideoParse *parse);
  gboolean (*stop) (GstBaseVideoParse *parse);
  gboolean (*reset) (GstBaseVideoParse *parse);
  GstFlowReturn (*parse_data) (GstBaseVideoParse *parse, gboolean at_eos);
  int (*scan_for_sync) (GstAdapter *adapter, gboolean at_eos,
      int offset, int n);
  GstFlowReturn (*shape_output) (GstBaseVideoParse *parse, GstVideoFrame *frame);
  GstCaps *(*get_caps) (GstBaseVideoParse *parse);

};

GType gst_base_video_parse_get_type (void);

int gst_base_video_parse_get_width (GstBaseVideoParse *parse);
int gst_base_video_parse_get_height (GstBaseVideoParse *parse);
GstVideoState *gst_base_video_parse_get_state (GstBaseVideoParse *parse);
void gst_base_video_parse_set_state (GstBaseVideoParse *parse,
    GstVideoState *state);

guint64 gst_base_video_parse_get_timestamp_offset (GstBaseVideoParse *parse);

gboolean gst_base_video_parse_set_src_caps (GstBaseVideoParse *base_video_parse, GstCaps *caps);

GstFlowReturn gst_base_video_parse_end_of_stream (GstBaseVideoParse *base_video_parse,
    GstBuffer *buffer);

void gst_base_video_parse_lost_sync (GstBaseVideoParse *base_video_parse);

GstVideoFrame * gst_base_video_parse_get_frame (GstBaseVideoParse *base_video_parse);
void gst_base_video_parse_add_to_frame (GstBaseVideoParse *base_video_parse, int n_bytes);
GstFlowReturn gst_base_video_parse_finish_frame (GstBaseVideoParse *base_video_parse);
void gst_base_video_parse_set_sync_point (GstBaseVideoParse *base_video_parse);
GstFlowReturn gst_base_video_parse_push (GstBaseVideoParse *base_video_parse,
    GstBuffer *buffer);

G_END_DECLS

#endif

