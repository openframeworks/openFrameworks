/* GStreamer
 *
 * Copyright (C) 2007 Sebastian Dröge <slomo@circular-chaos.org>
 *
 * gstcontrolsource.h: Interface declaration for control sources
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

#ifndef __GST_CONTROL_SOURCE_H__
#define __GST_CONTROL_SOURCE_H__

#include <glib-object.h>
#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_TYPE_CONTROL_SOURCE \
  (gst_control_source_get_type())
#define GST_CONTROL_SOURCE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_CONTROL_SOURCE,GstControlSource))
#define GST_CONTROL_SOURCE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_CONTROL_SOURCE,GstControlSourceClass))
#define GST_IS_CONTROL_SOURCE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_CONTROL_SOURCE))
#define GST_IS_CONTROL_SOURCE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_CONTROL_SOURCE))
#define GST_CONTROL_SOURCE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_CONTOL_SOURCE, GstControlSourceClass))

typedef struct _GstControlSource GstControlSource;
typedef struct _GstControlSourceClass GstControlSourceClass;
typedef struct _GstTimedValue GstTimedValue;
typedef struct _GstValueArray GstValueArray;

/**
 * GstTimedValue:
 * @timestamp: timestamp of the value change
 * @value: the corresponding value
 *
 * Structure for saving a timestamp and a value.
 */
struct _GstTimedValue
{
  GstClockTime timestamp;
  GValue value;
};

/**
 * GstValueArray:
 * @property_name: the name of the property this array belongs to
 * @nbsamples: number of samples requested
 * @sample_interval: interval between each sample
 * @values: pointer to the array
 *
 * Structure to receive multiple values at once.
 */
struct _GstValueArray
{
  const gchar *property_name;
  gint nbsamples;
  GstClockTime sample_interval;
  gpointer *values;
};

/** 
 * GstControlSourceGetValue
 * @self: the #GstControlSource instance
 * @timestamp: timestamp for which a value should be calculated
 * @value: a #GValue which will be set to the result. It must be initialized to the correct type.
 *
 * Function for returning a value for a given timestamp.
 *
 * Returns: %TRUE if the value was successfully calculated.
 *
 */
typedef gboolean (* GstControlSourceGetValue) (GstControlSource *self, GstClockTime timestamp, GValue *value);

/** 
 * GstControlSourceGetValueArray
 * @self: the #GstControlSource instance
 * @timestamp: timestamp for which a value should be calculated
 * @value_array: array to put control-values in
 *
 * Function for returning a #GstValueArray for a given timestamp.
 *
 * Returns: %TRUE if the values were successfully calculated.
 *
 */
typedef gboolean (* GstControlSourceGetValueArray) (GstControlSource *self, GstClockTime timestamp, GstValueArray *value_array);

/** 
 * GstControlSourceBind
 * @self: the #GstControlSource instance
 * @pspec: #GParamSpec that should be bound to
 *
 * Function for binding a #GstControlSource to a #GParamSpec.
 *
 * Returns: %TRUE if the property could be bound to the #GstControlSource, %FALSE otherwise.
 */
typedef gboolean (* GstControlSourceBind) (GstControlSource *self, GParamSpec *pspec);

/**
 * GstControlSource:
 * @get_value: Function for returning a value for a given timestamp
 * @get_value_array: Function for returning a #GstValueArray for a given timestamp
 *
 * The instance structure of #GstControlSource.
 */
struct _GstControlSource {
  GObject parent;

  /*< public >*/
  GstControlSourceGetValue get_value;             /* Returns the value for a property at a given timestamp */
  GstControlSourceGetValueArray get_value_array;  /* Returns values for a property in a given timespan */

  /*< private >*/
  gboolean bound;
  gpointer _gst_reserved[GST_PADDING];
};

/**
 * GstControlSourceClass:
 * @parent_class: Parent class
 * @bind: Class method for binding the #GstControlSource to a specific GParamSpec
 *
 * The class structure of #GstControlSource.
 */

struct _GstControlSourceClass
{
  GObjectClass parent_class;
  
  GstControlSourceBind bind;  /* Binds the GstControlSource to a specific GParamSpec */

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

GType gst_control_source_get_type (void);

/* Functions */

gboolean gst_control_source_get_value (GstControlSource *self, GstClockTime timestamp, GValue *value);
gboolean gst_control_source_get_value_array (GstControlSource *self, GstClockTime timestamp, GstValueArray *value_array);
gboolean gst_control_source_bind (GstControlSource *self, GParamSpec *pspec);

G_END_DECLS

#endif /* __GST_CONTROL_SOURCE_H__ */
