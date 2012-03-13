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

#ifndef _GST_BASE_VIDEO_CODEC_H_
#define _GST_BASE_VIDEO_CODEC_H_

#ifndef GST_USE_UNSTABLE_API
#warning "GstBaseVideoCodec is unstable API and may change in future."
#warning "You can define GST_USE_UNSTABLE_API to avoid this warning."
#endif

#include <gst/gst.h>
#include <gst/video/gstbasevideoutils.h>

G_BEGIN_DECLS

#define GST_TYPE_BASE_VIDEO_CODEC \
  (gst_base_video_codec_get_type())
#define GST_BASE_VIDEO_CODEC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_BASE_VIDEO_CODEC,GstBaseVideoCodec))
#define GST_BASE_VIDEO_CODEC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_BASE_VIDEO_CODEC,GstBaseVideoCodecClass))
#define GST_BASE_VIDEO_CODEC_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj),GST_TYPE_BASE_VIDEO_CODEC,GstBaseVideoCodecClass))
#define GST_IS_BASE_VIDEO_CODEC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_BASE_VIDEO_CODEC))
#define GST_IS_BASE_VIDEO_CODEC_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_BASE_VIDEO_CODEC))

/**
 * GST_BASE_VIDEO_CODEC_SINK_NAME:
 *
 * The name of the templates for the sink pad.
 */
#define GST_BASE_VIDEO_CODEC_SINK_NAME    "sink"
/**
 * GST_BASE_VIDEO_CODEC_SRC_NAME:
 *
 * The name of the templates for the source pad.
 */
#define GST_BASE_VIDEO_CODEC_SRC_NAME     "src"

/**
 * GST_BASE_VIDEO_CODEC_SRC_PAD:
 * @obj: base video codec instance
 *
 * Gives the pointer to the source #GstPad object of the element.
 */
#define GST_BASE_VIDEO_CODEC_SRC_PAD(obj)         (((GstBaseVideoCodec *) (obj))->srcpad)

/**
 * GST_BASE_VIDEO_CODEC_SINK_PAD:
 * @obj: base video codec instance
 *
 * Gives the pointer to the sink #GstPad object of the element.
 */
#define GST_BASE_VIDEO_CODEC_SINK_PAD(obj)        (((GstBaseVideoCodec *) (obj))->sinkpad)

/**
 * GST_BASE_VIDEO_CODEC_FLOW_NEED_DATA:
 *
 */
#define GST_BASE_VIDEO_CODEC_FLOW_NEED_DATA GST_FLOW_CUSTOM_SUCCESS

typedef struct _GstBaseVideoCodec GstBaseVideoCodec;
typedef struct _GstBaseVideoCodecClass GstBaseVideoCodecClass;

struct _GstBaseVideoCodec
{
  GstElement element;

  /*< private >*/
  GstPad *sinkpad;
  GstPad *srcpad;

  guint64 system_frame_number;

  GList *frames;
  GstVideoState state;
  GstSegment segment;

  GstCaps *caps;

  gdouble proportion;
  GstClockTime earliest_time;

  /* FIXME before moving to base */
  void *padding[GST_PADDING_LARGE];
};

struct _GstBaseVideoCodecClass
{
  GstElementClass element_class;

  gboolean (*start) (GstBaseVideoCodec *codec);
  gboolean (*stop) (GstBaseVideoCodec *codec);
  gboolean (*reset) (GstBaseVideoCodec *codec);
  GstFlowReturn (*parse_data) (GstBaseVideoCodec *codec, gboolean at_eos);
  int (*scan_for_sync) (GstAdapter *adapter, gboolean at_eos,
      int offset, int n);
  GstFlowReturn (*shape_output) (GstBaseVideoCodec *codec, GstVideoFrame *frame);
  GstCaps *(*get_caps) (GstBaseVideoCodec *codec);

  /* FIXME before moving to base */
  void *padding[GST_PADDING_LARGE];
};

GType gst_base_video_codec_get_type (void);

GstVideoFrame * gst_base_video_codec_new_frame (GstBaseVideoCodec *base_video_codec);
void gst_base_video_codec_free_frame (GstVideoFrame *frame);


G_END_DECLS

#endif

