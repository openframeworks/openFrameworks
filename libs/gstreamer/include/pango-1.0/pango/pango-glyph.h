/* Pango
 * pango-glyph.h: Glyph storage
 *
 * Copyright (C) 2000 Red Hat Software
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

#ifndef __PANGO_GLYPH_H__
#define __PANGO_GLYPH_H__

#include <pango/pango-types.h>
#include <pango/pango-item.h>

G_BEGIN_DECLS

typedef struct _PangoGlyphGeometry PangoGlyphGeometry;
typedef struct _PangoGlyphVisAttr PangoGlyphVisAttr;
typedef struct _PangoGlyphInfo PangoGlyphInfo;
typedef struct _PangoGlyphString PangoGlyphString;

/* 1024ths of a device unit */
typedef gint32 PangoGlyphUnit;

/* Positioning information about a glyph
 */
struct _PangoGlyphGeometry
{
  PangoGlyphUnit width;
  PangoGlyphUnit x_offset;
  PangoGlyphUnit y_offset;
};

/* Visual attributes of a glyph
 */
struct _PangoGlyphVisAttr
{
  guint is_cluster_start : 1;
};

/* A single glyph
 */
struct _PangoGlyphInfo
{
  PangoGlyph    glyph;
  PangoGlyphGeometry geometry;
  PangoGlyphVisAttr  attr;
};

/* A string of glyphs with positional information and visual attributes -
 * ready for drawing
 */
struct _PangoGlyphString {
  gint num_glyphs;

  PangoGlyphInfo *glyphs;

  /* This is a memory inefficient way of representing the information
   * here - each value gives the byte index within the text
   * corresponding to the glyph string of the start of the cluster to
   * which the glyph belongs.
   */
  gint *log_clusters;

  /*< private >*/
  gint space;
};

#define PANGO_TYPE_GLYPH_STRING (pango_glyph_string_get_type ())

PangoGlyphString *pango_glyph_string_new      (void);
void              pango_glyph_string_set_size (PangoGlyphString *string,
					       gint              new_len);
GType             pango_glyph_string_get_type (void) G_GNUC_CONST;
PangoGlyphString *pango_glyph_string_copy     (PangoGlyphString *string);
void              pango_glyph_string_free     (PangoGlyphString *string);
void              pango_glyph_string_extents  (PangoGlyphString *glyphs,
					       PangoFont        *font,
					       PangoRectangle   *ink_rect,
					       PangoRectangle   *logical_rect);
int               pango_glyph_string_get_width(PangoGlyphString *glyphs);

void              pango_glyph_string_extents_range  (PangoGlyphString *glyphs,
						     int               start,
						     int               end,
						     PangoFont        *font,
						     PangoRectangle   *ink_rect,
						     PangoRectangle   *logical_rect);

void pango_glyph_string_get_logical_widths (PangoGlyphString *glyphs,
					    const char       *text,
					    int               length,
					    int               embedding_level,
					    int              *logical_widths);

void pango_glyph_string_index_to_x (PangoGlyphString *glyphs,
				    char             *text,
				    int               length,
				    PangoAnalysis    *analysis,
				    int               index_,
				    gboolean          trailing,
				    int              *x_pos);
void pango_glyph_string_x_to_index (PangoGlyphString *glyphs,
				    char             *text,
				    int               length,
				    PangoAnalysis    *analysis,
				    int               x_pos,
				    int              *index_,
				    int              *trailing);

/* Turn a string of characters into a string of glyphs
 */
void pango_shape (const gchar      *text,
		  gint              length,
		  const PangoAnalysis *analysis,
		  PangoGlyphString *glyphs);

GList *pango_reorder_items (GList *logical_items);

G_END_DECLS

#endif /* __PANGO_GLYPH_H__ */
