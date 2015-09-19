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
 *     Rich Walsh <dragtext@e-vertise.com>
 */

#ifndef _CAIRO_OS2_H_
#define _CAIRO_OS2_H_

#define INCL_DOS
#define INCL_DOSSEMAPHORES
#define INCL_DOSERRORS
#define INCL_WIN
#define INCL_GPI

#include "cairo.h"

#include <os2.h>

CAIRO_BEGIN_DECLS

/* The OS/2 Specific Cairo API */

cairo_public void
cairo_os2_init (void);

cairo_public void
cairo_os2_fini (void);

#if CAIRO_HAS_OS2_SURFACE

cairo_public cairo_surface_t *
cairo_os2_surface_create (HPS hps_client_window,
                          int width,
                          int height);

cairo_public cairo_surface_t *
cairo_os2_surface_create_for_window (HWND hwnd_client_window,
                                     int  width,
                                     int  height);

cairo_public void
cairo_os2_surface_set_hwnd (cairo_surface_t *surface,
                            HWND             hwnd_client_window);

cairo_public int
cairo_os2_surface_set_size (cairo_surface_t *surface,
                            int              new_width,
                            int              new_height,
                            int              timeout);

cairo_public void
cairo_os2_surface_refresh_window (cairo_surface_t *surface,
                                  HPS              hps_begin_paint,
                                  PRECTL           prcl_begin_paint_rect);

cairo_public void
cairo_os2_surface_set_manual_window_refresh (cairo_surface_t *surface,
                                             cairo_bool_t     manual_refresh);

cairo_public cairo_bool_t
cairo_os2_surface_get_manual_window_refresh (cairo_surface_t *surface);

cairo_public cairo_status_t
cairo_os2_surface_get_hps (cairo_surface_t *surface,
                           HPS             *hps);

cairo_public cairo_status_t
cairo_os2_surface_set_hps (cairo_surface_t *surface,
                           HPS              hps);

#else  /* CAIRO_HAS_OS2_SURFACE */
# error Cairo was not compiled with support for the OS/2 backend
#endif /* CAIRO_HAS_OS2_SURFACE */

CAIRO_END_DECLS

#endif /* _CAIRO_OS2_H_ */
