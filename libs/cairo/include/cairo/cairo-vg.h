/* -*- Mode: c; tab-width: 8; c-basic-offset: 4; indent-tabs-mode: t; -*- */
/* cairo - a vector graphics library with display and print output
 *
 * Copyright © 2007 * Mozilla Corporation
 * Copyright © 2009 Chris Wilson
 *
 * This library is free software; you can redistribute it and/or
 * modify it either under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * (the "LGPL") or, at your option, under the terms of the Mozilla
 * Public License Version 1.1 (the "MPL"). If you do not alter this
 * notice, a recipient may use your version of this file under either
 * the MPL or the LGPL.
 *
 * You should have received a copy of the LGPL along with this library
 * in the file COPYING-LGPL-2.1; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA 02110-1335, USA
 * You should have received a copy of the MPL along with this library
 * in the file COPYING-MPL-1.1
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
 * OF ANY KIND, either express or implied. See the LGPL or the MPL for
 * the specific language governing rights and limitations.
 *
 * The Original Code is the cairo graphics library.
 *
 * The Initial Developer of the Original Code is Mozilla Corporation.
 *
 * Contributor(s):
 *      Vladimir Vukicevic <vladimir@mozilla.com>
 *      Chris Wilson <chris@chris-wilson.co.uk>
 */

#ifndef CAIRO_VG_H
#define CAIRO_VG_H

#include "cairo.h"

#if CAIRO_HAS_VG_SURFACE

#include <VG/openvg.h>

CAIRO_BEGIN_DECLS

typedef struct _cairo_vg_context cairo_vg_context_t;

#if CAIRO_HAS_GLX_FUNCTIONS
typedef struct __GLXcontextRec *GLXContext;
typedef struct _XDisplay Display;

cairo_public cairo_vg_context_t *
cairo_vg_context_create_for_glx (Display *dpy,
				 GLXContext ctx);
#endif

#if CAIRO_HAS_EGL_FUNCTIONS
#include <EGL/egl.h>

cairo_public cairo_vg_context_t *
cairo_vg_context_create_for_egl (EGLDisplay egl_display,
				 EGLContext egl_context);
#endif

cairo_public cairo_status_t
cairo_vg_context_status (cairo_vg_context_t *context);

cairo_public void
cairo_vg_context_destroy (cairo_vg_context_t *context);

cairo_public cairo_surface_t *
cairo_vg_surface_create (cairo_vg_context_t *context,
			 cairo_content_t content, int width, int height);

cairo_public cairo_surface_t *
cairo_vg_surface_create_for_image (cairo_vg_context_t *context,
				   VGImage image,
				   VGImageFormat format,
				   int width, int height);

cairo_public VGImage
cairo_vg_surface_get_image (cairo_surface_t *abstract_surface);

cairo_public VGImageFormat
cairo_vg_surface_get_format (cairo_surface_t *abstract_surface);

cairo_public int
cairo_vg_surface_get_height (cairo_surface_t *abstract_surface);

cairo_public int
cairo_vg_surface_get_width (cairo_surface_t *abstract_surface);

CAIRO_END_DECLS

#else  /* CAIRO_HAS_VG_SURFACE*/
# error Cairo was not compiled with support for the OpenVG backend
#endif /* CAIRO_HAS_VG_SURFACE*/

#endif /* CAIRO_VG_H */
