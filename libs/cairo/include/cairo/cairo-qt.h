/* -*- Mode: c; c-basic-offset: 4; indent-tabs-mode: t; tab-width: 8; -*- */
/* cairo - a vector graphics library with display and print output
 *
 * Copyright © 2008 Mozilla Corporation
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
 */

#ifndef CAIRO_QT_H
#define CAIRO_QT_H

#include "cairo.h"

#if CAIRO_HAS_QT_SURFACE

#include <QtGui/QImage>
#include <QtGui/QPainter>

CAIRO_BEGIN_DECLS

cairo_public cairo_surface_t *
cairo_qt_surface_create (QPainter *painter);

cairo_public cairo_surface_t *
cairo_qt_surface_create_with_qimage (cairo_format_t format,
				     int width,
				     int height);

cairo_public cairo_surface_t *
cairo_qt_surface_create_with_qpixmap (cairo_content_t content,
				      int width,
				      int height);

cairo_public QPainter *
cairo_qt_surface_get_qpainter (cairo_surface_t *surface);

/* XXX needs hooking to generic surface layer, my vote is for
cairo_public cairo_surface_t *
cairo_surface_map_image (cairo_surface_t *surface);
cairo_public void
cairo_surface_unmap_image (cairo_surface_t *surface, cairo_surface_t *image);
*/
cairo_public cairo_surface_t *
cairo_qt_surface_get_image (cairo_surface_t *surface);

cairo_public QImage *
cairo_qt_surface_get_qimage (cairo_surface_t *surface);

CAIRO_END_DECLS

#else /* CAIRO_HAS_QT_SURFACE */

# error Cairo was not compiled with support for the Qt backend

#endif /* CAIRO_HAS_QT_SURFACE */

#endif /* CAIRO_QT_H */
