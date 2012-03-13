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


#ifndef __GST_AUDIO_FILTER_H__
#define __GST_AUDIO_FILTER_H__

#include <gst/gst.h>
#include <gst/base/gstbasetransform.h>
#include <gst/audio/gstringbuffer.h>

G_BEGIN_DECLS

typedef struct _GstAudioFilter GstAudioFilter;
typedef struct _GstAudioFilterClass GstAudioFilterClass;

#define GST_TYPE_AUDIO_FILTER \
  (gst_audio_filter_get_type())
#define GST_AUDIO_FILTER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_AUDIO_FILTER,GstAudioFilter))
#define GST_AUDIO_FILTER_CAST(obj) \
  ((GstAudioFilter *) (obj))
#define GST_AUDIO_FILTER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_AUDIO_FILTER,GstAudioFilterClass))
#define GST_AUDIO_FILTER_CLASS_CAST(klass) \
  ((GstAudioFilterClass *) (klass))
#define GST_IS_AUDIO_FILTER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_AUDIO_FILTER))
#define GST_IS_AUDIO_FILTER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_AUDIO_FILTER))

/**
 * GstAudioFilter:
 * @basetransform: Element parent class
 *
 * Base class for audio filters with the same format for input and output.
 *
 * Since: 0.10.12
 */
struct _GstAudioFilter {
  GstBaseTransform basetransform;

  /*< protected >*/
  GstRingBufferSpec format;   /* currently configured format */

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

/**
 * GstAudioFilterClass:
 * @basetransformclass: parent class
 * @setup: virtual function called whenever the format changes
 *
 * In addition to the @setup virtual function, you should also override the
 * GstBaseTransform::transform and/or GstBaseTransform::transform_ip virtual
 * function.
 *
 * Since: 0.10.12
 */

struct _GstAudioFilterClass {
  GstBaseTransformClass basetransformclass;

  /* virtual function, called whenever the format changes */
  gboolean  (*setup) (GstAudioFilter * filter, GstRingBufferSpec * format);

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

GType   gst_audio_filter_get_type (void);

void    gst_audio_filter_class_add_pad_templates (GstAudioFilterClass * klass,
                                                  const GstCaps       * allowed_caps);

G_END_DECLS

#endif /* __GST_AUDIO_FILTER_H__ */

