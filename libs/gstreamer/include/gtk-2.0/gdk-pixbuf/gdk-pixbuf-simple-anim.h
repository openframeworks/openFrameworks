/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */
/* GdkPixbuf library - Simple frame-based animations
 *
 * Copyright (C) 2004 Dom Lachowicz
 *
 * Authors: Dom Lachowicz <cinamod@hotmail.com>
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

#ifndef GDK_PIXBUF_SIMPLE_ANIM_H
#define GDK_PIXBUF_SIMPLE_ANIM_H

#include <gdk-pixbuf/gdk-pixbuf-animation.h>

G_BEGIN_DECLS

typedef struct _GdkPixbufSimpleAnim GdkPixbufSimpleAnim;
typedef struct _GdkPixbufSimpleAnimClass GdkPixbufSimpleAnimClass;

#define GDK_TYPE_PIXBUF_SIMPLE_ANIM              (gdk_pixbuf_simple_anim_get_type ())
#define GDK_PIXBUF_SIMPLE_ANIM(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_PIXBUF_SIMPLE_ANIM, GdkPixbufSimpleAnim))
#define GDK_IS_PIXBUF_SIMPLE_ANIM(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_PIXBUF_SIMPLE_ANIM))

#define GDK_PIXBUF_SIMPLE_ANIM_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_PIXBUF_SIMPLE_ANIM, GdkPixbufSimpleAnimClass))
#define GDK_IS_PIXBUF_SIMPLE_ANIM_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_PIXBUF_SIMPLE_ANIM))
#define GDK_PIXBUF_SIMPLE_ANIM_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_PIXBUF_SIMPLE_ANIM, GdkPixbufSimpleAnimClass))

GType gdk_pixbuf_simple_anim_get_type (void) G_GNUC_CONST;
GType gdk_pixbuf_simple_anim_iter_get_type (void) G_GNUC_CONST;

GdkPixbufSimpleAnim *gdk_pixbuf_simple_anim_new           (gint   width, 
                                                           gint   height,
                                                           gfloat rate);
void                 gdk_pixbuf_simple_anim_add_frame     (GdkPixbufSimpleAnim *animation,
                                                           GdkPixbuf           *pixbuf);
void                 gdk_pixbuf_simple_anim_set_loop      (GdkPixbufSimpleAnim *animation,
                                                           gboolean             loop);
gboolean             gdk_pixbuf_simple_anim_get_loop      (GdkPixbufSimpleAnim *animation);

G_END_DECLS


#endif  /* GDK_PIXBUF_SIMPLE_ANIM_H */
