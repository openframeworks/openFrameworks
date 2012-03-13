/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wtay@chello.be>
 *
 * gstinterface.h: Declarations of interface stuff
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

#ifndef __GST_IMPLEMENTS_INTERFACE_H__
#define __GST_IMPLEMENTS_INTERFACE_H__

#include <gst/gstelement.h>

G_BEGIN_DECLS

#define GST_TYPE_IMPLEMENTS_INTERFACE \
  (gst_implements_interface_get_type ())
#define GST_IMPLEMENTS_INTERFACE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_IMPLEMENTS_INTERFACE, \
				      GstImplementsInterface))
#define GST_IS_IMPLEMENTS_INTERFACE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_IMPLEMENTS_INTERFACE))
#define GST_IMPLEMENTS_INTERFACE_GET_CLASS(inst) \
  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), GST_TYPE_IMPLEMENTS_INTERFACE, \
				  GstImplementsInterfaceClass))

/**
 * GstImplementsInterface:
 *
 * Opaque #GstImplementsInterface structure.
 */
typedef struct _GstImplementsInterface GstImplementsInterface;

typedef struct _GstImplementsInterfaceClass GstImplementsInterfaceClass;

/* This small extra virtual function is here to provide an
 * interface functionality on a per-instance basis rather
 * than a per-class basis, which is the case for glib.
 */
struct _GstImplementsInterfaceClass {
  GTypeInterface parent;

  /* virtual functions */
  gboolean (* supported) (GstImplementsInterface *iface,
			  GType                   iface_type);

  gpointer _gst_reserved[GST_PADDING];
};

#define GST_IMPLEMENTS_INTERFACE_CHECK_INSTANCE_CAST(obj, type, cast_t) \
  ((cast_t *) gst_implements_interface_cast ((obj), (type)))
#define GST_IMPLEMENTS_INTERFACE_CHECK_INSTANCE_TYPE(obj, type) \
  (gst_implements_interface_check ((obj), (type)))

GType	 gst_implements_interface_get_type    (void);

/* wrapper functions to check for functionality implementation */
gboolean gst_element_implements_interface     (GstElement *element,
					       GType       iface_type);
gpointer gst_implements_interface_cast	      (gpointer    from,
					       GType       type);
gboolean gst_implements_interface_check	      (gpointer    from,
					       GType       type);

G_END_DECLS

#endif /* __GST_IMPLEMENTS_INTERFACE_H__ */
