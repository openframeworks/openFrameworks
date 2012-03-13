/* GStreamer
 * Copyright (C) 2004 Benjamin Otte <otte@gnome.org>
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

#ifndef __GST_ADAPTER_H__
#define __GST_ADAPTER_H__

G_BEGIN_DECLS


#define GST_TYPE_ADAPTER \
  (gst_adapter_get_type())
#define GST_ADAPTER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_ADAPTER, GstAdapter))
#define GST_ADAPTER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_ADAPTER, GstAdapterClass))
#define GST_ADAPTER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_ADAPTER, GstAdapterClass))
#define GST_IS_ADAPTER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_ADAPTER))
#define GST_IS_ADAPTER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_ADAPTER))

typedef struct _GstAdapter GstAdapter;
typedef struct _GstAdapterClass GstAdapterClass;
typedef struct _GstAdapterPrivate GstAdapterPrivate;

/**
 * GstAdapter:
 *
 * The opaque #GstAdapter data structure.
 */
struct _GstAdapter {
  GObject       object;

  /*< private >*/
  GSList *      buflist;
  guint         size;
  guint         skip;

  /* we keep state of assembled pieces */
  guint8 *      assembled_data;
  guint         assembled_size;
  guint         assembled_len;

  /* ABI added */
  /* Remember where the end of our buffer list is to
   * speed up the push */
  GSList *buflist_end;

  GstAdapterPrivate *priv;

  gpointer _gst_reserved[GST_PADDING - 2];
};

struct _GstAdapterClass {
  GObjectClass  parent_class;

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

GType                   gst_adapter_get_type            (void);

GstAdapter *            gst_adapter_new                 (void);

void                    gst_adapter_clear               (GstAdapter *adapter);
void                    gst_adapter_push                (GstAdapter *adapter, GstBuffer* buf);
const guint8 *          gst_adapter_peek                (GstAdapter *adapter, guint size);
void                    gst_adapter_copy                (GstAdapter *adapter, guint8 *dest,
                                                         guint offset, guint size);
void                    gst_adapter_flush               (GstAdapter *adapter, guint flush);
guint8*                 gst_adapter_take                (GstAdapter *adapter, guint nbytes);
GstBuffer*              gst_adapter_take_buffer         (GstAdapter *adapter, guint nbytes);
GList*                  gst_adapter_take_list           (GstAdapter *adapter, guint nbytes);
guint                   gst_adapter_available           (GstAdapter *adapter);
guint                   gst_adapter_available_fast      (GstAdapter *adapter);

GstClockTime            gst_adapter_prev_timestamp      (GstAdapter *adapter, guint64 *distance);

guint                   gst_adapter_masked_scan_uint32  (GstAdapter * adapter, guint32 mask,
                                                         guint32 pattern, guint offset, guint size);

guint                   gst_adapter_masked_scan_uint32_peek  (GstAdapter * adapter, guint32 mask,
                                                         guint32 pattern, guint offset, guint size, guint32 * value);

G_END_DECLS

#endif /* __GST_ADAPTER_H__ */
