/* Pango
 * pango-font.h: Font handling
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

#ifndef __PANGO_FONTMAP_H__
#define __PANGO_FONTMAP_H__

#include <pango/pango-font.h>
#include <pango/pango-fontset.h>

G_BEGIN_DECLS

#define PANGO_TYPE_FONT_MAP              (pango_font_map_get_type ())
#define PANGO_FONT_MAP(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), PANGO_TYPE_FONT_MAP, PangoFontMap))
#define PANGO_IS_FONT_MAP(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), PANGO_TYPE_FONT_MAP))

typedef struct _PangoContext PangoContext;

GType         pango_font_map_get_type       (void) G_GNUC_CONST;
PangoContext * pango_font_map_create_context (PangoFontMap               *fontmap);
PangoFont *   pango_font_map_load_font     (PangoFontMap                 *fontmap,
					    PangoContext                 *context,
					    const PangoFontDescription   *desc);
PangoFontset *pango_font_map_load_fontset  (PangoFontMap                 *fontmap,
					    PangoContext                 *context,
					    const PangoFontDescription   *desc,
					    PangoLanguage                *language);
void          pango_font_map_list_families (PangoFontMap                 *fontmap,
					    PangoFontFamily            ***families,
					    int                          *n_families);

#ifdef PANGO_ENABLE_BACKEND

#define PANGO_FONT_MAP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), PANGO_TYPE_FONT_MAP, PangoFontMapClass))
#define PANGO_IS_FONT_MAP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), PANGO_TYPE_FONT_MAP))
#define PANGO_FONT_MAP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), PANGO_TYPE_FONT_MAP, PangoFontMapClass))

typedef struct _PangoFontMapClass PangoFontMapClass;

struct _PangoFontMap
{
  GObject parent_instance;
};

struct _PangoFontMapClass
{
  GObjectClass parent_class;

  /*< public >*/

  PangoFont *   (*load_font)     (PangoFontMap               *fontmap,
				  PangoContext               *context,
				  const PangoFontDescription *desc);
  void          (*list_families) (PangoFontMap               *fontmap,
				  PangoFontFamily          ***families,
				  int                        *n_families);
  PangoFontset *(*load_fontset)  (PangoFontMap               *fontmap,
				  PangoContext               *context,
				  const PangoFontDescription *desc,
				  PangoLanguage              *language);

  const char *shape_engine_type;

  /*< private >*/

  /* Padding for future expansion */
  void (*_pango_reserved1) (void);
  void (*_pango_reserved2) (void);
  void (*_pango_reserved3) (void);
  void (*_pango_reserved4) (void);
};

const char   *pango_font_map_get_shape_engine_type (PangoFontMap *fontmap);

#endif /* PANGO_ENABLE_BACKEND */

G_END_DECLS

#endif /* __PANGO_FONTMAP_H__ */
