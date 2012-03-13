/* GStreamer
 * Copyright (C) 2005 Stefan Kost <ensonic@users.sf.net>
 *
 * gstchildproxy.h: interface header for multi child elements
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

#ifndef __GST_CHILD_PROXY_H__
#define __GST_CHILD_PROXY_H__

#include <glib-object.h>
#include <gst/gst.h>

G_BEGIN_DECLS


#define GST_TYPE_CHILD_PROXY			(gst_child_proxy_get_type ())
#define GST_CHILD_PROXY(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_CHILD_PROXY, GstChildProxy))
#define GST_IS_CHILD_PROXY(obj)	                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_CHILD_PROXY))
#define GST_CHILD_PROXY_GET_INTERFACE(obj)	(G_TYPE_INSTANCE_GET_INTERFACE ((obj), GST_TYPE_CHILD_PROXY, GstChildProxyInterface))

/**
 * GstChildProxy:
 *
 * Opaque #GstChildProxy data structure.
 */
typedef struct _GstChildProxy GstChildProxy;    /* dummy object */
typedef struct _GstChildProxyInterface GstChildProxyInterface;

/**
 * GstChildProxyInterface:
 * @parent: parent interface type.
 * @get_child_by_index: virtual method to fetch the child
 * @get_children_count: virtual method to get the children count
 *
 * #GstChildProxy interface.
 */
struct _GstChildProxyInterface
{
  GTypeInterface parent;

  /* methods */
  GstObject *(*get_child_by_index) (GstChildProxy * parent, guint index);
  guint (*get_children_count) (GstChildProxy * parent);
  /*< private >*/
  /* signals */
  void (*child_added) (GstChildProxy * parent, GstObject * child);
  void (*child_removed) (GstChildProxy * parent, GstObject * child);

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

GType gst_child_proxy_get_type (void);

GstObject *gst_child_proxy_get_child_by_name (GstChildProxy * parent, const gchar * name);
GstObject *gst_child_proxy_get_child_by_index (GstChildProxy * parent, guint index);
guint gst_child_proxy_get_children_count (GstChildProxy * parent);

gboolean gst_child_proxy_lookup (GstObject *object, const gchar *name,
    GstObject **target, GParamSpec **pspec);
void gst_child_proxy_get_property (GstObject * object, const gchar *name, GValue *value);
void gst_child_proxy_get_valist (GstObject * object,
    const gchar * first_property_name, va_list var_args);
void gst_child_proxy_get (GstObject * object, const gchar * first_property_name,
    ...) G_GNUC_NULL_TERMINATED;
void gst_child_proxy_set_property (GstObject * object, const gchar *name, const GValue *value);
void gst_child_proxy_set_valist (GstObject* object,
    const gchar * first_property_name, va_list var_args);
void gst_child_proxy_set (GstObject * object, const gchar * first_property_name,
    ...) G_GNUC_NULL_TERMINATED;
void gst_child_proxy_child_added (GstObject * object, GstObject *  child);
void gst_child_proxy_child_removed (GstObject * object, GstObject *  child);

G_END_DECLS

#endif /* __GST_CHILD_PROXY_H__ */
