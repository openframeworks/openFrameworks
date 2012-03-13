/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2000 Wim Taymans <wim.taymans@chello.be>
 *
 * gstindexfactory.h: Header for GstIndexFactory, base class to handle efficient
 *                    storage or caching of seeking information.
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

#ifndef __GST_INDEX_FACTORY_H__
#define __GST_INDEX_FACTORY_H__

#include <gst/gstobject.h>
#include <gst/gstformat.h>
#include <gst/gstpluginfeature.h>

G_BEGIN_DECLS

#define GST_TYPE_INDEX_FACTORY  		(gst_index_factory_get_type())
#define GST_INDEX_FACTORY(obj) 			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_INDEX_FACTORY, GstIndexFactory))
#define GST_IS_INDEX_FACTORY(obj) 		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_INDEX_FACTORY))
#define GST_INDEX_FACTORY_CLASS(klass) 		(G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_INDEX_FACTORY, GstIndexFactoryClass))
#define GST_IS_INDEX_FACTORY_CLASS(klass) 	(G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_INDEX_FACTORY))
#define GST_INDEX_FACTORY_GET_CLASS(obj) 	(G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_INDEX_FACTORY, GstIndexFactoryClass))

typedef struct _GstIndexFactory GstIndexFactory;
typedef struct _GstIndexFactoryClass GstIndexFactoryClass;

/**
 * GstIndexFactory:
 *
 * The GstIndexFactory object
 */
struct _GstIndexFactory {
  GstPluginFeature feature;

  gchar *longdesc;            /* long description of the index (well, don't overdo it..) */
  GType type;                 /* unique GType of the index */

  gpointer _gst_reserved[GST_PADDING];
};

struct _GstIndexFactoryClass {
  GstPluginFeatureClass parent;

  gpointer _gst_reserved[GST_PADDING];
};

GType 			gst_index_factory_get_type 	(void);

GstIndexFactory*	gst_index_factory_new 		(const gchar *name,
							 const gchar *longdesc, GType type);
void 			gst_index_factory_destroy	(GstIndexFactory *factory);

GstIndexFactory*	gst_index_factory_find		(const gchar *name);

GstIndex*		gst_index_factory_create 	(GstIndexFactory *factory);
GstIndex*		gst_index_factory_make   	(const gchar *name);

G_END_DECLS

#endif /* __GST_INDEX_FACTORY_H__ */
