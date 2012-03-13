/* GStreamer Tuner
 * Copyright (C) 2003 Ronald Bultje <rbultje@ronald.bitfreak.net>
 *
 * tunernorm.h: tuner norm object design
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

#ifndef __GST_TUNER_NORM_H__
#define __GST_TUNER_NORM_H__

#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_TYPE_TUNER_NORM \
  (gst_tuner_norm_get_type ())
#define GST_TUNER_NORM(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_TUNER_NORM, GstTunerNorm))
#define GST_TUNER_NORM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_TUNER_NORM, GstTunerNormClass))
#define GST_IS_TUNER_NORM(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_TUNER_NORM))
#define GST_IS_TUNER_NORM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_TUNER_NORM))

typedef struct _GstTunerNorm GstTunerNorm;
typedef struct _GstTunerNormClass GstTunerNormClass;

/**
 * GstTunerNorm:
 * @label: A string containing a descriptive name for the norm
 * @framerate: A GValue containing the framerate associated with this norm,
 *             if any. (May be unset).
 */
struct _GstTunerNorm {
  GObject parent;

  /*< public >*/
  gchar  *label;
  GValue  framerate;
};

struct _GstTunerNormClass {
  GObjectClass parent;

  gpointer _gst_reserved[GST_PADDING];
};

GType           gst_tuner_norm_get_type         (void);

G_END_DECLS

#endif /* __GST_TUNER_NORM_H__ */
