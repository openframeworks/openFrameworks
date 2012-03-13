/* GdkPixbuf library - Progressive loader object
 *
 * Copyright (C) 1999 The Free Software Foundation
 *
 * Authors: Mark Crichton <crichton@gimp.org>
 *          Miguel de Icaza <miguel@gnu.org>
 *          Federico Mena-Quintero <federico@gimp.org>
 *          Jonathan Blandford <jrb@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#if defined(GDK_PIXBUF_DISABLE_SINGLE_INCLUDES) && !defined (GDK_PIXBUF_H_INSIDE) && !defined (GDK_PIXBUF_COMPILATION)
#error "Only <gdk-pixbuf/gdk-pixbuf.h> can be included directly."
#endif

#ifndef GDK_PIXBUF_LOADER_H
#define GDK_PIXBUF_LOADER_H

#include <glib.h>
#include <glib-object.h>
#include <gdk-pixbuf/gdk-pixbuf-core.h>
#include <gdk-pixbuf/gdk-pixbuf-animation.h>
#include <gdk-pixbuf/gdk-pixbuf-io.h>

G_BEGIN_DECLS

#define GDK_TYPE_PIXBUF_LOADER		   (gdk_pixbuf_loader_get_type ())
#define GDK_PIXBUF_LOADER(obj)		   (G_TYPE_CHECK_INSTANCE_CAST ((obj), GDK_TYPE_PIXBUF_LOADER, GdkPixbufLoader))
#define GDK_PIXBUF_LOADER_CLASS(klass)	   (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_PIXBUF_LOADER, GdkPixbufLoaderClass))
#define GDK_IS_PIXBUF_LOADER(obj)	   (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GDK_TYPE_PIXBUF_LOADER))
#define GDK_IS_PIXBUF_LOADER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_PIXBUF_LOADER))
#define GDK_PIXBUF_LOADER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_PIXBUF_LOADER, GdkPixbufLoaderClass))


typedef struct _GdkPixbufLoader GdkPixbufLoader;
struct _GdkPixbufLoader
{
  GObject parent_instance;
  
  /*< private >*/
  gpointer priv;
};

typedef struct _GdkPixbufLoaderClass GdkPixbufLoaderClass;
struct _GdkPixbufLoaderClass
{
  GObjectClass parent_class;

  void (*size_prepared)      (GdkPixbufLoader *loader, 
			      int              width,
			      int              height);

  void (*area_prepared)      (GdkPixbufLoader *loader);

  /* Last known frame needs a redraw for x, y, width, height */
  void (*area_updated)       (GdkPixbufLoader *loader,
                              int              x,
                              int              y,
			      int              width,
			      int              height);

  void (*closed)             (GdkPixbufLoader *loader);
};

GType                gdk_pixbuf_loader_get_type      (void) G_GNUC_CONST;
GdkPixbufLoader *    gdk_pixbuf_loader_new           (void);
GdkPixbufLoader *    gdk_pixbuf_loader_new_with_type (const char *image_type,
                                                      GError    **error);
GdkPixbufLoader *    gdk_pixbuf_loader_new_with_mime_type (const char *mime_type,
							   GError    **error);
void                 gdk_pixbuf_loader_set_size (GdkPixbufLoader  *loader,
                                                 int               width,
						 int               height);
gboolean             gdk_pixbuf_loader_write         (GdkPixbufLoader *loader,
						      const guchar    *buf,
						      gsize            count,
                                                      GError         **error);
GdkPixbuf *          gdk_pixbuf_loader_get_pixbuf    (GdkPixbufLoader *loader);
GdkPixbufAnimation * gdk_pixbuf_loader_get_animation (GdkPixbufLoader *loader);
gboolean             gdk_pixbuf_loader_close         (GdkPixbufLoader *loader,
                                                      GError         **error);
GdkPixbufFormat     *gdk_pixbuf_loader_get_format    (GdkPixbufLoader *loader);

G_END_DECLS

#endif


