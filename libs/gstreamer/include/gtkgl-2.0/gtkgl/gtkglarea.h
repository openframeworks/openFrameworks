/*
 * Copyright (C) 1997-1998 Janne Löf <jlof@mail.student.oulu.fi>
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
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#ifndef __GTK_GL_AREA_H__
#define __GTK_GL_AREA_H__

#include <gdk/gdk.h>
#include <gtkgl/gdkgl.h>
#include <gtk/gtkdrawingarea.h>


G_BEGIN_DECLS

#define GTK_TYPE_GL_AREA            (gtk_gl_area_get_type())
#define GTK_GL_AREA(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_GL_AREA, GtkGLArea))
#define GTK_GL_AREA_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST (klass, GTK_TYPE_GL_AREA, GtkGLAreaClass))
#define GTK_IS_GL_AREA(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_GL_AREA))
#define GTK_IS_GL_AREA_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_GL_AREA))
#define GTK_GL_AREA_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_GL_AREA, GtkGLArea))


typedef struct _GtkGLArea       GtkGLArea;
typedef struct _GtkGLAreaClass  GtkGLAreaClass;


struct _GtkGLArea
{
  GtkDrawingArea  darea;
  GdkGLContext *glcontext;
};

struct _GtkGLAreaClass
{
  GtkDrawingAreaClass parent_class;
};

GType      gtk_gl_area_get_type   (void);
GtkWidget* gtk_gl_area_new        (int       *attrList);
GtkWidget* gtk_gl_area_share_new  (int       *attrList,
                                   GtkGLArea *share);
GtkWidget* gtk_gl_area_new_vargs  (GtkGLArea *share,
				   ...);


gint       gtk_gl_area_make_current(GtkGLArea *glarea);

void       gtk_gl_area_endgl      (GtkGLArea *glarea); /* deprecated */

void       gtk_gl_area_swap_buffers(GtkGLArea *glarea);


#ifndef GTKGL_DISABLE_DEPRECATED

#  define gtk_gl_area_begingl(glarea) \
      gtk_gl_area_make_current(glarea)
#  define gtk_gl_area_endgl(glarea) \
      glFlush()
#  define gtk_gl_area_swapbuffers(glarea) \
      gtk_gl_area_swap_buffers(glarea)
#  define gtk_gl_area_size(glarea, width, height) \
      gtk_widget_set_size_request(GTK_WIDGET(glarea), (width), (height))

#endif

G_END_DECLS

#endif /* __GTK_GL_AREA_H__ */
