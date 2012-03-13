/* Pango
 * pango-types.h:
 *
 * Copyright (C) 1999 Red Hat Software
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __PANGO_TYPES_H__
#define __PANGO_TYPES_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _PangoLogAttr PangoLogAttr;

typedef struct _PangoEngineLang PangoEngineLang;
typedef struct _PangoEngineShape PangoEngineShape;

typedef struct _PangoFont    PangoFont;
typedef struct _PangoFontMap PangoFontMap;

typedef struct _PangoRectangle PangoRectangle;



/* A index of a glyph into a font. Rendering system dependent */
typedef guint32 PangoGlyph;



#define PANGO_SCALE 1024
#define PANGO_PIXELS(d) (((int)(d) + 512) >> 10)
#define PANGO_PIXELS_FLOOR(d) (((int)(d)) >> 10)
#define PANGO_PIXELS_CEIL(d) (((int)(d) + 1023) >> 10)
/* The above expressions are just slightly wrong for floating point d;
 * For example we'd expect PANGO_PIXELS(-512.5) => -1 but instead we get 0.
 * That's unlikely to matter for practical use and the expression is much
 * more compact and faster than alternatives that work exactly for both
 * integers and floating point.
 *
 * PANGO_PIXELS also behaves differently for +512 and -512.
 */

#define PANGO_UNITS_ROUND(d)				\
  (((d) + (PANGO_SCALE >> 1)) & ~(PANGO_SCALE - 1))


int    pango_units_from_double (double d) G_GNUC_CONST;
double pango_units_to_double (int i) G_GNUC_CONST;



/* A rectangle. Used to store logical and physical extents of glyphs,
 * runs, strings, etc.
 */
struct _PangoRectangle
{
  int x;
  int y;
  int width;
  int height;
};

/* Macros to translate from extents rectangles to ascent/descent/lbearing/rbearing
 */
#define PANGO_ASCENT(rect) (-(rect).y)
#define PANGO_DESCENT(rect) ((rect).y + (rect).height)
#define PANGO_LBEARING(rect) ((rect).x)
#define PANGO_RBEARING(rect) ((rect).x + (rect).width)

void pango_extents_to_pixels (PangoRectangle *inclusive,
			      PangoRectangle *nearest);


#include <pango/pango-gravity.h>
#include <pango/pango-language.h>
#include <pango/pango-matrix.h>
#include <pango/pango-script.h>
#include <pango/pango-bidi-type.h>


G_END_DECLS

#endif /* __PANGO_TYPES_H__ */
