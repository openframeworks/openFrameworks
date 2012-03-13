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

#ifndef _GST_BASE_VIDEO_ENCODER_H_
#define _GST_BASE_VIDEO_ENCODER_H_

#ifndef GST_USE_UNSTABLE_API
#warning "GstBaseVideoEncoder is unstable API and may change in future."
#warning "You can define GST_USE_UNSTABLE_API to avoid this warning."
#endif

#include <gst/video/gstbasevideocodec.h>
#include <gst/video/gstbasevideoutils.h>

G_BEGIN_DECLS

#define GST_TYPE_BASE_VIDEO_ENCODER \
  (gst_base_video_encoder_get_type())
#define GST_BASE_VIDEO_ENCODER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_BASE_VIDEO_ENCODER,GstBaseVideoEncoder))
#define GST_BASE_VIDEO_ENCODER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_BASE_VIDEO_ENCODER,GstBaseVideoEncoderClass))
#define GST_BASE_VIDEO_ENCODER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj),GST_TYPE_BASE_VIDEO_ENCODER,GstBaseVideoEncoderClass))
#define GST_IS_BASE_VIDEO_ENCODER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_BASE_VIDEO_ENCODER))
#define GST_IS_BASE_VIDEO_ENCODER_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_BASE_VIDEO_ENCODER))

/**
 * GST_BASE_VIDEO_ENCODER_SINK_NAME:
 *
 * The name of the templates for the sink pad.
 */
#define GST_BASE_VIDEO_ENCODER_SINK_NAME    "sink"
/**
 * GST_BASE_VIDEO_ENCODER_SRC_NAME:
 *
 * The name of the templates for the source pad.
 */
#define GST_BASE_VIDEO_ENCODER_SRC_NAME     "src"


typedef struct _GstBaseVideoEncoder GstBaseVideoEncoder;
typedef struct _GstBaseVideoEncoderClass GstBaseVideoEncoderClass;

struct _GstBaseVideoEncoder
{
  GstBaseVideoCodec base_video_codec;

  /*< private >*/
  gboolean sink_clipping;

  guint64 presentation_frame_number;
  int distance_from_sync;

  gboolean set_output_caps;

  gint64 min_latency;
  gint64 max_latency;

  gboolean force_keyframe;

  /* FIXME before moving to base */
  void *padding[GST_PADDING_LARGE];
};

struct _GstBaseVideoEncoderClass
{
  GstBaseVideoCodecClass base_video_codec_class;

  gboolean (*set_format) (GstBaseVideoEncoder *coder, GstVideoState *state);
  gboolean (*start) (GstBaseVideoEncoder *coder);
  gboolean (*stop) (GstBaseVideoEncoder *coder);
  gboolean (*finish) (GstBaseVideoEncoder *coder);
  gboolean (*handle_frame) (GstBaseVideoEncoder *coder, GstVideoFrame *frame);
  GstFlowReturn (*shape_output) (GstBaseVideoEncoder *coder, GstVideoFrame *frame);
  GstCaps *(*get_caps) (GstBaseVideoEncoder *coder);

  /* FIXME before moving to base */
  void *padding[GST_PADDING_LARGE];
};

GType gst_base_video_encoder_get_type (void);

int gst_base_video_encoder_get_width (GstBaseVideoEncoder *coder);
int gst_base_video_encoder_get_height (GstBaseVideoEncoder *coder);
const GstVideoState *gst_base_video_encoder_get_state (GstBaseVideoEncoder *coder);

guint64 gst_base_video_encoder_get_timestamp_offset (GstBaseVideoEncoder *coder);

GstVideoFrame *gst_base_video_encoder_get_frame (GstBaseVideoEncoder *coder,
    int frame_number);
GstVideoFrame *gst_base_video_encoder_get_oldest_frame (GstBaseVideoEncoder *coder);
GstFlowReturn gst_base_video_encoder_finish_frame (GstBaseVideoEncoder *base_video_encoder,
    GstVideoFrame *frame);
GstFlowReturn gst_base_video_encoder_end_of_stream (GstBaseVideoEncoder *base_video_encoder,
    GstBuffer *buffer);

void gst_base_video_encoder_set_latency (GstBaseVideoEncoder *base_video_encoder,
    GstClockTime min_latency, GstClockTime max_latency);
void gst_base_video_encoder_set_latency_fields (GstBaseVideoEncoder *base_video_encoder,
    int n_fields);


G_END_DECLS

#endif

