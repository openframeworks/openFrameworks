/* GStreamer PropertyProbe
 * Copyright (C) 2003 David A. Schleef <ds@schleef.org>
 *
 * property_probe.h: property_probe interface design
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

#ifndef __GST_PROPERTY_PROBE_H__
#define __GST_PROPERTY_PROBE_H__

#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_TYPE_PROPERTY_PROBE \
  (gst_property_probe_get_type ())
#define GST_PROPERTY_PROBE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_PROPERTY_PROBE, GstPropertyProbe))
#define GST_IS_PROPERTY_PROBE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_PROPERTY_PROBE))
#define GST_PROPERTY_PROBE_GET_IFACE(obj) \
  (G_TYPE_INSTANCE_GET_INTERFACE ((obj), GST_TYPE_PROPERTY_PROBE, GstPropertyProbeInterface))

/**
 * GstPropertyProbe:
 *
 * Opaque #GstPropertyProbe data structure.
 */
typedef struct _GstPropertyProbe GstPropertyProbe; /* dummy typedef */
typedef struct _GstPropertyProbeInterface GstPropertyProbeInterface;

/**
 * GstPropertyProbeInterface:
 * @klass: parent interface type.
 * @probe_needed: default signal handler
 * @get_properties: virtual method to get list of probable properties
 * @needs_probe: virtual method to tell if probe need update
 * @probe_property: virtual method to probe a property
 * @get_values: virtual method to get probe results for a property
 *
 * #GstPropertyProbe interface.
 */
struct _GstPropertyProbeInterface {
  GTypeInterface klass;

  /* signals */
  void          (*probe_needed)   (GstPropertyProbe *probe,
                                   const GParamSpec *pspec);

  /* virtual functions */
  const GList * (*get_properties) (GstPropertyProbe *probe);
  gboolean      (*needs_probe)    (GstPropertyProbe *probe,
                                   guint             prop_id,
                                   const GParamSpec *pspec);
  void          (*probe_property) (GstPropertyProbe *probe,
                                   guint             prop_id,
                                   const GParamSpec *pspec);
  GValueArray * (*get_values)     (GstPropertyProbe *probe,
                                   guint             prop_id,
                                   const GParamSpec *pspec);

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

GType        gst_property_probe_get_type       (void);

/* virtual class function wrappers */

/* returns list of GParamSpecs */
const GList *     gst_property_probe_get_properties      (GstPropertyProbe *probe);
const GParamSpec *gst_property_probe_get_property        (GstPropertyProbe *probe,
                                                          const gchar      *name);

/* probe one property */
void              gst_property_probe_probe_property      (GstPropertyProbe *probe,
                                                          const GParamSpec *pspec);
void              gst_property_probe_probe_property_name (GstPropertyProbe *probe,
                                                          const gchar      *name);

/* do we need a probe? */
gboolean          gst_property_probe_needs_probe         (GstPropertyProbe *probe,
                                                          const GParamSpec *pspec);
gboolean          gst_property_probe_needs_probe_name    (GstPropertyProbe *probe,
                                                          const gchar      *name);

/* returns list of GValues */
GValueArray *     gst_property_probe_get_values          (GstPropertyProbe *probe,
                                                          const GParamSpec *pspec);
GValueArray *     gst_property_probe_get_values_name     (GstPropertyProbe *probe,
                                                          const gchar      *name);

/* sugar */
GValueArray *     gst_property_probe_probe_and_get_values (GstPropertyProbe *probe,
                                                          const GParamSpec *pspec);
GValueArray *     gst_property_probe_probe_and_get_values_name (GstPropertyProbe *probe,
                                                          const gchar      *name);

G_END_DECLS

#endif /* __GST_PROPERTY_PROBE_H__ */
