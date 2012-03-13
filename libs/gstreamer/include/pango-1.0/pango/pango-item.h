/* Pango
 * pango-item.h: Structure for storing run information
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

#ifndef __PANGO_ITEM_H__
#define __PANGO_ITEM_H__

#include <pango/pango-types.h>

G_BEGIN_DECLS

typedef struct _PangoAnalysis PangoAnalysis;
typedef struct _PangoItem PangoItem;

/* TODO: if more flags are needed, turn this into a real PangoAnalysisFlags enum */
#define PANGO_ANALYSIS_FLAG_CENTERED_BASELINE (1 << 0)

struct _PangoAnalysis
{
  PangoEngineShape *shape_engine;
  PangoEngineLang  *lang_engine;
  PangoFont *font;

  guint8 level;
  guint8 gravity; /* PangoGravity */
  guint8 flags;

  guint8 script; /* PangoScript */
  PangoLanguage *language;

  GSList *extra_attrs;
};

struct _PangoItem
{
  gint offset;
  gint length;
  gint num_chars;
  PangoAnalysis analysis;
};

#define PANGO_TYPE_ITEM (pango_item_get_type ())

GType pango_item_get_type (void) G_GNUC_CONST;

PangoItem *pango_item_new   (void);
PangoItem *pango_item_copy  (PangoItem  *item);
void       pango_item_free  (PangoItem  *item);
PangoItem *pango_item_split (PangoItem  *orig,
			     int         split_index,
			     int         split_offset);

G_END_DECLS

#endif /* __PANGO_ITEM_H__ */
