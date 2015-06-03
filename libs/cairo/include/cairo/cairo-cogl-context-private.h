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

#ifndef CAIRO_COGL_CONTEXT_PRIVATE_H
#define CAIRO_COGL_CONTEXT_PRIVATE_H

#include "cairo-default-context-private.h"
#include "cairo-cogl-private.h"

typedef struct _cairo_cogl_context {
    cairo_default_context_t base;

    cairo_cogl_device_t *dev;
    int path_ctm_age;
    cairo_path_fixed_t user_path;

    cairo_bool_t path_is_rectangle;
    double x, y, width, height;
} cairo_cogl_context_t;

cairo_t *
_cairo_cogl_context_create (void *target);

#endif /* CAIRO_COGL_CONTEXT_PRIVATE_H */
