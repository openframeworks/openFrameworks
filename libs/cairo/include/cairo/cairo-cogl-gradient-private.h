/* cairo - a vector graphics library with display and print output
 *
 * Copyright © 2011 Intel Corporation.
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
 * http://www.mozilla.og/MPL/
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
 * OF ANY KIND, either express or implied. See the LGPL or the MPL for
 * the specific language governing rights and limitations.
 *
 * Contributor(s):
 *      Robert Bragg <robert@linux.intel.com>
 */

#ifndef CAIRO_COGL_GRADIENT_PRIVATE_H
#define CAIRO_COGL_GRADIENT_PRIVATE_H

#include "cairoint.h"
#include "cairo-pattern-private.h"

#include <cogl/cogl2-experimental.h>

#define CAIRO_COGL_LINEAR_GRADIENT_CACHE_SIZE (1024 * 1024)

typedef enum _cairo_cogl_gradient_compatibility {
    CAIRO_COGL_GRADIENT_CAN_EXTEND_PAD	    = 1<<0,
    CAIRO_COGL_GRADIENT_CAN_EXTEND_REPEAT   = 1<<1,
    CAIRO_COGL_GRADIENT_CAN_EXTEND_REFLECT  = 1<<2,
    CAIRO_COGL_GRADIENT_CAN_EXTEND_NONE	    = 1<<3
} cairo_cogl_gradient_compatibility_t;
#define CAIRO_COGL_GRADIENT_CAN_EXTEND_ALL (CAIRO_COGL_GRADIENT_CAN_EXTEND_PAD |\
					    CAIRO_COGL_GRADIENT_CAN_EXTEND_REPEAT|\
					    CAIRO_COGL_GRADIENT_CAN_EXTEND_REFLECT|\
					    CAIRO_COGL_GRADIENT_CAN_EXTEND_NONE)

typedef struct _cairo_cogl_linear_texture_entry {
    cairo_cogl_gradient_compatibility_t compatibility;
    CoglTexture	*texture;
    float translate_x;
    float scale_x;
} cairo_cogl_linear_texture_entry_t;

typedef struct _cairo_cogl_linear_gradient {
    cairo_cache_entry_t		 cache_entry;
    cairo_reference_count_t	 ref_count;
    GList			*textures;
    int				 n_stops;
    const cairo_gradient_stop_t	*stops;
    cairo_gradient_stop_t	 stops_embedded[1];
} cairo_cogl_linear_gradient_t;

cairo_int_status_t
_cairo_cogl_get_linear_gradient (cairo_cogl_device_t *context,
				 cairo_extend_t extend_mode,
				 int n_stops,
				 const cairo_gradient_stop_t *stops,
				 cairo_cogl_linear_gradient_t **gradient_out);

cairo_cogl_linear_texture_entry_t *
_cairo_cogl_linear_gradient_texture_for_extend (cairo_cogl_linear_gradient_t *gradient,
						cairo_extend_t extend_mode);

cairo_cogl_linear_gradient_t *
_cairo_cogl_linear_gradient_reference (cairo_cogl_linear_gradient_t *gradient);

void
_cairo_cogl_linear_gradient_destroy (cairo_cogl_linear_gradient_t *gradient);

cairo_bool_t
_cairo_cogl_linear_gradient_equal (const void *key_a, const void *key_b);

#endif /* CAIRO_COGL_GRADIENT_PRIVATE_H */
