/* GdkPixbuf library - transformations
 *
 * Copyright (C) 2003 The Free Software Foundation
 *
 * Authors: Mark Crichton <crichton@gimp.org>
 *          Miguel de Icaza <miguel@gnu.org>
 *          Federico Mena-Quintero <federico@gimp.org>
 *          Havoc Pennington <hp@redhat.com>
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

#ifndef GDK_PIXBUF_TRANSFORM_H
#define GDK_PIXBUF_TRANSFORM_H

#include <glib.h>
#include <gdk-pixbuf/gdk-pixbuf-core.h>


G_BEGIN_DECLS

/* Scaling */

/* Interpolation modes */
typedef enum {
	GDK_INTERP_NEAREST,
	GDK_INTERP_TILES,
	GDK_INTERP_BILINEAR,
	GDK_INTERP_HYPER
} GdkInterpType;

typedef enum {
	GDK_PIXBUF_ROTATE_NONE             =   0,
	GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE =  90,
	GDK_PIXBUF_ROTATE_UPSIDEDOWN       = 180,
	GDK_PIXBUF_ROTATE_CLOCKWISE        = 270
} GdkPixbufRotation;

void gdk_pixbuf_scale           (const GdkPixbuf *src,
				 GdkPixbuf       *dest,
				 int              dest_x,
				 int              dest_y,
				 int              dest_width,
				 int              dest_height,
				 double           offset_x,
				 double           offset_y,
				 double           scale_x,
				 double           scale_y,
				 GdkInterpType    interp_type);
void gdk_pixbuf_composite       (const GdkPixbuf *src,
				 GdkPixbuf       *dest,
				 int              dest_x,
				 int              dest_y,
				 int              dest_width,
				 int              dest_height,
				 double           offset_x,
				 double           offset_y,
				 double           scale_x,
				 double           scale_y,
				 GdkInterpType    interp_type,
				 int              overall_alpha);
void gdk_pixbuf_composite_color (const GdkPixbuf *src,
				 GdkPixbuf       *dest,
				 int              dest_x,
				 int              dest_y,
				 int              dest_width,
				 int              dest_height,
				 double           offset_x,
				 double           offset_y,
				 double           scale_x,
				 double           scale_y,
				 GdkInterpType    interp_type,
				 int              overall_alpha,
				 int              check_x,
				 int              check_y,
				 int              check_size,
				 guint32          color1,
				 guint32          color2);

GdkPixbuf *gdk_pixbuf_scale_simple           (const GdkPixbuf *src,
					      int              dest_width,
					      int              dest_height,
					      GdkInterpType    interp_type);

GdkPixbuf *gdk_pixbuf_composite_color_simple (const GdkPixbuf *src,
					      int              dest_width,
					      int              dest_height,
					      GdkInterpType    interp_type,
					      int              overall_alpha,
					      int              check_size,
					      guint32          color1,
					      guint32          color2);

GdkPixbuf *gdk_pixbuf_rotate_simple          (const GdkPixbuf   *src,
				              GdkPixbufRotation  angle);
GdkPixbuf *gdk_pixbuf_flip                   (const GdkPixbuf   *src,
				              gboolean           horizontal);
				     
G_END_DECLS


#endif  /* GDK_PIXBUF_TRANSFORM_H */
