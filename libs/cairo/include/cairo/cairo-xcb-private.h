/* Cairo - a vector graphics library with display and print output
 *
 * Copyright © 2005 Red Hat, Inc.
 * Copyright © 2009 Intel Corporation
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
 * Contributors(s):
 *	Chris Wilson <chris@chris-wilson.co.uk>
 */

#ifndef CAIRO_XCB_PRIVATE_H
#define CAIRO_XCB_PRIVATE_H

#include "cairo-xcb.h"

#include "cairoint.h"

#include "cairo-cache-private.h"
#include "cairo-compiler-private.h"
#include "cairo-device-private.h"
#include "cairo-error-private.h"
#include "cairo-freelist-private.h"
#include "cairo-list-private.h"
#include "cairo-mutex-private.h"
#include "cairo-pattern-private.h"
#include "cairo-reference-count-private.h"
#include "cairo-scaled-font-private.h"
#include "cairo-spans-private.h"
#include "cairo-surface-private.h"

#include <xcb/xcb.h>
#include <xcb/render.h>
#include <xcb/xcbext.h>
#include <pixman.h>

#define XLIB_COORD_MAX 32767

/* maximum number of cached GC's */
#define GC_CACHE_SIZE 4

#define CAIRO_XCB_RENDER_AT_LEAST(major, minor)	\
	((XCB_RENDER_MAJOR_VERSION > major) ||	\
	((XCB_RENDER_MAJOR_VERSION == major) && (XCB_RENDER_MINOR_VERSION >= minor)))

typedef struct _cairo_xcb_connection cairo_xcb_connection_t;
typedef struct _cairo_xcb_font cairo_xcb_font_t;
typedef struct _cairo_xcb_screen cairo_xcb_screen_t;
typedef struct _cairo_xcb_surface cairo_xcb_surface_t;
typedef struct _cairo_xcb_picture cairo_xcb_picture_t;
typedef struct _cairo_xcb_shm_mem_pool cairo_xcb_shm_mem_pool_t;
typedef struct _cairo_xcb_shm_info cairo_xcb_shm_info_t;

struct _cairo_xcb_shm_info {
    cairo_xcb_connection_t *connection;
    uint32_t shm;
    uint32_t offset;
    size_t size;
    void *mem;
    cairo_xcb_shm_mem_pool_t *pool;
    xcb_get_input_focus_cookie_t sync;
    cairo_list_t pending;
};

struct _cairo_xcb_surface {
    cairo_surface_t base;
    cairo_image_surface_t *fallback;
    cairo_boxes_t fallback_damage;

    cairo_xcb_connection_t *connection;
    cairo_xcb_screen_t *screen;

    xcb_drawable_t drawable;
    cairo_bool_t owns_pixmap;

    cairo_bool_t deferred_clear;
    cairo_color_t deferred_clear_color;

    int width;
    int height;
    int depth;

    xcb_render_picture_t picture;
    xcb_render_pictformat_t xrender_format;
    pixman_format_code_t pixman_format;
    uint32_t precision;

    cairo_list_t link;
};

struct _cairo_xcb_picture {
    cairo_surface_t base;

    cairo_xcb_screen_t *screen;
    xcb_render_picture_t picture;
    xcb_render_pictformat_t xrender_format;
    pixman_format_code_t pixman_format;

    int width, height;

    cairo_extend_t extend;
    cairo_filter_t filter;
    cairo_bool_t has_component_alpha;
    xcb_render_transform_t transform;

    int x0, y0;
    int x, y;

    cairo_list_t link;
};

#if CAIRO_HAS_XLIB_XCB_FUNCTIONS
typedef struct _cairo_xlib_xcb_surface {
    cairo_surface_t base;

    cairo_xcb_surface_t *xcb;

    /* original settings for query */
    void *display;
    void *screen;
    void *visual;
    void *format;
} cairo_xlib_xcb_surface_t;
#endif


enum {
    GLYPHSET_INDEX_ARGB32,
    GLYPHSET_INDEX_A8,
    GLYPHSET_INDEX_A1,
    NUM_GLYPHSETS
};

typedef struct _cairo_xcb_font_glyphset_free_glyphs {
    xcb_render_glyphset_t   glyphset;
    int			    glyph_count;
    xcb_render_glyph_t	    glyph_indices[128];
} cairo_xcb_font_glyphset_free_glyphs_t;

typedef struct _cairo_xcb_font_glyphset_info {
    xcb_render_glyphset_t   glyphset;
    cairo_format_t	    format;
    xcb_render_pictformat_t xrender_format;
    cairo_xcb_font_glyphset_free_glyphs_t *pending_free_glyphs;
} cairo_xcb_font_glyphset_info_t;

struct _cairo_xcb_font {
    cairo_scaled_font_private_t      base;
    cairo_scaled_font_t		    *scaled_font;
    cairo_xcb_connection_t	    *connection;
    cairo_xcb_font_glyphset_info_t  glyphset_info[NUM_GLYPHSETS];
    cairo_list_t link;
};

struct _cairo_xcb_screen {
    cairo_xcb_connection_t *connection;

    xcb_screen_t	    *xcb_screen;

    xcb_gcontext_t gc[GC_CACHE_SIZE];
    uint8_t gc_depths[GC_CACHE_SIZE];

    cairo_surface_t *stock_colors[CAIRO_STOCK_NUM_COLORS];
    struct {
	cairo_surface_t *picture;
	cairo_color_t color;
    } solid_cache[16];
    int solid_cache_size;

    cairo_cache_t linear_pattern_cache;
    cairo_cache_t radial_pattern_cache;
    cairo_freelist_t pattern_cache_entry_freelist;

    cairo_list_t link;
    cairo_list_t surfaces;
    cairo_list_t pictures;
};

struct _cairo_xcb_connection {
    cairo_device_t device;

    xcb_connection_t *xcb_connection;

    xcb_render_pictformat_t standard_formats[5];
    cairo_hash_table_t *xrender_formats;
    cairo_hash_table_t *visual_to_xrender_format;

    unsigned int maximum_request_length;
    unsigned int flags;
    unsigned int original_flags;

    int force_precision;

    const xcb_setup_t *root;
    const xcb_query_extension_reply_t *render;
    const xcb_query_extension_reply_t *shm;

    cairo_list_t free_xids;
    cairo_freepool_t xid_pool;

    cairo_mutex_t shm_mutex;
    cairo_list_t shm_pools;
    cairo_list_t shm_pending;
    cairo_freepool_t shm_info_freelist;

    cairo_mutex_t screens_mutex;
    cairo_list_t screens;

    cairo_list_t fonts;

    cairo_list_t link;
};

enum {
    CAIRO_XCB_HAS_RENDER			= 0x0001,
    CAIRO_XCB_RENDER_HAS_FILL_RECTANGLES	= 0x0002,
    CAIRO_XCB_RENDER_HAS_COMPOSITE		= 0x0004,
    CAIRO_XCB_RENDER_HAS_COMPOSITE_TRAPEZOIDS	= 0x0008,
    CAIRO_XCB_RENDER_HAS_COMPOSITE_GLYPHS	= 0x0010,
    CAIRO_XCB_RENDER_HAS_PICTURE_TRANSFORM	= 0x0020,
    CAIRO_XCB_RENDER_HAS_FILTERS		= 0x0040,
    CAIRO_XCB_RENDER_HAS_PDF_OPERATORS		= 0x0080,
    CAIRO_XCB_RENDER_HAS_EXTENDED_REPEAT	= 0x0100,
    CAIRO_XCB_RENDER_HAS_GRADIENTS		= 0x0200,

    CAIRO_XCB_HAS_SHM				= 0x80000000,

    CAIRO_XCB_RENDER_MASK = CAIRO_XCB_HAS_RENDER |
			    CAIRO_XCB_RENDER_HAS_FILL_RECTANGLES |
			    CAIRO_XCB_RENDER_HAS_COMPOSITE |
			    CAIRO_XCB_RENDER_HAS_COMPOSITE_TRAPEZOIDS |
			    CAIRO_XCB_RENDER_HAS_COMPOSITE_GLYPHS |
			    CAIRO_XCB_RENDER_HAS_PICTURE_TRANSFORM |
			    CAIRO_XCB_RENDER_HAS_FILTERS |
			    CAIRO_XCB_RENDER_HAS_PDF_OPERATORS |
			    CAIRO_XCB_RENDER_HAS_EXTENDED_REPEAT |
			    CAIRO_XCB_RENDER_HAS_GRADIENTS,
    CAIRO_XCB_SHM_MASK    = CAIRO_XCB_HAS_SHM
};

#define CAIRO_XCB_SHM_SMALL_IMAGE 8192

cairo_private extern const cairo_surface_backend_t _cairo_xcb_surface_backend;

cairo_private cairo_xcb_connection_t *
_cairo_xcb_connection_get (xcb_connection_t *connection);

static inline cairo_xcb_connection_t *
_cairo_xcb_connection_reference (cairo_xcb_connection_t *connection)
{
    return (cairo_xcb_connection_t *) cairo_device_reference (&connection->device);
}

cairo_private xcb_render_pictformat_t
_cairo_xcb_connection_get_xrender_format (cairo_xcb_connection_t *connection,
					  pixman_format_code_t pixman_format);

cairo_private xcb_render_pictformat_t
_cairo_xcb_connection_get_xrender_format_for_visual (cairo_xcb_connection_t *connection,
						     const xcb_visualid_t visual);

static inline cairo_status_t cairo_warn
_cairo_xcb_connection_acquire (cairo_xcb_connection_t *connection)
{
    return cairo_device_acquire (&connection->device);
}

cairo_private uint32_t
_cairo_xcb_connection_get_xid (cairo_xcb_connection_t *connection);

cairo_private void
_cairo_xcb_connection_put_xid (cairo_xcb_connection_t *connection,
			       uint32_t xid);

static inline void
_cairo_xcb_connection_release (cairo_xcb_connection_t *connection)
{
    cairo_device_release (&connection->device);
}

static inline void
_cairo_xcb_connection_destroy (cairo_xcb_connection_t *connection)
{
    cairo_device_destroy (&connection->device);
}

cairo_private cairo_int_status_t
_cairo_xcb_connection_allocate_shm_info (cairo_xcb_connection_t *display,
					 size_t size,
					 cairo_bool_t might_reuse,
					 cairo_xcb_shm_info_t **shm_info_out);

cairo_private void
_cairo_xcb_shm_info_destroy (cairo_xcb_shm_info_t *shm_info);

cairo_private void
_cairo_xcb_connection_shm_mem_pools_flush (cairo_xcb_connection_t *connection);

cairo_private void
_cairo_xcb_connection_shm_mem_pools_fini (cairo_xcb_connection_t *connection);

cairo_private void
_cairo_xcb_font_close (cairo_xcb_font_t *font);

cairo_private cairo_xcb_screen_t *
_cairo_xcb_screen_get (xcb_connection_t *connection,
		       xcb_screen_t *screen);

cairo_private void
_cairo_xcb_screen_finish (cairo_xcb_screen_t *screen);

cairo_private xcb_gcontext_t
_cairo_xcb_screen_get_gc (cairo_xcb_screen_t *screen,
			  xcb_drawable_t drawable,
			  int depth);

cairo_private void
_cairo_xcb_screen_put_gc (cairo_xcb_screen_t *screen, int depth, xcb_gcontext_t gc);

cairo_private cairo_status_t
_cairo_xcb_screen_store_linear_picture (cairo_xcb_screen_t *screen,
					const cairo_linear_pattern_t *linear,
					cairo_surface_t *picture);

cairo_private cairo_surface_t *
_cairo_xcb_screen_lookup_linear_picture (cairo_xcb_screen_t *screen,
					 const cairo_linear_pattern_t *linear);

cairo_private cairo_status_t
_cairo_xcb_screen_store_radial_picture (cairo_xcb_screen_t *screen,
					const cairo_radial_pattern_t *radial,
					cairo_surface_t *picture);

cairo_private cairo_surface_t *
_cairo_xcb_screen_lookup_radial_picture (cairo_xcb_screen_t *screen,
					 const cairo_radial_pattern_t *radial);

cairo_private cairo_surface_t *
_cairo_xcb_surface_create_similar_image (void *abstrct_other,
					 cairo_format_t format,
					 int width,
					 int height);

cairo_private cairo_surface_t *
_cairo_xcb_surface_create_similar (void			*abstract_other,
				   cairo_content_t	 content,
				   int			 width,
				   int			 height);

cairo_private cairo_surface_t *
_cairo_xcb_surface_create_internal (cairo_xcb_screen_t		*screen,
				    xcb_drawable_t		 drawable,
				    cairo_bool_t		 owns_pixmap,
				    pixman_format_code_t	 pixman_format,
				    xcb_render_pictformat_t	 xrender_format,
				    int				 width,
				    int				 height);

cairo_private_no_warn cairo_bool_t
_cairo_xcb_surface_get_extents (void *abstract_surface,
				cairo_rectangle_int_t *extents);

cairo_private cairo_int_status_t
_cairo_xcb_render_compositor_paint (const cairo_compositor_t     *compositor,
				    cairo_composite_rectangles_t *extents);

cairo_private cairo_int_status_t
_cairo_xcb_render_compositor_mask (const cairo_compositor_t     *compositor,
				   cairo_composite_rectangles_t *extents);

cairo_private cairo_int_status_t
_cairo_xcb_render_compositor_stroke (const cairo_compositor_t     *compositor,
				     cairo_composite_rectangles_t *extents,
				     const cairo_path_fixed_t     *path,
				     const cairo_stroke_style_t   *style,
				     const cairo_matrix_t         *ctm,
				     const cairo_matrix_t         *ctm_inverse,
				     double                        tolerance,
				     cairo_antialias_t             antialias);

cairo_private cairo_int_status_t
_cairo_xcb_render_compositor_fill (const cairo_compositor_t     *compositor,
				   cairo_composite_rectangles_t *extents,
				   const cairo_path_fixed_t     *path,
				   cairo_fill_rule_t             fill_rule,
				   double                        tolerance,
				   cairo_antialias_t             antialias);

cairo_private cairo_int_status_t
_cairo_xcb_render_compositor_glyphs (const cairo_compositor_t     *compositor,
				     cairo_composite_rectangles_t *extents,
				     cairo_scaled_font_t          *scaled_font,
				     cairo_glyph_t                *glyphs,
				     int                           num_glyphs,
				     cairo_bool_t                  overlap);
cairo_private void
_cairo_xcb_surface_scaled_font_fini (cairo_scaled_font_t *scaled_font);

cairo_private void
_cairo_xcb_surface_scaled_glyph_fini (cairo_scaled_glyph_t *scaled_glyph,
				      cairo_scaled_font_t  *scaled_font);

cairo_private cairo_status_t
_cairo_xcb_surface_clear (cairo_xcb_surface_t *dst);

cairo_private cairo_status_t
_cairo_xcb_surface_core_copy_boxes (cairo_xcb_surface_t		*dst,
				   const cairo_pattern_t	*src_pattern,
				   const cairo_rectangle_int_t	*extents,
				   const cairo_boxes_t		*boxes);

cairo_private cairo_status_t
_cairo_xcb_surface_core_fill_boxes (cairo_xcb_surface_t *dst,
				    const cairo_color_t	*color,
				    cairo_boxes_t *boxes);

cairo_private xcb_pixmap_t
_cairo_xcb_connection_create_pixmap (cairo_xcb_connection_t *connection,
				     uint8_t depth,
				     xcb_drawable_t drawable,
				     uint16_t width,
				     uint16_t height);

cairo_private void
_cairo_xcb_connection_free_pixmap (cairo_xcb_connection_t *connection,
				   xcb_pixmap_t pixmap);

cairo_private xcb_gcontext_t
_cairo_xcb_connection_create_gc (cairo_xcb_connection_t *connection,
				 xcb_drawable_t drawable,
				 uint32_t value_mask,
				 uint32_t *values);

cairo_private void
_cairo_xcb_connection_free_gc (cairo_xcb_connection_t *connection,
			       xcb_gcontext_t gc);

cairo_private void
_cairo_xcb_connection_change_gc (cairo_xcb_connection_t *connection,
				 xcb_gcontext_t gc,
				 uint32_t value_mask,
				 uint32_t *values);

cairo_private void
_cairo_xcb_connection_copy_area (cairo_xcb_connection_t *connection,
				 xcb_drawable_t src,
				 xcb_drawable_t dst,
				 xcb_gcontext_t gc,
				 int16_t src_x,
				 int16_t src_y,
				 int16_t dst_x,
				 int16_t dst_y,
				 uint16_t width,
				 uint16_t height);

cairo_private void
_cairo_xcb_connection_put_image (cairo_xcb_connection_t *connection,
				 xcb_drawable_t dst,
				 xcb_gcontext_t gc,
				 uint16_t width,
				 uint16_t height,
				 int16_t dst_x,
				 int16_t dst_y,
				 uint8_t depth,
				 uint32_t length,
				 void *data);

cairo_private void
_cairo_xcb_connection_put_subimage (cairo_xcb_connection_t *connection,
				    xcb_drawable_t dst,
				    xcb_gcontext_t gc,
				    int16_t src_x,
				    int16_t src_y,
				    uint16_t width,
				    uint16_t height,
				    uint16_t cpp,
				    int stride,
				    int16_t dst_x,
				    int16_t dst_y,
				    uint8_t depth,
				    void *data);

cairo_private cairo_status_t
_cairo_xcb_connection_get_image (cairo_xcb_connection_t *connection,
				 xcb_drawable_t src,
				 int16_t src_x,
				 int16_t src_y,
				 uint16_t width,
				 uint16_t height,
				 xcb_get_image_reply_t **reply);

cairo_private void
_cairo_xcb_connection_poly_fill_rectangle (cairo_xcb_connection_t *connection,
					   xcb_drawable_t dst,
					   xcb_gcontext_t gc,
					   uint32_t num_rectangles,
					   xcb_rectangle_t *rectangles);

cairo_private cairo_status_t
_cairo_xcb_shm_image_create (cairo_xcb_connection_t *connection,
			     pixman_format_code_t pixman_format,
			     int width, int height,
			     cairo_image_surface_t **image_out,
			     cairo_xcb_shm_info_t **shm_info_out);

#if CAIRO_HAS_XCB_SHM_FUNCTIONS
cairo_private uint32_t
_cairo_xcb_connection_shm_attach (cairo_xcb_connection_t *connection,
				  uint32_t id,
				  cairo_bool_t readonly);

cairo_private void
_cairo_xcb_connection_shm_put_image (cairo_xcb_connection_t *connection,
				     xcb_drawable_t dst,
				     xcb_gcontext_t gc,
				     uint16_t total_width,
				     uint16_t total_height,
				     int16_t src_x,
				     int16_t src_y,
				     uint16_t width,
				     uint16_t height,
				     int16_t dst_x,
				     int16_t dst_y,
				     uint8_t depth,
				     uint32_t shm,
				     uint32_t offset);

cairo_private cairo_status_t
_cairo_xcb_connection_shm_get_image (cairo_xcb_connection_t *connection,
				     xcb_drawable_t src,
				     int16_t src_x,
				     int16_t src_y,
				     uint16_t width,
				     uint16_t height,
				     uint32_t shmseg,
				     uint32_t offset);

cairo_private void
_cairo_xcb_connection_shm_detach (cairo_xcb_connection_t *connection,
				  uint32_t segment);
#else
static inline void
_cairo_xcb_connection_shm_put_image (cairo_xcb_connection_t *connection,
				     xcb_drawable_t dst,
				     xcb_gcontext_t gc,
				     uint16_t total_width,
				     uint16_t total_height,
				     int16_t src_x,
				     int16_t src_y,
				     uint16_t width,
				     uint16_t height,
				     int16_t dst_x,
				     int16_t dst_y,
				     uint8_t depth,
				     uint32_t shm,
				     uint32_t offset)
{
    ASSERT_NOT_REACHED;
}
#endif

cairo_private void
_cairo_xcb_connection_render_create_picture (cairo_xcb_connection_t  *connection,
					     xcb_render_picture_t     picture,
					     xcb_drawable_t           drawable,
					     xcb_render_pictformat_t  format,
					     uint32_t                 value_mask,
					     uint32_t	             *value_list);

cairo_private void
_cairo_xcb_connection_render_change_picture (cairo_xcb_connection_t     *connection,
					     xcb_render_picture_t  picture,
					     uint32_t              value_mask,
					     uint32_t             *value_list);

cairo_private void
_cairo_xcb_connection_render_set_picture_clip_rectangles (cairo_xcb_connection_t      *connection,
							  xcb_render_picture_t   picture,
							  int16_t                clip_x_origin,
							  int16_t                clip_y_origin,
							  uint32_t               rectangles_len,
							  xcb_rectangle_t       *rectangles);

cairo_private void
_cairo_xcb_connection_render_free_picture (cairo_xcb_connection_t *connection,
					   xcb_render_picture_t  picture);

cairo_private void
_cairo_xcb_connection_render_composite (cairo_xcb_connection_t     *connection,
					uint8_t               op,
					xcb_render_picture_t  src,
					xcb_render_picture_t  mask,
					xcb_render_picture_t  dst,
					int16_t               src_x,
					int16_t               src_y,
					int16_t               mask_x,
					int16_t               mask_y,
					int16_t               dst_x,
					int16_t               dst_y,
					uint16_t              width,
					uint16_t              height);

cairo_private void
_cairo_xcb_connection_render_trapezoids (cairo_xcb_connection_t *connection,
					 uint8_t                       op,
					 xcb_render_picture_t          src,
					 xcb_render_picture_t          dst,
					 xcb_render_pictformat_t       mask_format,
					 int16_t                       src_x,
					 int16_t                       src_y,
					 uint32_t                      traps_len,
					 xcb_render_trapezoid_t       *traps);

cairo_private void
_cairo_xcb_connection_render_create_glyph_set (cairo_xcb_connection_t	*connection,
					       xcb_render_glyphset_t	 id,
					       xcb_render_pictformat_t  format);

cairo_private void
_cairo_xcb_connection_render_free_glyph_set (cairo_xcb_connection_t      *connection,
					     xcb_render_glyphset_t  glyphset);

cairo_private void
_cairo_xcb_connection_render_add_glyphs (cairo_xcb_connection_t             *connection,
					 xcb_render_glyphset_t         glyphset,
					 uint32_t                      num_glyphs,
					 uint32_t                     *glyphs_id,
					 xcb_render_glyphinfo_t       *glyphs,
					 uint32_t                      data_len,
					 uint8_t                      *data);

cairo_private void
_cairo_xcb_connection_render_free_glyphs (cairo_xcb_connection_t         *connection,
					  xcb_render_glyphset_t     glyphset,
					  uint32_t                  num_glyphs,
					  xcb_render_glyph_t       *glyphs);

cairo_private void
_cairo_xcb_connection_render_composite_glyphs_8 (cairo_xcb_connection_t        *connection,
						 uint8_t                  op,
						 xcb_render_picture_t     src,
						 xcb_render_picture_t     dst,
						 xcb_render_pictformat_t  mask_format,
						 xcb_render_glyphset_t    glyphset,
						 int16_t                  src_x,
						 int16_t                  src_y,
						 uint32_t                 glyphcmds_len,
						 uint8_t                 *glyphcmds);

cairo_private void
_cairo_xcb_connection_render_composite_glyphs_16 (cairo_xcb_connection_t        *connection,
						  uint8_t                  op,
						  xcb_render_picture_t     src,
						  xcb_render_picture_t     dst,
						  xcb_render_pictformat_t  mask_format,
						  xcb_render_glyphset_t    glyphset,
						  int16_t                  src_x,
						  int16_t                  src_y,
						  uint32_t                 glyphcmds_len,
						  uint8_t                 *glyphcmds);

cairo_private void
_cairo_xcb_connection_render_composite_glyphs_32 (cairo_xcb_connection_t        *connection,
						  uint8_t                  op,
						  xcb_render_picture_t     src,
						  xcb_render_picture_t     dst,
						  xcb_render_pictformat_t  mask_format,
						  xcb_render_glyphset_t    glyphset,
						  int16_t                  src_x,
						  int16_t                  src_y,
						  uint32_t                 glyphcmds_len,
						  uint8_t                 *glyphcmds);

cairo_private void
_cairo_xcb_connection_render_fill_rectangles (cairo_xcb_connection_t      *connection,
					      uint8_t                op,
					      xcb_render_picture_t   dst,
					      xcb_render_color_t     color,
					      uint32_t               num_rects,
					      xcb_rectangle_t       *rects);

cairo_private void
_cairo_xcb_connection_render_set_picture_transform (cairo_xcb_connection_t       *connection,
						    xcb_render_picture_t    picture,
						    xcb_render_transform_t  *transform);

cairo_private void
_cairo_xcb_connection_render_set_picture_filter (cairo_xcb_connection_t         *connection,
						 xcb_render_picture_t      picture,
						 uint16_t                  filter_len,
						 char                     *filter);

cairo_private void
_cairo_xcb_connection_render_create_solid_fill (cairo_xcb_connection_t     *connection,
						xcb_render_picture_t  picture,
						xcb_render_color_t    color);

cairo_private void
_cairo_xcb_connection_render_create_linear_gradient (cairo_xcb_connection_t         *connection,
						     xcb_render_picture_t      picture,
						     xcb_render_pointfix_t     p1,
						     xcb_render_pointfix_t     p2,
						     uint32_t                  num_stops,
						     xcb_render_fixed_t *stops,
						     xcb_render_color_t *colors);

cairo_private void
_cairo_xcb_connection_render_create_radial_gradient (cairo_xcb_connection_t         *connection,
						     xcb_render_picture_t      picture,
						     xcb_render_pointfix_t     inner,
						     xcb_render_pointfix_t     outer,
						     xcb_render_fixed_t        inner_radius,
						     xcb_render_fixed_t        outer_radius,
						     uint32_t                  num_stops,
						     xcb_render_fixed_t *stops,
						     xcb_render_color_t *colors);

cairo_private void
_cairo_xcb_connection_render_create_conical_gradient (cairo_xcb_connection_t         *c,
						      xcb_render_picture_t      picture,
						      xcb_render_pointfix_t     center,
						      xcb_render_fixed_t        angle,
						      uint32_t                  num_stops,
						      xcb_render_fixed_t *stops,
						      xcb_render_color_t *colors);
#if CAIRO_HAS_XLIB_XCB_FUNCTIONS
slim_hidden_proto (cairo_xcb_surface_create);
slim_hidden_proto (cairo_xcb_surface_create_for_bitmap);
slim_hidden_proto (cairo_xcb_surface_create_with_xrender_format);
slim_hidden_proto (cairo_xcb_surface_set_size);
slim_hidden_proto (cairo_xcb_surface_set_drawable);
slim_hidden_proto (cairo_xcb_device_debug_get_precision);
slim_hidden_proto_no_warn (cairo_xcb_device_debug_set_precision);
slim_hidden_proto_no_warn (cairo_xcb_device_debug_cap_xrender_version);
#endif

#endif /* CAIRO_XCB_PRIVATE_H */
