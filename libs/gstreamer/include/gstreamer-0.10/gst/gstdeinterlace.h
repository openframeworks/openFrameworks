/*
 * GStreamer
 * Copyright (C) 2005 Martin Eikermann <meiker@upb.de>
 * Copyright (C) 2008-2010 Sebastian Dröge <slomo@collabora.co.uk>
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

#ifndef __GST_DEINTERLACE_H__
#define __GST_DEINTERLACE_H__

#include <gst/gst.h>
#include <gst/video/video.h>

#include "gstdeinterlacemethod.h"

G_BEGIN_DECLS

#define GST_TYPE_DEINTERLACE \
  (gst_deinterlace_get_type())
#define GST_DEINTERLACE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_DEINTERLACE,GstDeinterlace))
#define GST_DEINTERLACE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_DEINTERLACE,GstDeinterlace))
#define GST_IS_DEINTERLACE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_DEINTERLACE))
#define GST_IS_DEINTERLACE_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_DEINTERLACE))

typedef struct _GstDeinterlace GstDeinterlace;
typedef struct _GstDeinterlaceClass GstDeinterlaceClass;

#define GST_DEINTERLACE_MAX_FIELD_HISTORY 10

typedef enum
{
  GST_DEINTERLACE_TOMSMOCOMP,
  GST_DEINTERLACE_GREEDY_H,
  GST_DEINTERLACE_GREEDY_L,
  GST_DEINTERLACE_VFIR,
  GST_DEINTERLACE_LINEAR,
  GST_DEINTERLACE_LINEAR_BLEND,
  GST_DEINTERLACE_SCALER_BOB,
  GST_DEINTERLACE_WEAVE,
  GST_DEINTERLACE_WEAVE_TFF,
  GST_DEINTERLACE_WEAVE_BFF
} GstDeinterlaceMethods;

typedef enum
{
  GST_DEINTERLACE_ALL,         /* All (missing data is interp.) */
  GST_DEINTERLACE_TF,          /* Top Fields Only */
  GST_DEINTERLACE_BF           /* Bottom Fields Only */
} GstDeinterlaceFields;

typedef enum
{
  GST_DEINTERLACE_LAYOUT_AUTO,
  GST_DEINTERLACE_LAYOUT_TFF,
  GST_DEINTERLACE_LAYOUT_BFF
} GstDeinterlaceFieldLayout;

typedef enum {
  GST_DEINTERLACE_MODE_AUTO,
  GST_DEINTERLACE_MODE_INTERLACED,
  GST_DEINTERLACE_MODE_DISABLED
} GstDeinterlaceMode;

struct _GstDeinterlace
{
  GstElement parent;

  GstPad *srcpad, *sinkpad;

  /* <private> */
  GstDeinterlaceMode mode;

  GstDeinterlaceFieldLayout field_layout;

  GstDeinterlaceFields fields;

  GstDeinterlaceMethods method_id; /* current state (differs when flushing) */
  GstDeinterlaceMethods user_set_method_id; /* property value */
  GstDeinterlaceMethod *method;

  GstVideoFormat format;
  gint width, height; /* frame width & height */
  guint frame_size; /* frame size in bytes */
  gint fps_n, fps_d; /* frame rate */
  gboolean interlaced; /* is input interlaced? */

  gboolean passthrough;

  GstClockTime field_duration; /* Duration of one field */

  /* The most recent pictures 
     PictureHistory[0] is always the most recent.
     Pointers are NULL if the picture in question isn't valid, e.g. because
     the program just started or a picture was skipped.
   */
  GstDeinterlaceField field_history[GST_DEINTERLACE_MAX_FIELD_HISTORY];
  guint history_count;

  /* Set to TRUE if we're in still frame mode,
     i.e. just forward all buffers
   */
  gboolean still_frame_mode;

  /* Last buffer that was pushed in */
  GstBuffer *last_buffer;

  /* Current segment */
  GstSegment segment;

  /* QoS stuff */
  gdouble proportion;
  GstClockTime earliest_time;

  /* Upstream negotiation stuff */
  GstCaps *sink_caps;
  GstCaps *src_caps;

  GstCaps *request_caps;

  gboolean reconfigure;
  GstDeinterlaceMode new_mode;
  GstDeinterlaceFields new_fields;
};

struct _GstDeinterlaceClass
{
  GstElementClass parent_class;
};

GType gst_deinterlace_get_type (void);

G_END_DECLS

#endif /* __GST_DEINTERLACE_H__ */
