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
 *      Robert Bragg <robert@linux.intel.com>
 */

#ifndef CAIRO_VG_H
#define CAIRO_VG_H

#include "cairo.h"

#if CAIRO_HAS_COGL_SURFACE

#include <cogl/cogl2-experimental.h>

CAIRO_BEGIN_DECLS

cairo_public cairo_device_t *
cairo_cogl_device_create (CoglContext *context);

cairo_public cairo_surface_t *
cairo_cogl_surface_create (cairo_device_t *device,
			   CoglFramebuffer *framebuffer);

cairo_public CoglFramebuffer *
cairo_cogl_surface_get_framebuffer (cairo_surface_t *surface);

cairo_public CoglTexture *
cairo_cogl_surface_get_texture (cairo_surface_t *surface);

cairo_public void
cairo_cogl_surface_end_frame (cairo_surface_t *surface);

CAIRO_END_DECLS

#else  /* CAIRO_HAS_COGL_SURFACE*/
# error Cairo was not compiled with support for the Cogl backend
#endif /* CAIRO_HAS_COGL_SURFACE*/

#endif /* CAIRO_COGL_H */
