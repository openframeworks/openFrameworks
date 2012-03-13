/* GStreamer
 * Copyright (C) 2006 Edward Hervey <edward@fluendo.com>
 *
 * gstdataqueue.h:
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


#ifndef __GST_DATA_QUEUE_H__
#define __GST_DATA_QUEUE_H__

#include <gst/gst.h>

G_BEGIN_DECLS
#define GST_TYPE_DATA_QUEUE \
  (gst_data_queue_get_type())
#define GST_DATA_QUEUE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_DATA_QUEUE,GstDataQueue))
#define GST_DATA_QUEUE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_DATA_QUEUE,GstDataQueueClass))
#define GST_IS_DATA_QUEUE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_DATA_QUEUE))
#define GST_IS_DATA_QUEUE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_DATA_QUEUE))
typedef struct _GstDataQueue GstDataQueue;
typedef struct _GstDataQueueClass GstDataQueueClass;
typedef struct _GstDataQueueSize GstDataQueueSize;
typedef struct _GstDataQueueItem GstDataQueueItem;

/**
 * GstDataQueueItem:
 * @object: the #GstMiniObject to queue.
 * @size: the size in bytes of the miniobject.
 * @duration: the duration in #GstClockTime of the miniobject. Can not be
 * #GST_CLOCK_TIME_NONE.
 * @visible: #TRUE if @object should be considered as a visible object.
 * @destroy: The #GDestroyNotify function to use to free the #GstDataQueueItem.
 * This function should also drop the reference to @object the owner of the
 * #GstDataQueueItem is assumed to hold.
 *
 * Structure used by #GstDataQueue. You can supply a different structure, as
 * long as the top of the structure is identical to this structure.
 *
 * Since: 0.10.11
 */

struct _GstDataQueueItem
{
  GstMiniObject *object;
  guint size;
  guint64 duration;
  gboolean visible;

  /* user supplied destroy function */
  GDestroyNotify destroy;
};

/**
 * GstDataQueueSize:
 * @visible: number of buffers
 * @bytes: number of bytes
 * @time: amount of time
 *
 * Structure describing the size of a queue.
 *
 * Since: 0.10.11
 */
struct _GstDataQueueSize
{
  guint visible;
  guint bytes;
  guint64 time;
};

/**
 * GstDataQueueCheckFullFunction:
 * @queue: a #GstDataQueue.
 * @visible: The number of visible items currently in the queue.
 * @bytes: The amount of bytes currently in the queue.
 * @time: The accumulated duration of the items currently in the queue.
 * @checkdata: The #gpointer registered when the #GstDataQueue was created.
 * 
 * The prototype of the function used to inform the queue that it should be
 * considered as full.
 *
 * Returns: #TRUE if the queue should be considered full.
 *
 * Since: 0.10.11
 */
typedef gboolean (*GstDataQueueCheckFullFunction) (GstDataQueue * queue,
    guint visible, guint bytes, guint64 time, gpointer checkdata);

typedef void (*GstDataQueueFullCallback) (GstDataQueue * queue, gpointer checkdata);
typedef void (*GstDataQueueEmptyCallback) (GstDataQueue * queue, gpointer checkdata);

/**
 * GstDataQueue:
 * @object: the parent structure
 *
 * Opaque #GstDataQueue structure.
 *
 * Since: 0.10.11
 */
struct _GstDataQueue
{
  GObject object;

  /*< private >*/
  /* the queue of data we're keeping our grubby hands on */
  GQueue *queue;

  GstDataQueueSize cur_level;   /* size of the queue */
  GstDataQueueCheckFullFunction checkfull;      /* Callback to check if the queue is full */
  gpointer *checkdata;

  GMutex *qlock;                /* lock for queue (vs object lock) */
  GCond *item_add;              /* signals buffers now available for reading */
  GCond *item_del;              /* signals space now available for writing */
  gboolean flushing;            /* indicates whether conditions where signalled because
                                 * of external flushing */
  GstDataQueueFullCallback fullcallback;
  GstDataQueueEmptyCallback emptycallback;
  gpointer _gst_reserved[GST_PADDING - 2];
};

struct _GstDataQueueClass
{
  GObjectClass parent_class;

  /* signals */
  void (*empty) (GstDataQueue * queue);
  void (*full) (GstDataQueue * queue);

  gpointer _gst_reserved[GST_PADDING];
};

GType gst_data_queue_get_type (void);

GstDataQueue * gst_data_queue_new            (GstDataQueueCheckFullFunction checkfull,
                                              gpointer checkdata);

GstDataQueue * gst_data_queue_new_full       (GstDataQueueCheckFullFunction checkfull,
					      GstDataQueueFullCallback fullcallback,
					      GstDataQueueEmptyCallback emptycallback,
					      gpointer checkdata);

gboolean       gst_data_queue_push           (GstDataQueue * queue, GstDataQueueItem * item);
gboolean       gst_data_queue_pop            (GstDataQueue * queue, GstDataQueueItem ** item);

void           gst_data_queue_flush          (GstDataQueue * queue);
void           gst_data_queue_set_flushing   (GstDataQueue * queue, gboolean flushing);

gboolean       gst_data_queue_drop_head      (GstDataQueue * queue, GType type);

gboolean       gst_data_queue_is_full        (GstDataQueue * queue);
gboolean       gst_data_queue_is_empty       (GstDataQueue * queue);

void           gst_data_queue_get_level      (GstDataQueue * queue, GstDataQueueSize *level);
void           gst_data_queue_limits_changed (GstDataQueue * queue);

G_END_DECLS

#endif /* __GST_DATA_QUEUE_H__ */
