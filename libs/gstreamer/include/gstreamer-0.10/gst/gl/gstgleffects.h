/*
 * GStreamer
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

#ifndef __GST_GL_EFFECTS_H__
#define __GST_GL_EFFECTS_H__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gstglfilter.h>
#include <gstgleffectssources.h>

G_BEGIN_DECLS


#define GST_TYPE_GL_EFFECTS            (gst_gl_effects_get_type())
#define GST_GL_EFFECTS(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_GL_EFFECTS,GstGLEffects))
#define GST_IS_GL_EFFECTS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_GL_EFFECTS))
#define GST_GL_EFFECTS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass) , GST_TYPE_GL_EFFECTS,GstGLEffectsClass))
#define GST_IS_GL_EFFECTS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass) , GST_TYPE_GL_EFFECTS))
#define GST_GL_EFFECTS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj) , GST_TYPE_GL_EFFECTS,GstGLEffectsClass))

typedef struct _GstGLEffects GstGLEffects;
typedef struct _GstGLEffectsClass GstGLEffectsClass;

typedef void (* GstGLEffectProcessFunc) (GstGLEffects *effects);

#define NEEDED_TEXTURES 5

enum {
  GST_GL_EFFECTS_CURVE_HEAT,
  GST_GL_EFFECTS_CURVE_SEPIA,
  GST_GL_EFFECTS_CURVE_XPRO,
  GST_GL_EFFECTS_CURVE_LUMA_XPRO,
  GST_GL_EFFECTS_CURVE_XRAY,
  GST_GL_EFFECTS_N_CURVES
};

struct _GstGLEffects
{
  GstGLFilter filter;

  GstGLEffectProcessFunc effect;
  gint current_effect;

  GLuint intexture;
  GLuint midtexture[NEEDED_TEXTURES];
  GLuint outtexture;

  GLuint curve[GST_GL_EFFECTS_N_CURVES];

  GHashTable *shaderstable;

  gboolean horizontal_swap; /* switch left to right */

#ifdef OPENGL_ES2
  GLint draw_attr_position_loc;
  GLint draw_attr_texture_loc;
#endif
};

struct _GstGLEffectsClass
{
  GstGLFilterClass filter_class;
};

enum
{
  PROP_0,
  PROP_EFFECT,
  PROP_HSWAP
};


GType gst_gl_effects_get_type (void);

void gst_gl_effects_draw_texture (GstGLEffects * effects, GLuint tex);

void gst_gl_effects_identity (GstGLEffects *effects);
void gst_gl_effects_mirror (GstGLEffects *effects);
void gst_gl_effects_squeeze (GstGLEffects *effects);
void gst_gl_effects_stretch (GstGLEffects *effects);
void gst_gl_effects_tunnel (GstGLEffects *effects);
void gst_gl_effects_fisheye (GstGLEffects *effects);
void gst_gl_effects_twirl (GstGLEffects *effects);
void gst_gl_effects_bulge (GstGLEffects *effects);
void gst_gl_effects_square (GstGLEffects *effects);
void gst_gl_effects_heat (GstGLEffects *effects);
void gst_gl_effects_sepia (GstGLEffects *effects);
void gst_gl_effects_xpro (GstGLEffects *effects);
void gst_gl_effects_xray (GstGLEffects *effects);
void gst_gl_effects_luma_xpro (GstGLEffects *effects);
void gst_gl_effects_sin (GstGLEffects *effects);
void gst_gl_effects_glow (GstGLEffects *effects);

G_END_DECLS

#endif /*__GST_GL_EFFECTS_H__ */
