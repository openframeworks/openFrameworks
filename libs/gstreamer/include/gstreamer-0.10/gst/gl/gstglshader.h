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

#ifndef __GST_GL_SHADER_H__
#define __GST_GL_SHADER_H__

/* OpenGL 2.0 for Embedded Systems */
#ifdef OPENGL_ES2
#include <GLES2/gl2.h>
#include "gstgles2.h"
/* OpenGL for usual systems */
#else
#include <GL/glew.h>
#endif
#include <gst/gst.h>

G_BEGIN_DECLS

#define GST_GL_TYPE_SHADER         (gst_gl_shader_get_type())
#define GST_GL_SHADER(o)           (G_TYPE_CHECK_INSTANCE_CAST((o), GST_GL_TYPE_SHADER, GstGLShader))
#define GST_GL_SHADER_CLASS(k)     (G_TYPE_CHECK_CLASS((k), GST_GL_TYPE_SHADER, GstGLShaderClass))
#define GST_GL_IS_SHADER(o)        (G_TYPE_CHECK_INSTANCE_TYPE((o), GST_GL_TYPE_SHADER))
#define GST_GL_IS_SHADER_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE((k), GST_GL_TYPE_SHADER))
#define GST_GL_SHADER_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), GST_GL_TYPE_SHADER, GstGLShaderClass))

#define GST_GL_SHADER_ERROR (gst_gl_shader_error_quark ())

typedef enum {
  GST_GL_SHADER_ERROR_COMPILE,
  GST_GL_SHADER_ERROR_LINK,
  GST_GL_SHADER_ERROR_PROGRAM
} GstGLShaderError;

typedef enum {
  GST_GL_SHADER_FRAGMENT_SOURCE,
  GST_GL_SHADER_VERTEX_SOURCE
} GstGLShaderSourceType;

typedef struct _GstGLShader        GstGLShader;
typedef struct _GstGLShaderPrivate GstGLShaderPrivate;
typedef struct _GstGLShaderClass   GstGLShaderClass;

struct _GstGLShader {
  /*< private >*/
  GObject parent;
  GstGLShaderPrivate *priv;
};

struct _GstGLShaderClass {
  /*< private >*/
  GObjectClass parent_class;
};

/* methods */

GQuark gst_gl_shader_error_quark (void);
GType gst_gl_shader_get_type (void);

GstGLShader * gst_gl_shader_new (void);

void gst_gl_shader_set_vertex_source   (GstGLShader *shader, 
					const gchar *src);
void gst_gl_shader_set_fragment_source (GstGLShader *shader, 
					const gchar *src);
G_CONST_RETURN gchar * gst_gl_shader_get_vertex_source (GstGLShader *shader);
G_CONST_RETURN gchar * gst_gl_shader_get_fragment_source (GstGLShader *shader);

void gst_gl_shader_set_active (GstGLShader *shader,
			       gboolean active);

gboolean gst_gl_shader_is_compiled (GstGLShader *shader);

gboolean gst_gl_shader_compile (GstGLShader *shader, GError **error);
gboolean gst_gl_shader_compile_and_check (GstGLShader *shader, const gchar *source, GstGLShaderSourceType type);
void gst_gl_shader_release (GstGLShader *shader);
void gst_gl_shader_use (GstGLShader *shader);

void gst_gl_shader_set_uniform_1i (GstGLShader *shader, const gchar *name, gint value);
void gst_gl_shader_set_uniform_1f (GstGLShader *shader, const gchar *name, gfloat value);
void gst_gl_shader_set_uniform_1fv (GstGLShader *shader, const gchar *name, guint count, gfloat * value);
void gst_gl_shader_set_uniform_matrix_4fv (GstGLShader * shader, const gchar * name,
  GLsizei count, GLboolean transpose, const GLfloat* value);

GLint gst_gl_shader_get_attribute_location (GstGLShader *shader, const gchar *name);
void gst_gl_shader_bind_attribute_location (GstGLShader * shader, GLuint index, const gchar * name);

G_END_DECLS

#endif /* __GST_GL_SHADER_H__ */
