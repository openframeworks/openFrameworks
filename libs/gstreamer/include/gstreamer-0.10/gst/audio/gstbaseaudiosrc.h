/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2005 Wim Taymans <wim@fluendo.com>
 *
 * gstbaseaudiosrc.h:
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

/* a base class for audio sources.
 */

#ifndef __GST_BASE_AUDIO_SRC_H__
#define __GST_BASE_AUDIO_SRC_H__

#include <gst/gst.h>
#include <gst/base/gstpushsrc.h>
#include "gstringbuffer.h"
#include "gstaudioclock.h"

G_BEGIN_DECLS

#define GST_TYPE_BASE_AUDIO_SRC                 (gst_base_audio_src_get_type())
#define GST_BASE_AUDIO_SRC(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_BASE_AUDIO_SRC,GstBaseAudioSrc))
#define GST_BASE_AUDIO_SRC_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_BASE_AUDIO_SRC,GstBaseAudioSrcClass))
#define GST_BASE_AUDIO_SRC_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_BASE_AUDIO_SRC, GstBaseAudioSrcClass))
#define GST_IS_BASE_AUDIO_SRC(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_BASE_AUDIO_SRC))
#define GST_IS_BASE_AUDIO_SRC_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_BASE_AUDIO_SRC))

/**
 * GST_BASE_AUDIO_SRC_CLOCK:
 * @obj: a #GstBaseAudioSrc
 *
 * Get the #GstClock of @obj.
 */
#define GST_BASE_AUDIO_SRC_CLOCK(obj)    (GST_BASE_AUDIO_SRC (obj)->clock)
/**
 * GST_BASE_AUDIO_SRC_PAD:
 * @obj: a #GstBaseAudioSrc
 *
 * Get the source #GstPad of @obj.
 */
#define GST_BASE_AUDIO_SRC_PAD(obj)      (GST_BASE_SRC (obj)->srcpad)

typedef struct _GstBaseAudioSrc GstBaseAudioSrc;
typedef struct _GstBaseAudioSrcClass GstBaseAudioSrcClass;
typedef struct _GstBaseAudioSrcPrivate GstBaseAudioSrcPrivate;

/**
 * GstBaseAudioSrcSlaveMethod:
 * @GST_BASE_AUDIO_SRC_SLAVE_RESAMPLE: Resample to match the master clock. 
 * @GST_BASE_AUDIO_SRC_SLAVE_RETIMESTAMP: Retimestamp output buffers with master
 * clock time.
 * @GST_BASE_AUDIO_SRC_SLAVE_SKEW: Adjust capture pointer when master clock
 * drifts too much.
 * @GST_BASE_AUDIO_SRC_SLAVE_NONE: No adjustment is done. 
 *
 * Different possible clock slaving algorithms when the internal audio clock was
 * not selected as the pipeline clock.
 */
typedef enum
{
  GST_BASE_AUDIO_SRC_SLAVE_RESAMPLE,
  GST_BASE_AUDIO_SRC_SLAVE_RETIMESTAMP,
  GST_BASE_AUDIO_SRC_SLAVE_SKEW,
  GST_BASE_AUDIO_SRC_SLAVE_NONE
} GstBaseAudioSrcSlaveMethod;

#define GST_TYPE_BASE_AUDIO_SRC_SLAVE_METHOD (gst_base_audio_src_slave_method_get_type ())

/**
 * GstBaseAudioSrc:
 *
 * Opaque #GstBaseAudioSrc.
 */
struct _GstBaseAudioSrc {
  GstPushSrc     element;

  /*< protected >*/ /* with LOCK */
  /* our ringbuffer */
  GstRingBuffer *ringbuffer;

  /* required buffer and latency */
  GstClockTime   buffer_time;
  GstClockTime   latency_time;

  /* the next sample to write */
  guint64        next_sample;

  /* clock */
  GstClock      *clock;

  /*< private >*/
  GstBaseAudioSrcPrivate *priv;

  gpointer _gst_reserved[GST_PADDING - 1];
};

/**
 * GstBaseAudioSrcClass:
 * @parent_class: the parent class.
 * @create_ringbuffer: create and return a #GstRingBuffer to read from.
 *
 * #GstBaseAudioSrc class. Override the vmethod to implement
 * functionality.
 */
struct _GstBaseAudioSrcClass {
  GstPushSrcClass parent_class;

  /* subclass ringbuffer allocation */
  GstRingBuffer* (*create_ringbuffer)  (GstBaseAudioSrc *src);

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

GType gst_base_audio_src_get_type(void);
GType gst_base_audio_src_slave_method_get_type (void);

GstRingBuffer *gst_base_audio_src_create_ringbuffer (GstBaseAudioSrc *src);

void       gst_base_audio_src_set_provide_clock        (GstBaseAudioSrc *src, gboolean provide);
gboolean   gst_base_audio_src_get_provide_clock        (GstBaseAudioSrc *src);

void       gst_base_audio_src_set_slave_method         (GstBaseAudioSrc *src,
                                                        GstBaseAudioSrcSlaveMethod method);
GstBaseAudioSrcSlaveMethod
           gst_base_audio_src_get_slave_method         (GstBaseAudioSrc *src);


G_END_DECLS

#endif /* __GST_BASE_AUDIO_SRC_H__ */
