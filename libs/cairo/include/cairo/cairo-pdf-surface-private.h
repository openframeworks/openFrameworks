/* -*- Mode: c; tab-width: 8; c-basic-offset: 4; indent-tabs-mode: t; -*- */
/* cairo - a vector graphics library with display and print output
 *
 * Copyright © 2004 Red Hat, Inc
 * Copyright © 2006 Red Hat, Inc
 * Copyright © 2007, 2008 Adrian Johnson
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
 *	Kristian Høgsberg <krh@redhat.com>
 *	Carl Worth <cworth@cworth.org>
 *	Adrian Johnson <ajohnson@redneon.com>
 */

#ifndef CAIRO_PDF_SURFACE_PRIVATE_H
#define CAIRO_PDF_SURFACE_PRIVATE_H

#include "cairo-pdf.h"

#include "cairo-surface-private.h"
#include "cairo-surface-clipper-private.h"
#include "cairo-pdf-operators-private.h"
#include "cairo-path-fixed-private.h"

typedef struct _cairo_pdf_resource {
    unsigned int id;
} cairo_pdf_resource_t;

#define CAIRO_NUM_OPERATORS (CAIRO_OPERATOR_HSL_LUMINOSITY + 1)

typedef struct _cairo_pdf_group_resources {
    cairo_bool_t  operators[CAIRO_NUM_OPERATORS];
    cairo_array_t alphas;
    cairo_array_t smasks;
    cairo_array_t patterns;
    cairo_array_t shadings;
    cairo_array_t xobjects;
    cairo_array_t fonts;
} cairo_pdf_group_resources_t;

typedef struct _cairo_pdf_source_surface_entry {
    cairo_hash_entry_t base;
    unsigned int id;
    unsigned char *unique_id;
    unsigned long unique_id_length;
    cairo_bool_t interpolate;
    cairo_bool_t stencil_mask;
    cairo_pdf_resource_t surface_res;
    int width;
    int height;
    cairo_rectangle_int_t extents;
} cairo_pdf_source_surface_entry_t;

typedef struct _cairo_pdf_source_surface {
    cairo_pattern_type_t type;
    cairo_surface_t *surface;
    cairo_pattern_t *raster_pattern;
    cairo_pdf_source_surface_entry_t *hash_entry;
} cairo_pdf_source_surface_t;

typedef struct _cairo_pdf_pattern {
    double width;
    double height;
    cairo_rectangle_int_t extents;
    cairo_pattern_t *pattern;
    cairo_pdf_resource_t pattern_res;
    cairo_pdf_resource_t gstate_res;
    cairo_bool_t is_shading;
} cairo_pdf_pattern_t;

typedef enum _cairo_pdf_operation {
    PDF_PAINT,
    PDF_MASK,
    PDF_FILL,
    PDF_STROKE,
    PDF_SHOW_GLYPHS
} cairo_pdf_operation_t;

typedef struct _cairo_pdf_smask_group {
    double		  width;
    double		  height;
    cairo_rectangle_int_t extents;
    cairo_pdf_resource_t  group_res;
    cairo_pdf_operation_t operation;
    cairo_pattern_t	 *source;
    cairo_pdf_resource_t  source_res;
    cairo_pattern_t	 *mask;
    cairo_path_fixed_t	  path;
    cairo_fill_rule_t	  fill_rule;
    cairo_stroke_style_t  style;
    cairo_matrix_t	  ctm;
    cairo_matrix_t	  ctm_inverse;
    char		 *utf8;
    int                   utf8_len;
    cairo_glyph_t	 *glyphs;
    int			  num_glyphs;
    cairo_text_cluster_t *clusters;
    int                   num_clusters;
    cairo_bool_t          cluster_flags;
    cairo_scaled_font_t	 *scaled_font;
} cairo_pdf_smask_group_t;

typedef struct _cairo_pdf_surface cairo_pdf_surface_t;

struct _cairo_pdf_surface {
    cairo_surface_t base;

    /* Prefer the name "output" here to avoid confusion over the
     * structure within a PDF document known as a "stream". */
    cairo_output_stream_t *output;

    double width;
    double height;
    cairo_matrix_t cairo_to_pdf;

    cairo_array_t objects;
    cairo_array_t pages;
    cairo_array_t rgb_linear_functions;
    cairo_array_t alpha_linear_functions;
    cairo_array_t page_patterns;
    cairo_array_t page_surfaces;
    cairo_hash_table_t *all_surfaces;
    cairo_array_t smask_groups;
    cairo_array_t knockout_group;

    cairo_scaled_font_subsets_t *font_subsets;
    cairo_array_t fonts;

    cairo_pdf_resource_t next_available_resource;
    cairo_pdf_resource_t pages_resource;

    cairo_pdf_version_t pdf_version;
    cairo_bool_t compress_content;

    cairo_pdf_resource_t content;
    cairo_pdf_resource_t content_resources;
    cairo_pdf_group_resources_t resources;
    cairo_bool_t has_fallback_images;
    cairo_bool_t header_emitted;

    struct {
	cairo_bool_t active;
	cairo_pdf_resource_t self;
	cairo_pdf_resource_t length;
	long start_offset;
	cairo_bool_t compressed;
	cairo_output_stream_t *old_output;
    } pdf_stream;

    struct {
	cairo_bool_t active;
	cairo_output_stream_t *stream;
	cairo_output_stream_t *mem_stream;
	cairo_output_stream_t *old_output;
	cairo_pdf_resource_t   resource;
	cairo_box_double_t     bbox;
	cairo_bool_t is_knockout;
    } group_stream;

    cairo_surface_clipper_t clipper;

    cairo_pdf_operators_t pdf_operators;
    cairo_paginated_mode_t paginated_mode;
    cairo_bool_t select_pattern_gstate_saved;

    cairo_bool_t force_fallbacks;

    cairo_operator_t current_operator;
    cairo_bool_t current_pattern_is_solid_color;
    cairo_bool_t current_color_is_stroke;
    double current_color_red;
    double current_color_green;
    double current_color_blue;
    double current_color_alpha;

    cairo_surface_t *paginated_surface;
};

#endif /* CAIRO_PDF_SURFACE_PRIVATE_H */
