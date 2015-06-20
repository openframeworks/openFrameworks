/* Cairo - a vector graphics library with display and print output
 *
 * Copyright © 2005 Red Hat, Inc.
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
 *	Karl Tomlinson <karlt+@karlt.net>, Mozilla Corporation
 */

#ifndef CAIRO_XLIB_PRIVATE_H
#define CAIRO_XLIB_PRIVATE_H

#include "cairo-xlib.h"
#include "cairo-xlib-xrender-private.h"

#include "cairo-compiler-private.h"
#include "cairo-device-private.h"
#include "cairo-freelist-type-private.h"
#include "cairo-list-private.h"
#include "cairo-reference-count-private.h"
#include "cairo-types-private.h"
#include "cairo-scaled-font-private.h"
#include "cairo-surface-private.h"

#include <pixman.h>
#include <string.h>

typedef struct _cairo_xlib_display cairo_xlib_display_t;
typedef struct _cairo_xlib_shm_display cairo_xlib_shm_display_t;
typedef struct _cairo_xlib_screen cairo_xlib_screen_t;
typedef struct _cairo_xlib_source cairo_xlib_source_t;
typedef struct _cairo_xlib_proxy cairo_xlib_proxy_t;
typedef struct _cairo_xlib_surface cairo_xlib_surface_t;

/* size of color cube */
#define CUBE_SIZE 6
/* size of gray ramp */
#define RAMP_SIZE 16
/* maximum number of cached GC's */
#define GC_CACHE_SIZE 4

struct _cairo_xlib_display {
    cairo_device_t base;

    cairo_xlib_display_t *next;

    Display *display;
    cairo_list_t screens;
    cairo_list_t fonts;

    cairo_xlib_shm_display_t *shm;

    const cairo_compositor_t *compositor;

    int render_major;
    int render_minor;
    XRenderPictFormat *cached_xrender_formats[CAIRO_FORMAT_RGB16_565 + 1];

    int force_precision;

    cairo_surface_t *white;
    cairo_surface_t *alpha[256];
    cairo_surface_t *solid[32];
    uint32_t solid_cache[32]; /* low 16 are opaque, high 16 transparent */
    struct {
	uint32_t color;
	int index;
    } last_solid_cache[2];

    /* TRUE if the server has a bug with repeating pictures
     *
     *  https://bugs.freedesktop.org/show_bug.cgi?id=3566
     *
     * We can't test for this because it depends on whether the
     * picture is in video memory or not.
     *
     * We also use this variable as a guard against a second
     * independent bug with transformed repeating pictures:
     *
     * http://lists.freedesktop.org/archives/cairo/2004-September/001839.html
     *
     * Both are fixed in xorg >= 6.9 and hopefully in > 6.8.2, so
     * we can reuse the test for now.
     */
    unsigned int buggy_gradients : 1;
    unsigned int buggy_pad_reflect : 1;
    unsigned int buggy_repeat : 1;
    unsigned int closed :1;
};

typedef struct _cairo_xlib_visual_info {
    cairo_list_t link;
    VisualID visualid;
    struct { uint8_t a, r, g, b; } colors[256];
    uint8_t cube_to_pseudocolor[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE];
    uint8_t field8_to_cube[256];
    int8_t  dither8_to_cube[256];
    uint8_t gray8_to_pseudocolor[256];
} cairo_xlib_visual_info_t;

struct _cairo_xlib_screen {
    cairo_list_t link;

    cairo_device_t *device;
    Screen *screen;

    cairo_list_t surfaces;

    cairo_bool_t has_font_options;
    cairo_font_options_t font_options;

    GC gc[GC_CACHE_SIZE];
    uint8_t gc_depths[GC_CACHE_SIZE];

    cairo_list_t visuals;
};

enum {
    GLYPHSET_INDEX_ARGB32,
    GLYPHSET_INDEX_A8,
    GLYPHSET_INDEX_A1,
    NUM_GLYPHSETS
};

typedef struct _cairo_xlib_font_glyphset {
    GlyphSet		glyphset;
    cairo_format_t	format;
    XRenderPictFormat	*xrender_format;
    struct _cairo_xlib_font_glyphset_free_glyphs {
	int		count;
	unsigned long	indices[128];
    } to_free;
} cairo_xlib_font_glyphset_t;

typedef struct _cairo_xlib_font {
    cairo_scaled_font_private_t		base;
    cairo_scaled_font_t			*font;
    cairo_device_t			*device;
    cairo_list_t			link;
    cairo_xlib_font_glyphset_t		glyphset[NUM_GLYPHSETS];
} cairo_xlib_font_t;

struct _cairo_xlib_surface {
    cairo_surface_t base;

    Picture picture;
    Drawable drawable;

    const cairo_compositor_t *compositor;
    cairo_surface_t *shm;
    int fallback;

    cairo_xlib_display_t *display;
    cairo_xlib_screen_t *screen;
    cairo_list_t link;

    Display *dpy; /* only valid between acquire/release */
    cairo_bool_t owns_pixmap;
    Visual *visual;

    int use_pixmap;

    int width;
    int height;
    int depth;

    int precision;
    XRenderPictFormat *xrender_format;
    /* XXX pixman_format instead of masks? */
    uint32_t a_mask;
    uint32_t r_mask;
    uint32_t g_mask;
    uint32_t b_mask;

    struct _cairo_xlib_source {
	cairo_surface_t base;

	Picture picture;
	Pixmap pixmap;
	Display *dpy;

	unsigned int filter:3;
	unsigned int extend:3;
	unsigned int has_matrix:1;
	unsigned int has_component_alpha:1;
    } embedded_source;
};

struct _cairo_xlib_proxy {
    struct _cairo_xlib_source source;
    cairo_surface_t *owner;
};

inline static cairo_bool_t
_cairo_xlib_vendor_is_xorg (Display *dpy)
{
    const char *const vendor = ServerVendor (dpy);
    return strstr (vendor, "X.Org") || strstr (vendor, "Xorg");
}

cairo_private cairo_status_t
_cairo_xlib_surface_get_gc (cairo_xlib_display_t *display,
                            cairo_xlib_surface_t *surface,
                            GC                   *gc);

cairo_private cairo_device_t *
_cairo_xlib_device_create (Display *display);

cairo_private void
_cairo_xlib_display_init_shm (cairo_xlib_display_t *display);

cairo_private void
_cairo_xlib_display_fini_shm (cairo_xlib_display_t *display);

cairo_private cairo_xlib_screen_t *
_cairo_xlib_display_get_screen (cairo_xlib_display_t *display,
				Screen *screen);

cairo_private cairo_status_t
_cairo_xlib_display_acquire (cairo_device_t *device,
                             cairo_xlib_display_t **display);

cairo_private cairo_bool_t
_cairo_xlib_display_has_repeat (cairo_device_t *device);

cairo_private cairo_bool_t
_cairo_xlib_display_has_reflect (cairo_device_t *device);

cairo_private cairo_bool_t
_cairo_xlib_display_has_gradients (cairo_device_t *device);

cairo_private void
_cairo_xlib_display_set_precision(cairo_device_t *device,
				  int precision);

cairo_private XRenderPictFormat *
_cairo_xlib_display_get_xrender_format (cairo_xlib_display_t	*display,
	                                cairo_format_t		 format);

cairo_private XRenderPictFormat *
_cairo_xlib_display_get_xrender_format_for_pixman (cairo_xlib_display_t *display,
						   pixman_format_code_t format);

cairo_private cairo_status_t
_cairo_xlib_screen_get (Display *dpy,
			Screen *screen,
			cairo_xlib_screen_t **out);

cairo_private void
_cairo_xlib_screen_destroy (cairo_xlib_display_t *display,
			    cairo_xlib_screen_t *info);

cairo_private GC
_cairo_xlib_screen_get_gc (cairo_xlib_display_t *display,
                           cairo_xlib_screen_t *info,
			   int depth,
			   Drawable drawable);
cairo_private void
_cairo_xlib_screen_put_gc (cairo_xlib_display_t *display,
			   cairo_xlib_screen_t *info,
			   int depth,
			   GC gc);

cairo_private cairo_font_options_t *
_cairo_xlib_screen_get_font_options (cairo_xlib_screen_t *info);

cairo_private cairo_status_t
_cairo_xlib_screen_get_visual_info (cairo_xlib_display_t *display,
                                    cairo_xlib_screen_t *info,
				    Visual *visual,
				    cairo_xlib_visual_info_t **out);

cairo_private cairo_status_t
_cairo_xlib_visual_info_create (Display *dpy,
	                        int screen,
				VisualID visualid,
				cairo_xlib_visual_info_t **out);

cairo_private void
_cairo_xlib_visual_info_destroy (cairo_xlib_visual_info_t *info);

cairo_private const cairo_compositor_t *
_cairo_xlib_core_compositor_get (void);

cairo_private const cairo_compositor_t *
_cairo_xlib_fallback_compositor_get (void);

cairo_private const cairo_compositor_t *
_cairo_xlib_mask_compositor_get (void);

cairo_private const cairo_compositor_t *
_cairo_xlib_traps_compositor_get (void);

cairo_private void
_cairo_xlib_surface_ensure_picture (cairo_xlib_surface_t    *surface);

cairo_private void
_cairo_xlib_surface_set_precision (cairo_xlib_surface_t	*surface,
				   cairo_antialias_t	 antialias);

cairo_private cairo_int_status_t
_cairo_xlib_surface_set_attributes (cairo_xlib_display_t       *display,
                                    cairo_xlib_surface_t       *surface,
				    cairo_surface_attributes_t *attributes,
				    double			xc,
				    double			yc);

cairo_private cairo_status_t
_cairo_xlib_surface_draw_image (cairo_xlib_surface_t   *surface,
				cairo_image_surface_t  *image,
				int                    src_x,
				int                    src_y,
				int                    width,
				int                    height,
				int                    dst_x,
				int                    dst_y);

cairo_private cairo_surface_t *
_cairo_xlib_source_create_for_pattern (cairo_surface_t *dst,
				       const cairo_pattern_t *pattern,
				       cairo_bool_t is_mask,
				       const cairo_rectangle_int_t *extents,
				       const cairo_rectangle_int_t *sample,
				       int *src_x, int *src_y);

cairo_private void
_cairo_xlib_font_close (cairo_xlib_font_t *font);

#define CAIRO_RENDER_AT_LEAST(surface, major, minor)	\
	(((surface)->render_major > major) ||			\
	 (((surface)->render_major == major) && ((surface)->render_minor >= minor)))

#define CAIRO_RENDER_HAS_CREATE_PICTURE(surface)		CAIRO_RENDER_AT_LEAST((surface), 0, 0)
#define CAIRO_RENDER_HAS_COMPOSITE(surface)		CAIRO_RENDER_AT_LEAST((surface), 0, 0)
#define CAIRO_RENDER_HAS_COMPOSITE_TEXT(surface)	CAIRO_RENDER_AT_LEAST((surface), 0, 0)

#define CAIRO_RENDER_HAS_FILL_RECTANGLES(surface)		CAIRO_RENDER_AT_LEAST((surface), 0, 1)

#define CAIRO_RENDER_HAS_DISJOINT(surface)			CAIRO_RENDER_AT_LEAST((surface), 0, 2)
#define CAIRO_RENDER_HAS_CONJOINT(surface)			CAIRO_RENDER_AT_LEAST((surface), 0, 2)

#define CAIRO_RENDER_HAS_TRAPEZOIDS(surface)		CAIRO_RENDER_AT_LEAST((surface), 0, 4)
#define CAIRO_RENDER_HAS_TRIANGLES(surface)		CAIRO_RENDER_AT_LEAST((surface), 0, 4)
#define CAIRO_RENDER_HAS_TRISTRIP(surface)			CAIRO_RENDER_AT_LEAST((surface), 0, 4)
#define CAIRO_RENDER_HAS_TRIFAN(surface)			CAIRO_RENDER_AT_LEAST((surface), 0, 4)

#define CAIRO_RENDER_HAS_PICTURE_TRANSFORM(surface)	CAIRO_RENDER_AT_LEAST((surface), 0, 6)
#define CAIRO_RENDER_HAS_FILTERS(surface)	CAIRO_RENDER_AT_LEAST((surface), 0, 6)

#define CAIRO_RENDER_HAS_EXTENDED_REPEAT(surface)	CAIRO_RENDER_AT_LEAST((surface), 0, 10)
#define CAIRO_RENDER_HAS_GRADIENTS(surface)	CAIRO_RENDER_AT_LEAST((surface), 0, 10)

#define CAIRO_RENDER_HAS_PDF_OPERATORS(surface)	CAIRO_RENDER_AT_LEAST((surface), 0, 11)

#define CAIRO_RENDER_SUPPORTS_OPERATOR(surface, op)	\
     ((op) <= CAIRO_OPERATOR_SATURATE ||			\
      (CAIRO_RENDER_HAS_PDF_OPERATORS(surface) &&	\
       (op) <= CAIRO_OPERATOR_HSL_LUMINOSITY))

/*
 * Return whether two xlib surfaces share the same
 * screen.  Both core and Render drawing require this
 * when using multiple drawables in an operation.
 */
static inline cairo_bool_t
_cairo_xlib_surface_same_screen (cairo_xlib_surface_t *dst,
				 cairo_xlib_surface_t *src)
{
    return dst->screen == src->screen;
}

cairo_private cairo_int_status_t
_cairo_xlib_core_fill_boxes (cairo_xlib_surface_t    *dst,
			     const cairo_color_t     *color,
			     cairo_boxes_t	    *boxes);

cairo_private cairo_int_status_t
_cairo_xlib_core_fill_rectangles (cairo_xlib_surface_t    *dst,
				  const cairo_color_t     *color,
				  int num_rects,
				  cairo_rectangle_int_t *rects);

static inline void
_cairo_xlib_surface_put_gc (cairo_xlib_display_t *display,
                            cairo_xlib_surface_t *surface,
                            GC                    gc)
{
    _cairo_xlib_screen_put_gc (display,
                               surface->screen,
			       surface->depth,
			       gc);
}

cairo_private cairo_surface_t *
_cairo_xlib_surface_create_similar_shm (void *surface,
					cairo_format_t format,
					int width, int height);

cairo_private cairo_surface_t *
_cairo_xlib_surface_get_shm (cairo_xlib_surface_t *surface,
			     cairo_bool_t overwrite);

cairo_private cairo_int_status_t
_cairo_xlib_surface_put_shm (cairo_xlib_surface_t *surface);

cairo_private cairo_surface_t *
_cairo_xlib_surface_create_shm (cairo_xlib_surface_t *other,
				pixman_format_code_t format,
				int width, int height);

cairo_private cairo_surface_t *
_cairo_xlib_surface_create_shm__image (cairo_xlib_surface_t *surface,
				       pixman_format_code_t format,
				       int width, int height);

cairo_private void
_cairo_xlib_shm_surface_get_ximage (cairo_surface_t *surface,
				    XImage *ximage);

cairo_private void *
_cairo_xlib_shm_surface_get_obdata (cairo_surface_t *surface);

cairo_private void
_cairo_xlib_shm_surface_mark_active (cairo_surface_t *shm);

cairo_private cairo_bool_t
_cairo_xlib_shm_surface_is_active (cairo_surface_t *surface);

cairo_private cairo_bool_t
_cairo_xlib_shm_surface_is_idle (cairo_surface_t *surface);

cairo_private Pixmap
_cairo_xlib_shm_surface_get_pixmap (cairo_surface_t *surface);

cairo_private XRenderPictFormat *
_cairo_xlib_shm_surface_get_xrender_format (cairo_surface_t *surface);

cairo_private pixman_format_code_t
_pixman_format_for_xlib_surface (cairo_xlib_surface_t *surface);

#endif /* CAIRO_XLIB_PRIVATE_H */
