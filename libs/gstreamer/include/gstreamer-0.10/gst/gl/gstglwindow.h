/*
 * GStreamer
 * Copyright (C) 2008 Julien Isorce <julien.isorce@gmail.com>
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

#ifndef __GST_GL_WINDOW_H__
#define __GST_GL_WINDOW_H__

/* OpenGL 2.0 for Embedded Systems */
#ifdef OPENGL_ES2
#undef UNICODE
#include <EGL/egl.h>
#define UNICODE
#include <GLES2/gl2.h>
#include "gstgles2.h"
/* OpenGL for usual systems */
#else
#if __APPLE__
#include <GL/glew.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif
#endif

#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_GL_TYPE_WINDOW         (gst_gl_window_get_type())
#define GST_GL_WINDOW(o)           (G_TYPE_CHECK_INSTANCE_CAST((o), GST_GL_TYPE_WINDOW, GstGLWindow))
#define GST_GL_WINDOW_CLASS(k)     (G_TYPE_CHECK_CLASS((k), GST_GL_TYPE_WINDOW, GstGLWindowClass))
#define GST_GL_IS_WINDOW(o)        (G_TYPE_CHECK_INSTANCE_TYPE((o), GST_GL_TYPE_WINDOW))
#define GST_GL_IS_WINDOW_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE((k), GST_GL_TYPE_WINDOW))
#define GST_GL_WINDOW_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), GST_GL_TYPE_WINDOW, GstGLWindowClass))

#define GST_GL_WINDOW_ERROR (gst_gl_window_error_quark ())

typedef void (* GstGLWindowCB) ( gpointer );
typedef void (* GstGLWindowCB2) ( gpointer, gint, gint );

#define	GST_GL_WINDOW_CB(f)			 ((GstGLWindowCB) (f))
#define	GST_GL_WINDOW_CB2(f)		 ((GstGLWindowCB2) (f))

typedef struct _GstGLWindow        GstGLWindow;
typedef struct _GstGLWindowPrivate GstGLWindowPrivate;
typedef struct _GstGLWindowClass   GstGLWindowClass;

struct _GstGLWindow {
  /*< private >*/
  GObject parent;
  GstGLWindowPrivate *priv;
};

struct _GstGLWindowClass {
  /*< private >*/
  GObjectClass parent_class;
};

/* methods */

GQuark gst_gl_window_error_quark (void);
GType gst_gl_window_get_type (void);

GstGLWindow * gst_gl_window_new (gulong external_gl_context);

gulong gst_gl_window_get_internal_gl_context (GstGLWindow *window);
void gst_gl_window_activate_gl_context (GstGLWindow *window, gboolean activate);

void gst_gl_window_set_external_window_id (GstGLWindow *window, gulong id);
void gst_gl_window_set_draw_callback (GstGLWindow *window, GstGLWindowCB callback, gpointer data);
void gst_gl_window_set_resize_callback (GstGLWindow *window, GstGLWindowCB2 callback, gpointer data);
void gst_gl_window_set_close_callback (GstGLWindow *window, GstGLWindowCB callback, gpointer data);

void gst_gl_window_draw_unlocked (GstGLWindow *window, gint width, gint height);
void gst_gl_window_draw (GstGLWindow *window, gint width, gint height);
void gst_gl_window_run_loop (GstGLWindow *window);
void gst_gl_window_quit_loop (GstGLWindow *window, GstGLWindowCB callback, gpointer data);

void gst_gl_window_send_message (GstGLWindow *window, GstGLWindowCB callback, gpointer data);

/* helper */
void gst_gl_window_init_platform ();

G_END_DECLS

#endif /* __GST_GL_WINDOW_H__ */
