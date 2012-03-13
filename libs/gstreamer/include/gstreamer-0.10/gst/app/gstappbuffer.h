/* GStreamer
 * Copyright (C) 2007 David Schleef <ds@schleef.org>
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

#ifndef _GST_APP_BUFFER_H_
#define _GST_APP_BUFFER_H_

#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_TYPE_APP_BUFFER \
  (gst_app_buffer_get_type())
#define GST_APP_BUFFER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_APP_BUFFER,GstAppBuffer))
#define GST_APP_BUFFER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_APP_BUFFER,GstAppBufferClass))
#define GST_IS_APP_BUFFER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_APP_BUFFER))
#define GST_IS_APP_BUFFER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_APP_BUFFER))

typedef struct _GstAppBuffer GstAppBuffer;
typedef struct _GstAppBufferClass GstAppBufferClass;
typedef void (*GstAppBufferFinalizeFunc) (void *priv);

struct _GstAppBuffer
{
  GstBuffer buffer;

  /*< private >*/
  GstAppBufferFinalizeFunc finalize;
  void *priv;
};

struct _GstAppBufferClass
{
  GstBufferClass buffer_class;
};

GType gst_app_buffer_get_type(void);

GstBuffer *gst_app_buffer_new (void *data, int length,
    GstAppBufferFinalizeFunc finalize, void *priv);

G_END_DECLS

#endif

