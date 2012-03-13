/* Pango
 * pango-glyph-item.h: Pair of PangoItem and a glyph string
 *
 * Copyright (C) 2002 Red Hat Software
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

#ifndef __PANGO_GLYPH_ITEM_H__
#define __PANGO_GLYPH_ITEM_H__

#include <pango/pango-attributes.h>
#include <pango/pango-break.h>
#include <pango/pango-item.h>
#include <pango/pango-glyph.h>

G_BEGIN_DECLS

typedef struct _PangoGlyphItem PangoGlyphItem;

struct _PangoGlyphItem
{
  PangoItem        *item;
  PangoGlyphString *glyphs;
};

#define PANGO_TYPE_GLYPH_ITEM (pango_glyph_item_get_type ())

GType pango_glyph_item_get_type (void) G_GNUC_CONST;

PangoGlyphItem *pango_glyph_item_split        (PangoGlyphItem *orig,
					       const char     *text,
					       int             split_index);
PangoGlyphItem *pango_glyph_item_copy         (PangoGlyphItem *orig);
void            pango_glyph_item_free         (PangoGlyphItem *glyph_item);
GSList *        pango_glyph_item_apply_attrs  (PangoGlyphItem *glyph_item,
					       const char     *text,
					       PangoAttrList  *list);
void            pango_glyph_item_letter_space (PangoGlyphItem *glyph_item,
					       const char     *text,
					       PangoLogAttr   *log_attrs,
					       int             letter_spacing);
void 	  pango_glyph_item_get_logical_widths (PangoGlyphItem *glyph_item,
					       const char     *text,
					       int            *logical_widths);


typedef struct _PangoGlyphItemIter PangoGlyphItemIter;

struct _PangoGlyphItemIter
{
  PangoGlyphItem *glyph_item;
  const gchar *text;

  int start_glyph;
  int start_index;
  int start_char;

  int end_glyph;
  int end_index;
  int end_char;
};

#define PANGO_TYPE_GLYPH_ITEM_ITER (pango_glyph_item_iter_get_type ())

GType               pango_glyph_item_iter_get_type (void) G_GNUC_CONST;
PangoGlyphItemIter *pango_glyph_item_iter_copy (PangoGlyphItemIter *orig);
void                pango_glyph_item_iter_free (PangoGlyphItemIter *iter);

gboolean pango_glyph_item_iter_init_start   (PangoGlyphItemIter *iter,
					     PangoGlyphItem     *glyph_item,
					     const char         *text);
gboolean pango_glyph_item_iter_init_end     (PangoGlyphItemIter *iter,
					     PangoGlyphItem     *glyph_item,
					     const char         *text);
gboolean pango_glyph_item_iter_next_cluster (PangoGlyphItemIter *iter);
gboolean pango_glyph_item_iter_prev_cluster (PangoGlyphItemIter *iter);

G_END_DECLS

#endif /* __PANGO_GLYPH_ITEM_H__ */
