/* GStreamer
 * Copyright (C) <1999> Erik Walthinsen <omega@cse.ogi.edu>
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


#ifndef __GST_VIDEO_FILTER_H__
#define __GST_VIDEO_FILTER_H__

#include <gst/base/gstbasetransform.h>

G_BEGIN_DECLS

typedef struct _GstVideoFilter GstVideoFilter;
typedef struct _GstVideoFilterClass GstVideoFilterClass;

#define GST_TYPE_VIDEO_FILTER \
  (gst_video_filter_get_type())
#define GST_VIDEO_FILTER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_VIDEO_FILTER,GstVideoFilter))
#define GST_VIDEO_FILTER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_VIDEO_FILTER,GstVideoFilterClass))
#define GST_VIDEO_FILTER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS((obj), GST_TYPE_VIDEO_FILTER, GstVideoFilterClass))
#define GST_IS_VIDEO_FILTER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_VIDEO_FILTER))
#define GST_IS_VIDEO_FILTER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_VIDEO_FILTER))

struct _GstVideoFilter {
  GstBaseTransform element;

  gboolean inited;
};

struct _GstVideoFilterClass {
  GstBaseTransformClass parent_class;
};

GType gst_video_filter_get_type (void);

G_END_DECLS

#endif /* __GST_VIDEO_FILTER_H__ */
