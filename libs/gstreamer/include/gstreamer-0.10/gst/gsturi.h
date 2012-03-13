/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wtay@chello.be>
 *
 * gsturi.h: Header for uri to element mappings
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


#ifndef __GST_URI_H__
#define __GST_URI_H__

#include <glib.h>
#include <gst/gstelement.h>
#include <gst/gstpluginfeature.h>

G_BEGIN_DECLS

/**
 * GstURIType:
 * @GST_URI_UNKNOWN	: The URI direction is unknown
 * @GST_URI_SINK	: The URI is a consumer.
 * @GST_URI_SRC		: The URI is a producer.
 *
 * The different types of URI direction.
 */

typedef enum {
  GST_URI_UNKNOWN,
  GST_URI_SINK,
  GST_URI_SRC
} GstURIType;

/**
 * GST_URI_TYPE_IS_VALID:
 * @type: A #GstURIType
 *
 * Tests if the type direction is valid.
 */
#define GST_URI_TYPE_IS_VALID(type) ((type) == GST_URI_SRC || (type) == GST_URI_SINK)

/* uri handler functions */
#define GST_TYPE_URI_HANDLER		(gst_uri_handler_get_type ())
#define GST_URI_HANDLER(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_URI_HANDLER, GstURIHandler))
#define GST_IS_URI_HANDLER(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_URI_HANDLER))
#define GST_URI_HANDLER_GET_INTERFACE(obj)	(G_TYPE_INSTANCE_GET_INTERFACE ((obj), GST_TYPE_URI_HANDLER, GstURIHandlerInterface))

/**
 * GstURIHandler:
 *
 * Opaque #GstURIHandler structure.
 */
typedef struct _GstURIHandler GstURIHandler;
typedef struct _GstURIHandlerInterface GstURIHandlerInterface;

/**
 * GstURIHandlerInterface:
 * @parent: The parent interface type
 * @get_type: Method to tell whether the element handles source or sink URI.
 * @get_protocols: Method to return the list of protocols handled by the element.
 * @get_uri: Method to return the URI currently handled by the element.
 * @set_uri: Method to set a new URI.
 * @get_type_full: Variant of get_type which takes a GType argument. This is 
 *  for use by bindings that need to pass context when creating a URI Handler.  *  If implemented, get_type will be used in preference to get_type_full. Since: 0.10.15.
 * @get_protocols_full: Variant of get_type which takes a GType argument. 
 * This is for use by bindings that need to pass context when creating a URI 
 * Handler. If implemented, get_protocols will be used in preference to 
 * get_protocols_full. Since: 0.10.15.
 *
 * Any #GstElement using this interface should implement these methods.
 */
struct _GstURIHandlerInterface {
  GTypeInterface	parent;

  /*< private >*/
  /* signals */
  void		(* new_uri)			(GstURIHandler * handler,
						 const gchar *   uri);
  /* idea for the future ?
  gboolean	(* require_password)		(GstURIHandler * handler,
						 gchar **	 username,
						 gchar **	 password);
   */

  /* vtable */

  /*< public >*/
  /* querying capabilities */
  GstURIType		(* get_type)		(void);
  gchar **		(* get_protocols)	(void);

  /* using the interface */
  G_CONST_RETURN gchar *(* get_uri)		(GstURIHandler * handler);
  gboolean		(* set_uri)		(GstURIHandler * handler,
						 const gchar *	 uri);

  GstURIType		(* get_type_full)	(GType type);
  gchar **		(* get_protocols_full)	(GType type);

  /*< private >*/
  /* we might want to add functions here to query features,
   * someone with gnome-vfs knowledge go ahead */

  gpointer _gst_reserved[GST_PADDING - 2];
};

/* general URI functions */

gboolean	gst_uri_protocol_is_valid	(const gchar * protocol);
gboolean	gst_uri_protocol_is_supported	(const GstURIType type,
						 const gchar *protocol);
gboolean	gst_uri_is_valid		(const gchar * uri);
gchar *		gst_uri_get_protocol		(const gchar * uri);
gboolean        gst_uri_has_protocol            (const gchar * uri,
                                                 const gchar * protocol);
gchar *		gst_uri_get_location		(const gchar * uri);
gchar *		gst_uri_construct		(const gchar * protocol,
						 const gchar * location);

GstElement *	gst_element_make_from_uri	(const GstURIType type,
						 const gchar *    uri,
						 const gchar *    elementname);

/* accessing the interface */
GType		gst_uri_handler_get_type	(void);

guint		gst_uri_handler_get_uri_type	(GstURIHandler * handler);
gchar **	gst_uri_handler_get_protocols	(GstURIHandler * handler);
G_CONST_RETURN
gchar *		gst_uri_handler_get_uri		(GstURIHandler * handler);
gboolean	gst_uri_handler_set_uri		(GstURIHandler * handler,
						 const gchar *	 uri);
void		gst_uri_handler_new_uri		(GstURIHandler * handler,
						 const gchar *	 uri);

G_END_DECLS

#endif /* __GST_URI_H__ */
