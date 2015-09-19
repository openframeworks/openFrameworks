/* vim: set sw=4 sts=4 et cin: */
/* cairo - a vector graphics library with display and print output
 *
 * Copyright (c) 2005-2006 netlabs.org
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
 * The Initial Developer of the Original Code is
 *     Doodle <doodle@scenergy.dfmk.hu>
 *
 * Contributor(s):
 *     Peter Weilbacher <mozilla@Weilbacher.org>
 */

#ifndef CAIRO_OS2_PRIVATE_H
#define CAIRO_OS2_PRIVATE_H

#include "cairo-os2.h"
#include "cairoint.h"

typedef struct _cairo_os2_surface
{
    cairo_surface_t        base;

    /* Mutex semaphore to protect private fields from concurrent access */
    HMTX                   hmtx_use_private_fields;
    /* Private fields: */
    HPS                    hps_client_window;
    HWND                   hwnd_client_window;
    BITMAPINFO2            bitmap_info;
    unsigned char         *pixels;
    cairo_image_surface_t *image_surface;
    int                    pixel_array_lend_count;
    HEV                    hev_pixel_array_came_back;

    RECTL                  rcl_dirty_area;
    cairo_bool_t           dirty_area_present;

    /* General flags: */
    cairo_bool_t           blit_as_changes;
    cairo_bool_t           use_24bpp;
} cairo_os2_surface_t;

#endif /* CAIRO_OS2_PRIVATE_H */
