/* cairo - a vector graphics library with display and print output
 *
 * Copyright © 2003 University of Southern California
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
 * The Initial Developer of the Original Code is University of Southern
 * California.
 *
 * Contributor(s):
 *	Carl D. Worth <cworth@isi.edu>
 */

/*
 * Environment variables affecting the backend:
 *
 *  %CAIRO_DIRECTFB_NO_ACCEL (boolean)
 *      if found, disables acceleration at all
 *
 *  %CAIRO_DIRECTFB_ARGB_FONT (boolean)
 *      if found, enables using ARGB fonts instead of A8
 */

#ifndef CAIRO_DIRECTFB_H
#define CAIRO_DIRECTFB_H

#include "cairo.h"

#if  CAIRO_HAS_DIRECTFB_SURFACE

#include <directfb.h>

CAIRO_BEGIN_DECLS

cairo_public cairo_surface_t *
cairo_directfb_surface_create (IDirectFB *dfb, IDirectFBSurface *surface);

CAIRO_END_DECLS

#else  /*CAIRO_HAS_DIRECTFB_SURFACE*/
# error Cairo was not compiled with support for the directfb backend
#endif /*CAIRO_HAS_DIRECTFB_SURFACE*/

#endif /*CAIRO_DIRECTFB_H*/
