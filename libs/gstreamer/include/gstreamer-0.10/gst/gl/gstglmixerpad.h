/* 
 * GStreamer
 * Copyright (C) 2009 Julien Isorce <julien.isorce@gmail.com>
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

#ifndef __GST_GL_MIXER_PAD_H__
#define __GST_GL_MIXER_PAD_H__

#include <gst/gst.h>
#include <gst/base/gstcollectpads.h>

#include "gstglbuffer.h"

G_BEGIN_DECLS

#define GST_TYPE_GL_MIXER_PAD (gst_gl_mixer_pad_get_type())
#define GST_GL_MIXER_PAD(obj) \
        (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_GL_MIXER_PAD, GstGLMixerPad))
#define GST_GL_MIXER_PAD_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_GL_MIXER_PAD, GstGLMixerPadiClass))
#define GST_IS_GL_MIXER_PAD(obj) \
        (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_GL_MIXER_PAD))
#define GST_IS_GL_MIXER_PAD_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_GL_MIXER_PAD))

typedef struct _GstGLMixerPad GstGLMixerPad;
typedef struct _GstGLMixerPadClass GstGLMixerPadClass;
typedef struct _GstGLMixerCollect GstGLMixerCollect;

struct _GstGLMixerCollect
{
  GstCollectData collect;       /* we extend the CollectData */

  GstBuffer *buffer;            /* the queued buffer for this pad */

  GstGLMixerPad *mixpad;
};

/* all information needed for one video stream */
struct _GstGLMixerPad
{
  GstPad parent;                /* subclass the pad */

  gint64 queued;

  gint width;
  gint height;
  gint fps_n;
  gint fps_d;

  GstGLDisplay *display;

  GstGLMixerCollect *mixcol;
};

struct _GstGLMixerPadClass
{
  GstPadClass parent_class;
};

G_END_DECLS
#endif /* __GST_GL_MIXER_PAD_H__ */
