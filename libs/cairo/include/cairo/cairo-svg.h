/* cairo - a vector graphics library with display and print output
 *
 * cairo-svg.h
 *
 * Copyright © 2005 Emmanuel Pacaud <emmanuel.pacaud@univ-poitiers.fr>
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
 */

#ifndef CAIRO_SVG_H
#define CAIRO_SVG_H

#include "cairo.h"

#if CAIRO_HAS_SVG_SURFACE

CAIRO_BEGIN_DECLS

/**
 * cairo_svg_version_t:
 * @CAIRO_SVG_VERSION_1_1: The version 1.1 of the SVG specification.
 * @CAIRO_SVG_VERSION_1_2: The version 1.2 of the SVG specification.
 *
 * #cairo_svg_version_t is used to describe the version number of the SVG
 * specification that a generated SVG file will conform to.
 */
typedef enum _cairo_svg_version {
    CAIRO_SVG_VERSION_1_1,
    CAIRO_SVG_VERSION_1_2
} cairo_svg_version_t;

cairo_public cairo_surface_t *
cairo_svg_surface_create (const char   *filename,
			  double	width_in_points,
			  double	height_in_points);

cairo_public cairo_surface_t *
cairo_svg_surface_create_for_stream (cairo_write_func_t	write_func,
				     void	       *closure,
				     double		width_in_points,
				     double		height_in_points);

cairo_public void
cairo_svg_surface_restrict_to_version (cairo_surface_t 		*surface,
				       cairo_svg_version_t  	 version);

cairo_public void
cairo_svg_get_versions (cairo_svg_version_t const	**versions,
                        int                      	 *num_versions);

cairo_public const char *
cairo_svg_version_to_string (cairo_svg_version_t version);

CAIRO_END_DECLS

#else  /* CAIRO_HAS_SVG_SURFACE */
# error Cairo was not compiled with support for the svg backend
#endif /* CAIRO_HAS_SVG_SURFACE */

#endif /* CAIRO_SVG_H */
