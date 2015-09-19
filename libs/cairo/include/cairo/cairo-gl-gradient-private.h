/* cairo - a vector graphics library with display and print output
 *
 * Copyright © 2009 Eric Anholt
 * Copyright © 2009 Chris Wilson
 * Copyright © 2005,2010 Red Hat, Inc
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
 * The Initial Developer of the Original Code is Red Hat, Inc.
 *
 * Contributor(s):
 *	Benjamin Otte <otte@gnome.org>
 *	Carl Worth <cworth@cworth.org>
 *	Chris Wilson <chris@chris-wilson.co.uk>
 *	Eric Anholt <eric@anholt.net>
 */

#ifndef CAIRO_GL_GRADIENT_PRIVATE_H
#define CAIRO_GL_GRADIENT_PRIVATE_H

#define GL_GLEXT_PROTOTYPES

#include "cairo-cache-private.h"
#include "cairo-device-private.h"
#include "cairo-reference-count-private.h"
#include "cairo-pattern-private.h"
#include "cairo-types-private.h"

#include "cairo-gl.h"

#if CAIRO_HAS_GL_SURFACE
#include <GL/gl.h>
#include <GL/glext.h>
#elif CAIRO_HAS_GLESV2_SURFACE
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#define CAIRO_GL_GRADIENT_CACHE_SIZE 4096

/* XXX: Declare in a better place */
typedef struct _cairo_gl_context cairo_gl_context_t;

typedef struct _cairo_gl_gradient {
    cairo_cache_entry_t           cache_entry;
    cairo_reference_count_t       ref_count;
    cairo_device_t               *device; /* NB: we don't hold a reference */
    GLuint                        tex;
    unsigned int	          n_stops;
    const cairo_gradient_stop_t  *stops;
    cairo_gradient_stop_t         stops_embedded[1];
} cairo_gl_gradient_t;

cairo_private cairo_int_status_t
_cairo_gl_gradient_create (cairo_gl_context_t           *ctx,
                           unsigned int                  n_stops,
                           const cairo_gradient_stop_t  *stops,
                           cairo_gl_gradient_t         **gradient_out);

cairo_private_no_warn cairo_gl_gradient_t *
_cairo_gl_gradient_reference (cairo_gl_gradient_t *gradient);

cairo_private void
_cairo_gl_gradient_destroy (cairo_gl_gradient_t *gradient);

cairo_private cairo_bool_t
_cairo_gl_gradient_equal (const void *key_a, const void *key_b);


#endif /* CAIRO_GL_GRADIENT_PRIVATE_H */
