/* GStreamer
 *
 * Copyright (C) <2005> Stefan Kost <ensonic at users dot sf dot net>
 *
 * gst-controller.h: dynamic parameter control subsystem
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

#ifndef __GST_CONTROLLER_H__
#define __GST_CONTROLLER_H__

#include <string.h>

#include <glib.h>
#include <glib-object.h>
#include <glib/gprintf.h>
#include <gst/gst.h>

#include "gstcontrolsource.h"
#include "gstinterpolationcontrolsource.h"

G_BEGIN_DECLS

/* type macros */

#define GST_TYPE_CONTROLLER	       (gst_controller_get_type ())
#define GST_CONTROLLER(obj)	       (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_CONTROLLER, GstController))
#define GST_CONTROLLER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_CONTROLLER, GstControllerClass))
#define GST_IS_CONTROLLER(obj)	       (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_CONTROLLER))
#define GST_IS_CONTROLLER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_CONTROLLERE))
#define GST_CONTROLLER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_CONTROLLER, GstControllerClass))

typedef struct _GstController GstController;
typedef struct _GstControllerClass GstControllerClass;
typedef struct _GstControllerPrivate GstControllerPrivate;

/**
 * GstController:
 *
 * The instance structure of GstController
 */

struct _GstController
{
  GObject parent;

  GList *properties;  /* List of GstControlledProperty */
  GMutex *lock;       /* Secure property access, elements will access from threads */
  GObject *object;    /* the object we control */

  /*< private >*/
  GstControllerPrivate *priv;
  gpointer       _gst_reserved[GST_PADDING - 1];
};

struct _GstControllerClass
{
  GObjectClass parent_class;

  /*< private >*/
  gpointer       _gst_reserved[GST_PADDING];
};

GType gst_controller_get_type (void);

/* GstController functions */

GstController *gst_controller_new_valist (GObject * object, va_list var_args);
GstController *gst_controller_new_list (GObject * object, GList *list);
GstController *gst_controller_new (GObject * object, ...) G_GNUC_NULL_TERMINATED;

gboolean gst_controller_remove_properties_valist (GstController * self,
    va_list var_args);
gboolean gst_controller_remove_properties_list (GstController * self,
						GList *list);
gboolean gst_controller_remove_properties (GstController * self, ...) G_GNUC_NULL_TERMINATED;

void gst_controller_set_disabled (GstController *self, gboolean disabled);
void gst_controller_set_property_disabled (GstController *self, const gchar * property_name, gboolean disabled);
gboolean gst_controller_set_control_source (GstController *self, const gchar * property_name, GstControlSource *csource);
GstControlSource * gst_controller_get_control_source (GstController *self, const gchar * property_name);

GstClockTime gst_controller_suggest_next_sync (GstController *self);
gboolean gst_controller_sync_values (GstController * self,
    GstClockTime timestamp);

GValue *gst_controller_get (GstController * self, const gchar * property_name,
    GstClockTime timestamp);
gboolean gst_controller_get_value_arrays (GstController * self,
    GstClockTime timestamp, GSList * value_arrays);
gboolean gst_controller_get_value_array (GstController * self,
    GstClockTime timestamp, GstValueArray * value_array);

/* GObject convenience functions */

GstController *gst_object_control_properties (GObject * object, ...) G_GNUC_NULL_TERMINATED;
gboolean gst_object_uncontrol_properties (GObject * object, ...) G_GNUC_NULL_TERMINATED;

GstController *gst_object_get_controller (GObject * object);
gboolean gst_object_set_controller (GObject * object, GstController * controller);

GstClockTime gst_object_suggest_next_sync (GObject * object);
gboolean gst_object_sync_values (GObject * object, GstClockTime timestamp);

gboolean gst_object_set_control_source (GObject *object, const gchar * property_name, GstControlSource *csource);
GstControlSource * gst_object_get_control_source (GObject *object, const gchar * property_name);

gboolean gst_object_get_value_arrays (GObject * object,
    GstClockTime timestamp, GSList * value_arrays);
gboolean gst_object_get_value_array (GObject * object,
    GstClockTime timestamp, GstValueArray * value_array);

GstClockTime gst_object_get_control_rate (GObject * object);
void gst_object_set_control_rate (GObject * object, GstClockTime control_rate);

/* lib init/done */

gboolean gst_controller_init (int * argc, char ***argv);


/* FIXME: deprecated functions */
#ifndef GST_DISABLE_DEPRECATED
gboolean gst_controller_set (GstController * self, const gchar * property_name,
    GstClockTime timestamp, GValue * value);
gboolean gst_controller_set_from_list (GstController * self,
    const gchar * property_name, GSList * timedvalues);

gboolean gst_controller_unset (GstController * self, const gchar * property_name,
    GstClockTime timestamp);
gboolean gst_controller_unset_all (GstController * self, const gchar * property_name);

const GList *gst_controller_get_all (GstController * self,
    const gchar * property_name);

gboolean gst_controller_set_interpolation_mode (GstController * self,
    const gchar * property_name, GstInterpolateMode mode);
#endif /* GST_DISABLE_DEPRECATED */

G_END_DECLS

#endif /* __GST_CONTROLLER_H__ */
