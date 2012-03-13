/* GStreamer
 *
 * Copyright (C) 2007 Sebastian Dröge <slomo@circular-chaos.org>
 *
 * gstinterpolationcontrolsource.h: Control source that provides several
 *                                  interpolation methods
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

#ifndef __GST_INTERPOLATION_CONTROL_SOURCE_H__
#define __GST_INTERPOLATION_CONTROL_SOURCE_H__

#include <glib-object.h>
#include <gst/gst.h>

#include "gstcontrolsource.h"

G_BEGIN_DECLS

#define GST_TYPE_INTERPOLATION_CONTROL_SOURCE \
  (gst_interpolation_control_source_get_type ())
#define GST_INTERPOLATION_CONTROL_SOURCE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_INTERPOLATION_CONTROL_SOURCE, GstInterpolationControlSource))
#define GST_INTERPOLATION_CONTROL_SOURCE_CLASS(vtable) \
  (G_TYPE_CHECK_CLASS_CAST ((vtable), GST_TYPE_INTERPOLATION_CONTROL_SOURCE, GstInterpolationControlSourceClass))
#define GST_IS_INTERPOLATION_CONTROL_SOURCE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_INTERPOLATION_CONTROL_SOURCE))
#define GST_IS_INTERPOLATION_CONTROL_SOURCE_CLASS(vtable) \
  (G_TYPE_CHECK_CLASS_TYPE ((vtable), GST_TYPE_INTERPOLATION_CONTROL_SOURCE))
#define GST_INTERPOLATION_CONTROL_SOURCE_GET_CLASS(inst) \
  (G_TYPE_INSTANCE_GET_CLASS ((inst), GST_TYPE_INTERPOLATION_CONTROL_SOURCE, GstInterpolationControlSourceClass))

typedef struct _GstInterpolationControlSource GstInterpolationControlSource;
typedef struct _GstInterpolationControlSourceClass GstInterpolationControlSourceClass;
typedef struct _GstInterpolationControlSourcePrivate GstInterpolationControlSourcePrivate;

/**
 * GstInterpolateMode:
 * @GST_INTERPOLATE_NONE: steps-like interpolation, default
 * @GST_INTERPOLATE_TRIGGER: returns the default value of the property,
 * except for times with specific values
 * @GST_INTERPOLATE_LINEAR: linear interpolation
 * @GST_INTERPOLATE_QUADRATIC: square interpolation (deprecated, maps to cubic)
 * @GST_INTERPOLATE_CUBIC: cubic interpolation
 * @GST_INTERPOLATE_USER: user-provided interpolation (not yet available)
 *
 * The various interpolation modes available.
 */
typedef enum
{
  GST_INTERPOLATE_NONE,
  GST_INTERPOLATE_TRIGGER,
  GST_INTERPOLATE_LINEAR,
  GST_INTERPOLATE_QUADRATIC,
  GST_INTERPOLATE_CUBIC,
  GST_INTERPOLATE_USER
} GstInterpolateMode;

/**
 * GstInterpolationControlSource:
 *
 * The instance structure of #GstControlSource.
 */
struct _GstInterpolationControlSource {
  GstControlSource parent;

  /* <private> */
  GMutex *lock;
  GstInterpolationControlSourcePrivate *priv;
  gpointer _gst_reserved[GST_PADDING];
};

struct _GstInterpolationControlSourceClass {
  GstControlSourceClass parent_class;
  
  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

GType gst_interpolation_control_source_get_type (void);

/* Functions */

GstInterpolationControlSource *gst_interpolation_control_source_new (void);
gboolean gst_interpolation_control_source_set_interpolation_mode (GstInterpolationControlSource *self, GstInterpolateMode mode);
gboolean gst_interpolation_control_source_set (GstInterpolationControlSource * self, GstClockTime timestamp, const GValue * value);
gboolean gst_interpolation_control_source_set_from_list (GstInterpolationControlSource * self, const GSList * timedvalues);
gboolean gst_interpolation_control_source_unset (GstInterpolationControlSource * self, GstClockTime timestamp);
void gst_interpolation_control_source_unset_all (GstInterpolationControlSource *self);
GList *gst_interpolation_control_source_get_all (GstInterpolationControlSource * self);
gint gst_interpolation_control_source_get_count (GstInterpolationControlSource * self);

G_END_DECLS

#endif /* __GST_INTERPOLATION_CONTROL_SOURCE_H__ */
