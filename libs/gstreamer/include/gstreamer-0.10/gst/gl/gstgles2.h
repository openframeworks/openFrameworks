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

/* Compatibility for OpenGL ES 2.0 */

#ifndef __GST_GL_ES2__
#define __GST_GL_ES2__

/* GLEW */

#define GLEW_OK 0
#define GLEW_NO_ERROR 0
#define GLEW_VERSION 1
#define GLEW_VERSION_MAJOR 5
#define GLEW_VERSION_MINOR 1
#define GLEW_VERSION_MICRO 0

/* SUPPORTED */

//FIXME:
#define GL_RGBA8 GL_RGBA
#define GL_BGRA GL_RGBA
#define GL_UNSIGNED_INT_8_8_8_8 GL_UNSIGNED_BYTE
#define GL_UNSIGNED_INT_8_8_8_8_REV GL_UNSIGNED_BYTE
//END FIXME

#define GL_TEXTURE_RECTANGLE_ARB GL_TEXTURE_2D
#define GL_TEXTURE0_ARB GL_TEXTURE0
#define GL_TEXTURE1_ARB GL_TEXTURE1
#define GL_TEXTURE2_ARB GL_TEXTURE2

#define GLEW_EXT_framebuffer_object 1
#define GL_FRAMEBUFFER_EXT GL_FRAMEBUFFER
#define GL_RENDERBUFFER_EXT GL_RENDERBUFFER
#define GL_DEPTH_ATTACHMENT_EXT GL_DEPTH_ATTACHMENT
#define GL_FRAMEBUFFER_COMPLETE_EXT GL_FRAMEBUFFER_COMPLETE
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT GL_FRAMEBUFFER_UNSUPPORTED
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS

#define GL_COLOR_ATTACHMENT0_EXT GL_COLOR_ATTACHMENT0

#define glFramebufferTexture2DEXT glFramebufferTexture2D
#define glFramebufferRenderbufferEXT glFramebufferRenderbuffer
#define glGenFramebuffersEXT glGenFramebuffers
#define glBindFramebufferEXT glBindFramebuffer
#define glRenderbufferStorageEXT glRenderbufferStorage
#define glDeleteRenderbuffersEXT glDeleteRenderbuffers
#define glDeleteFramebuffersEXT glDeleteFramebuffers
#define glCheckFramebufferStatusEXT glCheckFramebufferStatus
#define glGenRenderbuffersEXT glGenRenderbuffers
#define glBindRenderbufferEXT glBindRenderbuffer
#define glFramebufferTexture2DEXT glFramebufferTexture2D

#define glActiveTextureARB glActiveTexture

#define GLEW_ARB_fragment_shader 1
#define GLhandleARB GLuint
#define GL_FRAGMENT_SHADER_ARB GL_FRAGMENT_SHADER
#define GL_VERTEX_SHADER_ARB GL_VERTEX_SHADER
#define GL_OBJECT_COMPILE_STATUS_ARB GL_COMPILE_STATUS

#define glUseProgramObjectARB glUseProgram
#define glCreateProgramObjectARB glCreateProgram
#define glCreateShaderObjectARB glCreateShader
#define glCompileShaderARB glCompileShader
#define glShaderSourceARB glShaderSource
#define glGetInfoLogARB glGetProgramInfoLog
#define glAttachObjectARB glAttachShader
#define glDetachObjectARB glDetachShader
#define glDeleteObjectARB glDeleteProgram
#define glLinkProgramARB glLinkProgram
#define glGetObjectParameterivARB glGetShaderiv
#define glUniform1fARB glUniform1f
#define glUniform1fvARB glUniform1fv
#define glUniform1iARB glUniform1i
#define glUniformMatrix4fvARB glUniformMatrix4fv
#define glGetUniformLocationARB glGetUniformLocation
#define glGetAttribLocationARB glGetAttribLocation
#define glBindAttribLocationARB glBindAttribLocation

/* UNSUPPORTED */

#define GLEW_ARB_imaging 0
#define GLEW_MESA_ycbcr_texture 0
#define GL_BGR 0
#define GL_YCBCR_MESA 0
#define GL_UNSIGNED_SHORT_8_8_MESA 0
#define GL_UNSIGNED_SHORT_8_8_MESA 0
#define GL_UNSIGNED_SHORT_8_8_REV_MESA 0

#define GL_COLOR_ATTACHMENT1_EXT 0
#define GL_COLOR_ATTACHMENT2_EXT 0
#define GL_TEXTURE_ENV 0
#define GL_TEXTURE_ENV_MODE 0

#endif /* __GST_GL_ES2__ */
