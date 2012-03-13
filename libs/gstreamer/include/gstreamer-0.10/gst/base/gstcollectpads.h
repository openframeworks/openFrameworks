/* GStreamer
 * Copyright (C) 2005 Wim Taymans <wim@fluendo.com>
 *
 * gstcollect_pads.h:
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

#ifndef __GST_COLLECT_PADS_H__
#define __GST_COLLECT_PADS_H__

#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_TYPE_COLLECT_PADS            (gst_collect_pads_get_type())
#define GST_COLLECT_PADS(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_COLLECT_PADS,GstCollectPads))
#define GST_COLLECT_PADS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_COLLECT_PADS,GstCollectPadsClass))
#define GST_COLLECT_PADS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),GST_TYPE_COLLECT_PADS,GstCollectPadsClass))
#define GST_IS_COLLECT_PADS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_COLLECT_PADS))
#define GST_IS_COLLECT_PADS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_COLLECT_PADS))

typedef struct _GstCollectData GstCollectData;
typedef struct _GstCollectPads GstCollectPads;
typedef struct _GstCollectPadsPrivate GstCollectPadsPrivate;
typedef struct _GstCollectPadsClass GstCollectPadsClass;

/**
 * GstCollectDataDestroyNotify:
 * @data: the #GstCollectData that will be freed
 *
 * A function that will be called when the #GstCollectData will be freed.
 * It is passed the pointer to the structure and should free any custom
 * memory and resources allocated for it.
 *
 * Since: 0.10.12
 */
typedef void (*GstCollectDataDestroyNotify) (GstCollectData *data);

/**
 * GstCollectPadsClipFunction:
 * @pads: a #GstCollectPads 
 * @data: a #GstCollectData 
 * @buffer: a #GstBuffer 
 * @user_data: user data 
 *
 * A function that will be called when @buffer is received on the pad managed
 * by @data in the collecpad object @pads.
 *
 * The function should use the segment of @data and the negotiated media type on
 * the pad to perform clipping of @buffer. 
 *
 * This function takes ownership of @buffer.
 *
 * Returns: a #GstBuffer that contains the clipped data of @buffer or NULL when
 * the buffer has been clipped completely.
 *
 * Since: 0.10.26
 */
typedef GstBuffer * (*GstCollectPadsClipFunction) (GstCollectPads *pads, GstCollectData *data,
                                                   GstBuffer *buffer, gpointer user_data);

/**
 * GstCollectData:
 * @collect: owner #GstCollectPads
 * @pad: #GstPad managed by this data
 * @buffer: currently queued buffer.
 * @pos: position in the buffer
 * @segment: last segment received.
 *
 * Structure used by the collect_pads.
 */
struct _GstCollectData
{
  /* with LOCK of @collect */
  GstCollectPads        *collect;
  GstPad                *pad;
  GstBuffer             *buffer;
  guint                  pos;
  GstSegment             segment;

  /*< private >*/
  union {
    struct {
      gboolean           flushing;
      gboolean           new_segment;
      gboolean           eos;
      gint               refcount;
    } ABI;
    /* adding + 0 to mark ABI change to be undone later */
    gpointer _gst_reserved[GST_PADDING + 0];
  } abidata;
};

/**
 * GstCollectPadsFunction:
 * @pads: the #GstCollectPads that triggered the callback
 * @user_data: user data passed to gst_collect_pads_set_function()
 *
 * A function that will be called when all pads have received data.
 *
 * Returns: #GST_FLOW_OK for success
 */
typedef GstFlowReturn (*GstCollectPadsFunction) (GstCollectPads *pads, gpointer user_data);

#define GST_COLLECT_PADS_GET_PAD_LOCK(pads) (((GstCollectPads *)pads)->abidata.ABI.pad_lock)
#define GST_COLLECT_PADS_PAD_LOCK(pads)     (g_mutex_lock(GST_COLLECT_PADS_GET_PAD_LOCK (pads)))
#define GST_COLLECT_PADS_PAD_UNLOCK(pads)   (g_mutex_unlock(GST_COLLECT_PADS_GET_PAD_LOCK (pads)))

#define GST_COLLECT_PADS_GET_COND(pads) (((GstCollectPads *)pads)->cond)
#define GST_COLLECT_PADS_WAIT(pads)     (g_cond_wait (GST_COLLECT_PADS_GET_COND (pads), GST_OBJECT_GET_LOCK (pads)))
#define GST_COLLECT_PADS_SIGNAL(pads)   (g_cond_signal (GST_COLLECT_PADS_GET_COND (pads)))
#define GST_COLLECT_PADS_BROADCAST(pads)(g_cond_broadcast (GST_COLLECT_PADS_GET_COND (pads)))

/**
 * GstCollectPads:
 * @data: #GList of #GstCollectData managed by this #GstCollectPads.
 *
 * Collectpads object.
 * Note that @data is only reliable for iterating the list of #GstCollectData
 * when inside the #GstCollectPadsFunction callback.
 */
struct _GstCollectPads {
  GstObject      object;

  /*< public >*/ /* with LOCK */
  GSList        *data;                  /* list of CollectData items */

  /*< private >*/
  guint32        cookie;                /* @data list cookie */

  /* with LOCK */
  GCond         *cond;                  /* to signal removal of data */

  GstCollectPadsFunction func;          /* function and user_data for callback */
  gpointer       user_data;

  guint          numpads;               /* number of pads in @data */
  guint          queuedpads;            /* number of pads with a buffer */
  guint          eospads;               /* number of pads that are EOS */

  /* with LOCK and PAD_LOCK*/
  gboolean       started;

  /*< private >*/
  union {
    struct {
      /* since 0.10.6 */ /* with PAD_LOCK */
      GMutex    *pad_lock;              /* used to serialize add/remove */
      GSList    *pad_list;              /* updated pad list */
      guint32    pad_cookie;            /* updated cookie */
      GstCollectPadsPrivate  *priv;
    } ABI;
    /* adding + 0 to mark ABI change to be undone later */
    gpointer _gst_reserved[GST_PADDING + 0];
  } abidata;
};

struct _GstCollectPadsClass {
  GstObjectClass parent_class;

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

GType gst_collect_pads_get_type(void);

/* creating the object */
GstCollectPads* gst_collect_pads_new            (void);

/* set the callbacks */
void            gst_collect_pads_set_function      (GstCollectPads *pads, GstCollectPadsFunction func,
                                                    gpointer user_data);
void            gst_collect_pads_set_clip_function (GstCollectPads *pads, GstCollectPadsClipFunction clipfunc,
                                                    gpointer user_data);

/* pad management */
GstCollectData* gst_collect_pads_add_pad        (GstCollectPads *pads, GstPad *pad, guint size);
GstCollectData* gst_collect_pads_add_pad_full   (GstCollectPads *pads, GstPad *pad, guint size, GstCollectDataDestroyNotify destroy_notify);


gboolean        gst_collect_pads_remove_pad     (GstCollectPads *pads, GstPad *pad);
gboolean        gst_collect_pads_is_active      (GstCollectPads *pads, GstPad *pad);

/* start/stop collection */
GstFlowReturn   gst_collect_pads_collect        (GstCollectPads *pads);
GstFlowReturn   gst_collect_pads_collect_range  (GstCollectPads *pads, guint64 offset, guint length);

void            gst_collect_pads_start          (GstCollectPads *pads);
void            gst_collect_pads_stop           (GstCollectPads *pads);
void            gst_collect_pads_set_flushing   (GstCollectPads *pads, gboolean flushing);

/* get collected buffers */
GstBuffer*      gst_collect_pads_peek           (GstCollectPads *pads, GstCollectData *data);
GstBuffer*      gst_collect_pads_pop            (GstCollectPads *pads, GstCollectData *data);

/* get collected bytes */
guint           gst_collect_pads_available      (GstCollectPads *pads);
guint           gst_collect_pads_read           (GstCollectPads *pads, GstCollectData *data,
                                                 guint8 **bytes, guint size);
GstBuffer *     gst_collect_pads_read_buffer    (GstCollectPads * pads, GstCollectData * data,
                                                 guint size);
GstBuffer *     gst_collect_pads_take_buffer    (GstCollectPads * pads, GstCollectData * data,
                                                 guint size);
guint           gst_collect_pads_flush          (GstCollectPads *pads, GstCollectData *data,
                                                 guint size);

G_END_DECLS

#endif /* __GST_COLLECT_PADS_H__ */
