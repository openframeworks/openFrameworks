/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */
/* GdkPixbuf library - Animation support
 *
 * Copyright (C) 1999 The Free Software Foundation
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

#ifndef GDK_PIXBUF_ANIMATION_H
#define GDK_PIXBUF_ANIMATION_H

#include <glib-object.h>
#include <gdk-pixbuf/gdk-pixbuf-core.h>

G_BEGIN_DECLS

/* Animation support */

typedef struct _GdkPixbufAnimation GdkPixbufAnimation;
typedef struct _GdkPixbufAnimationIter GdkPixbufAnimationIter;

#define GDK_TYPE_PIXBUF_ANIMATION              (gdk_pixbuf_animation_get_type ())
#define GDK_PIXBUF_ANIMATION(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_PIXBUF_ANIMATION, GdkPixbufAnimation))
#define GDK_IS_PIXBUF_ANIMATION(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_PIXBUF_ANIMATION))

#define GDK_TYPE_PIXBUF_ANIMATION_ITER              (gdk_pixbuf_animation_iter_get_type ())
#define GDK_PIXBUF_ANIMATION_ITER(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_PIXBUF_ANIMATION_ITER, GdkPixbufAnimationIter))
#define GDK_IS_PIXBUF_ANIMATION_ITER(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_PIXBUF_ANIMATION_ITER))

GType               gdk_pixbuf_animation_get_type        (void) G_GNUC_CONST;

#ifdef G_OS_WIN32
#define gdk_pixbuf_animation_new_from_file gdk_pixbuf_animation_new_from_file_utf8
#endif

GdkPixbufAnimation *gdk_pixbuf_animation_new_from_file   (const char         *filename,
                                                          GError            **error);

#ifndef GDK_PIXBUF_DISABLE_DEPRECATED
GdkPixbufAnimation *gdk_pixbuf_animation_ref             (GdkPixbufAnimation *animation);
void                gdk_pixbuf_animation_unref           (GdkPixbufAnimation *animation);
#endif

int                 gdk_pixbuf_animation_get_width       (GdkPixbufAnimation *animation);
int                 gdk_pixbuf_animation_get_height      (GdkPixbufAnimation *animation);
gboolean            gdk_pixbuf_animation_is_static_image  (GdkPixbufAnimation *animation);
GdkPixbuf          *gdk_pixbuf_animation_get_static_image (GdkPixbufAnimation *animation);

GdkPixbufAnimationIter *gdk_pixbuf_animation_get_iter                        (GdkPixbufAnimation     *animation,
                                                                              const GTimeVal         *start_time);
GType                   gdk_pixbuf_animation_iter_get_type                   (void) G_GNUC_CONST;
int                     gdk_pixbuf_animation_iter_get_delay_time             (GdkPixbufAnimationIter *iter);
GdkPixbuf              *gdk_pixbuf_animation_iter_get_pixbuf                 (GdkPixbufAnimationIter *iter);
gboolean                gdk_pixbuf_animation_iter_on_currently_loading_frame (GdkPixbufAnimationIter *iter);
gboolean                gdk_pixbuf_animation_iter_advance                    (GdkPixbufAnimationIter *iter,
                                                                              const GTimeVal         *current_time);


#ifdef GDK_PIXBUF_ENABLE_BACKEND



typedef struct _GdkPixbufAnimationClass GdkPixbufAnimationClass;

#define GDK_PIXBUF_ANIMATION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_PIXBUF_ANIMATION, GdkPixbufAnimationClass))
#define GDK_IS_PIXBUF_ANIMATION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_PIXBUF_ANIMATION))
#define GDK_PIXBUF_ANIMATION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_PIXBUF_ANIMATION, GdkPixbufAnimationClass))

/* Private part of the GdkPixbufAnimation structure */
struct _GdkPixbufAnimation {
        GObject parent_instance;

};

struct _GdkPixbufAnimationClass {
        GObjectClass parent_class;

        /*< public >*/

        gboolean                (*is_static_image)  (GdkPixbufAnimation *anim);

        GdkPixbuf*              (*get_static_image) (GdkPixbufAnimation *anim);
        
        void                    (*get_size) (GdkPixbufAnimation *anim,
                                             int                 *width,
                                             int                 *height);
        
        GdkPixbufAnimationIter* (*get_iter) (GdkPixbufAnimation *anim,
                                             const GTimeVal     *start_time);

};



typedef struct _GdkPixbufAnimationIterClass GdkPixbufAnimationIterClass;

#define GDK_PIXBUF_ANIMATION_ITER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_PIXBUF_ANIMATION_ITER, GdkPixbufAnimationIterClass))
#define GDK_IS_PIXBUF_ANIMATION_ITER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_PIXBUF_ANIMATION_ITER))
#define GDK_PIXBUF_ANIMATION_ITER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_PIXBUF_ANIMATION_ITER, GdkPixbufAnimationIterClass))

struct _GdkPixbufAnimationIter {
        GObject parent_instance;

};

struct _GdkPixbufAnimationIterClass {
        GObjectClass parent_class;

        /*< public >*/

        int        (*get_delay_time)   (GdkPixbufAnimationIter *iter);

        GdkPixbuf* (*get_pixbuf)       (GdkPixbufAnimationIter *iter);

        gboolean   (*on_currently_loading_frame) (GdkPixbufAnimationIter *iter);

        gboolean   (*advance)          (GdkPixbufAnimationIter *iter,
                                        const GTimeVal         *current_time);
};
      

GType               gdk_pixbuf_non_anim_get_type (void) G_GNUC_CONST;
GdkPixbufAnimation* gdk_pixbuf_non_anim_new (GdkPixbuf *pixbuf);

#endif /* GDK_PIXBUF_ENABLE_BACKEND */

G_END_DECLS

#endif /* GDK_PIXBUF_ANIMATION_H */
