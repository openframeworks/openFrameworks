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

#ifndef __GST_GL_MIXER_H__
#define __GST_GL_MIXER_H__

#include <gst/gst.h>
#include <gst/video/video.h>
#include "gstglmixerpad.h"

G_BEGIN_DECLS

#define GST_TYPE_GL_MIXER (gst_gl_mixer_get_type())
#define GST_GL_MIXER(obj) \
        (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_GL_MIXER, GstGLMixer))
#define GST_GL_MIXER_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_GL_MIXER, GstGLMixerClass))
#define GST_IS_GL_MIXER(obj) \
        (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_GL_MIXER))
#define GST_IS_GL_MIXER_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_GL_MIXER))
#define GST_GL_MIXER_GET_CLASS(obj) \
        (G_TYPE_INSTANCE_GET_CLASS((obj),GST_TYPE_GL_MIXER,GstGLMixerClass))

typedef struct _GstGLMixer GstGLMixer;
typedef struct _GstGLMixerClass GstGLMixerClass;

typedef gboolean (*GstGLMixerSetCaps) (GstGLMixer* mixer,
  GstCaps* outcaps);
typedef void (*GstGLMixerReset) (GstGLMixer *mixer);
typedef gboolean (*GstGLMixerProcessFunc) (GstGLMixer *mix,
  GPtrArray *buffers, GstBuffer *outbuf);

struct _GstGLMixer
{
  GstElement element;

  /* pad */
  GstPad *srcpad;

  /* Lock to prevent the state to change while blending */
  GMutex *state_lock;
  /* Sink pads using Collect Pads from core's base library */
  GstCollectPads *collect;
  /* sinkpads, a GSList of GstGLMixerPads */
  GSList *sinkpads;

  GPtrArray *array_buffers;

  gint numpads;

  GstClockTime last_ts;

  /* the master pad */
  GstGLMixerPad *master;

  gint width;
  gint height;
  gboolean setcaps;
  gboolean sendseg;

  gint fps_n;
  gint fps_d;

  /* Next available sinkpad index */
  gint next_sinkpad;

  /* sink event handling */
  GstPadEventFunction collect_event;
  guint64	segment_position;
  gdouble	segment_rate;

  GstGLDisplay *display;
  GLuint fbo;
  GLuint depthbuffer;
};

struct _GstGLMixerClass
{
  GstElementClass parent_class;

  GstGLMixerSetCaps set_caps;
  GstGLMixerReset reset;
  GstGLMixerProcessFunc process_buffers;
};

GType gst_gl_mixer_get_type(void);

G_END_DECLS
#endif /* __GST_GL_MIXER_H__ */
