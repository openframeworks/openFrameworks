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

#ifndef _GST_APP_SRC_H_
#define _GST_APP_SRC_H_

#include <gst/gst.h>
#include <gst/base/gstpushsrc.h>

G_BEGIN_DECLS

#define GST_TYPE_APP_SRC \
  (gst_app_src_get_type())
#define GST_APP_SRC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_APP_SRC,GstAppSrc))
#define GST_APP_SRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_APP_SRC,GstAppSrcClass))
#define GST_IS_APP_SRC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_APP_SRC))
#define GST_IS_APP_SRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_APP_SRC))
/* Since 0.10.23 */
#define GST_APP_SRC_CAST(obj) \
  ((GstAppSrc*)(obj))

typedef struct _GstAppSrc GstAppSrc;
typedef struct _GstAppSrcClass GstAppSrcClass;
typedef struct _GstAppSrcPrivate GstAppSrcPrivate;

/**
 * GstAppSrcCallbacks:
 * @need_data: Called when the appsrc needs more data. A buffer or EOS should be
 *    pushed to appsrc from this thread or another thread. @length is just a hint
 *    and when it is set to -1, any number of bytes can be pushed into @appsrc.
 * @enough_data: Called when appsrc has enough data. It is recommended that the
 *    application stops calling push-buffer until the need_data callback is
 *    emited again to avoid excessive buffer queueing.
 * @seek_data: Called when a seek should be performed to the offset.
 *    The next push-buffer should produce buffers from the new @offset.
 *    This callback is only called for seekable stream types.
 *
 * A set of callbacks that can be installed on the appsrc with
 * gst_app_src_set_callbacks().
 *
 * Since: 0.10.23
 */
typedef struct {
  void      (*need_data)    (GstAppSrc *src, guint length, gpointer user_data);
  void      (*enough_data)  (GstAppSrc *src, gpointer user_data);
  gboolean  (*seek_data)    (GstAppSrc *src, guint64 offset, gpointer user_data);

  /*< private >*/
  gpointer     _gst_reserved[GST_PADDING];
} GstAppSrcCallbacks;

/**
 * GstAppStreamType:
 * @GST_APP_STREAM_TYPE_STREAM: No seeking is supported in the stream, such as a
 * live stream.
 * @GST_APP_STREAM_TYPE_SEEKABLE: The stream is seekable but seeking might not
 * be very fast, such as data from a webserver. 
 * @GST_APP_STREAM_TYPE_RANDOM_ACCESS: The stream is seekable and seeking is fast,
 * such as in a local file.
 *
 * The stream type.
 */
typedef enum
{
  GST_APP_STREAM_TYPE_STREAM,
  GST_APP_STREAM_TYPE_SEEKABLE,
  GST_APP_STREAM_TYPE_RANDOM_ACCESS
} GstAppStreamType;

struct _GstAppSrc
{
  GstBaseSrc basesrc;

  /*< private >*/
  GstAppSrcPrivate *priv;

  /*< private >*/
  gpointer     _gst_reserved[GST_PADDING];
};

struct _GstAppSrcClass
{
  GstBaseSrcClass basesrc_class;

  /* signals */
  void          (*need_data)       (GstAppSrc *src, guint length);
  void          (*enough_data)     (GstAppSrc *src);
  gboolean      (*seek_data)       (GstAppSrc *src, guint64 offset);

  /* actions */
  GstFlowReturn (*push_buffer)     (GstAppSrc *src, GstBuffer *buffer);
  GstFlowReturn (*end_of_stream)   (GstAppSrc *src);

  /*< private >*/
  gpointer     _gst_reserved[GST_PADDING];
};

GType gst_app_src_get_type(void);

void             gst_app_src_set_caps         (GstAppSrc *appsrc, const GstCaps *caps);
GstCaps*         gst_app_src_get_caps         (GstAppSrc *appsrc);

void             gst_app_src_set_size         (GstAppSrc *appsrc, gint64 size);
gint64           gst_app_src_get_size         (GstAppSrc *appsrc);

void             gst_app_src_set_stream_type  (GstAppSrc *appsrc, GstAppStreamType type);
GstAppStreamType gst_app_src_get_stream_type  (GstAppSrc *appsrc);

void             gst_app_src_set_max_bytes    (GstAppSrc *appsrc, guint64 max);
guint64          gst_app_src_get_max_bytes    (GstAppSrc *appsrc);

void             gst_app_src_set_latency      (GstAppSrc *appsrc, guint64 min, guint64 max);
void             gst_app_src_get_latency      (GstAppSrc *appsrc, guint64 *min, guint64 *max);

void             gst_app_src_set_emit_signals (GstAppSrc *appsrc, gboolean emit);
gboolean         gst_app_src_get_emit_signals (GstAppSrc *appsrc);

GstFlowReturn    gst_app_src_push_buffer      (GstAppSrc *appsrc, GstBuffer *buffer);
GstFlowReturn    gst_app_src_end_of_stream    (GstAppSrc *appsrc);

void             gst_app_src_set_callbacks    (GstAppSrc * appsrc,
                                               GstAppSrcCallbacks *callbacks,
                                               gpointer user_data,
                                               GDestroyNotify notify);

G_END_DECLS

#endif

