/* 
 * GStreamer
 * Copyright (C) 2007 David Schleef <ds@schleef.org>
 * Copyright (C) 2008 Julien Isorce <julien.isorce@gmail.com>
 * Copyright (C) 2008 Filippo Argiolas <filippo.argiolas@gmail.com>
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

#ifndef _GST_GL_FILTER_H_
#define _GST_GL_FILTER_H_

#include <gst/gst.h>
#include <gst/base/gstbasetransform.h>
#include <gst/video/video.h>

#include "gstglbuffer.h"

G_BEGIN_DECLS

#define GST_TYPE_GL_FILTER            (gst_gl_filter_get_type())
#define GST_GL_FILTER(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_GL_FILTER,GstGLFilter))
#define GST_IS_GL_FILTER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_GL_FILTER))
#define GST_GL_FILTER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass) ,GST_TYPE_GL_FILTER,GstGLFilterClass))
#define GST_IS_GL_FILTER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass) ,GST_TYPE_GL_FILTER))
#define GST_GL_FILTER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj) ,GST_TYPE_GL_FILTER,GstGLFilterClass))
typedef struct _GstGLFilter GstGLFilter;
typedef struct _GstGLFilterClass GstGLFilterClass;


typedef gboolean (*GstGLFilterSetCaps) (GstGLFilter* filter, 
					GstCaps* incaps, GstCaps* outcaps);
typedef gboolean (*GstGLFilterProcessFunc) (GstGLFilter *filter,
					    GstGLBuffer *inbuf, GstGLBuffer *outbuf);
typedef void (*GstGLFilterOnInitFBO) (GstGLFilter *filter);
typedef void (*GstGLFilterOnReset) (GstGLFilter *filter);
typedef void (*GstGLFilterOnStart) (GstGLFilter *filter);
typedef void (*GstGLFilterOnStop) (GstGLFilter *filter);

typedef void (*GstGLFilterGLStartFunc) (GstGLFilter *filter);
typedef void (*GstGLFilterGLStopFunc) (GstGLFilter *filter);

struct _GstGLFilter
{
  GstBaseTransform base_transform;

  GstGLDisplay *display;

  gint width;
  gint height;
  GLuint fbo;
  GLuint depthbuffer;

  GstGLShader *default_shader;

  gulong external_gl_context;
};

struct _GstGLFilterClass
{
  GstBaseTransformClass base_transform_class;
  GstGLFilterSetCaps set_caps;
  GstGLFilterProcessFunc filter;
  GstGLFilterOnInitFBO onInitFBO;
  
  GstGLFilterOnStart onStart;
  GstGLFilterOnStop onStop;
  GstGLFilterOnReset onReset;

  /* useful to init and cleanup custom gl resources */
  GstGLFilterGLStartFunc display_init_cb; /* run arbitrary gl code at start */
  GstGLFilterGLStopFunc display_reset_cb; /* run arbitrary gl code at stop */
};

GType gst_gl_filter_get_type(void);


void
gst_gl_filter_render_to_target (GstGLFilter *filter, 
				GLuint input, GLuint target,
				GLCB func, gpointer data);
void
gst_gl_filter_render_to_target_with_shader (GstGLFilter * filter,
                                            GLuint input, GLuint target, GstGLShader *shader);

void gst_gl_filter_draw_texture (GstGLFilter *filter, GLuint texture);

G_END_DECLS

#endif

