/* 
 * GStreame
 * Copyright (C) 2007 David Schleef <ds@schleef.org>
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

#ifndef _GST_GL_BUFFER_H_
#define _GST_GL_BUFFER_H_

#include <gst/gst.h>
#include <gst/video/video.h>

#include "gstgldisplay.h"

typedef struct _GstGLBuffer GstGLBuffer;

#define GST_TYPE_GL_BUFFER (gst_gl_buffer_get_type())

#define GST_IS_GL_BUFFER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_GL_BUFFER))
#define GST_GL_BUFFER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_GL_BUFFER, GstGLBuffer))

//A gl buffer has only one texture.
//Note that when a gl buffer is created by the upload filter,
//the colorspace conversion is made using a FBO. 
//And so the texture attached to this FBO is the one managed by the gl buffer.

struct _GstGLBuffer {
    GstBuffer buffer;

    GstGLDisplay *display;

    gint width;
    gint height;
    GLuint texture;
};

GType gst_gl_buffer_get_type (void);

#define gst_gl_buffer_ref(x) ((GstGLBuffer *)(gst_buffer_ref((GstBuffer *)(x))))
#define gst_gl_buffer_unref(x) (gst_buffer_unref((GstBuffer *)(x)))

GstGLBuffer* gst_gl_buffer_new (GstGLDisplay* display, gint gl_width, gint gl_height);
gint gst_gl_buffer_get_size (gint width, gint height);
gboolean gst_gl_buffer_parse_caps (GstCaps* caps, gint* width, gint* height);


#define GST_GL_VIDEO_CAPS \
  "video/x-raw-gl," \
  "width=(int)[1,8000]," \
  "height=(int)[1,6000]," \
  "pixel-aspect-ratio=(fraction)1/1," \
  "framerate=(fraction)[0/1,100/1]"

#endif

