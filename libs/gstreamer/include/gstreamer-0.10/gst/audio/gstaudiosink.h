/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2005 Wim Taymans <wim@fluendo.com>
 *
 * gstaudiosink.h:
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

#ifndef __GST_AUDIO_SINK_H__
#define __GST_AUDIO_SINK_H__

#include <gst/gst.h>
#include <gst/audio/gstbaseaudiosink.h>

G_BEGIN_DECLS

#define GST_TYPE_AUDIO_SINK             (gst_audio_sink_get_type())
#define GST_AUDIO_SINK(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_AUDIO_SINK,GstAudioSink))
#define GST_AUDIO_SINK_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_AUDIO_SINK,GstAudioSinkClass))
#define GST_AUDIO_SINK_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj),GST_TYPE_AUDIO_SINK,GstAudioSinkClass))
#define GST_IS_AUDIO_SINK(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_AUDIO_SINK))
#define GST_IS_AUDIO_SINK_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_AUDIO_SINK))

typedef struct _GstAudioSink GstAudioSink;
typedef struct _GstAudioSinkClass GstAudioSinkClass;

/**
 * GstAudioSink:
 * 
 * Opaque #GstAudioSink.
 */
struct _GstAudioSink {
  GstBaseAudioSink       element;

  /*< private >*/ /* with LOCK */
  GThread   *thread;

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

/**
 * GstAudioSinkClass:
 * @parent_class: the parent class structure.
 * @open: Open the device. No configuration needs to be done at this point.
 *        This function is also used to check if the device is available.
 * @prepare: Prepare the device to operate with the specified parameters.
 * @unprepare: Undo operations done in prepare.
 * @close: Close the device.
 * @write: Write data to the device.
 * @delay: Return how many samples are still in the device. This is used to
 *         drive the synchronisation.
 * @reset: Returns as quickly as possible from a write and flush any pending
 *         samples from the device.
 *
 * #GstAudioSink class. Override the vmethods to implement functionality.
 */
struct _GstAudioSinkClass {
  GstBaseAudioSinkClass parent_class;

  /* vtable */

  /* open the device with given specs */
  gboolean (*open)      (GstAudioSink *sink);
  /* prepare resources and state to operate with the given specs */
  gboolean (*prepare)   (GstAudioSink *sink, GstRingBufferSpec *spec);
  /* undo anything that was done in prepare() */
  gboolean (*unprepare) (GstAudioSink *sink);
  /* close the device */
  gboolean (*close)     (GstAudioSink *sink);
  /* write samples to the device */
  /* FIXME 0.11: change return value to gint, as most implementation use that
   * already anyway */
  guint    (*write)     (GstAudioSink *sink, gpointer data, guint length);
  /* get number of samples queued in the device */
  guint    (*delay)     (GstAudioSink *sink);
  /* reset the audio device, unblock from a write */
  void     (*reset)     (GstAudioSink *sink);

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

GType gst_audio_sink_get_type(void);

G_END_DECLS

#endif /* __GST_AUDIO_SINK_H__ */
