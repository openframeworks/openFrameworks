/* Pango
 * pango-fontset.h: Font set handling
 *
 * Copyright (C) 2001 Red Hat Software
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

#ifndef __PANGO_FONTSET_H__
#define __PANGO_FONTSET_H__

#include <pango/pango-coverage.h>
#include <pango/pango-types.h>

#include <glib-object.h>

G_BEGIN_DECLS

/*
 * PangoFontset
 */

#define PANGO_TYPE_FONTSET              (pango_fontset_get_type ())
#define PANGO_FONTSET(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), PANGO_TYPE_FONTSET, PangoFontset))
#define PANGO_IS_FONTSET(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), PANGO_TYPE_FONTSET))

GType pango_fontset_get_type (void) G_GNUC_CONST;

typedef struct _PangoFontset        PangoFontset;

/**
 * PangoFontsetForeachFunc
 * @fontset: a #PangoFontset
 * @font: a font from @fontset
 * @data: callback data
 *
 * A callback function used by pango_fontset_foreach() when enumerating
 * the fonts in a fontset.
 *
 * Returns: if %TRUE, stop iteration and return immediately.
 *
 * Since: 1.4
 **/
typedef gboolean (*PangoFontsetForeachFunc) (PangoFontset  *fontset,
					     PangoFont     *font,
					     gpointer       data);

PangoFont *       pango_fontset_get_font    (PangoFontset           *fontset,
					     guint                   wc);
PangoFontMetrics *pango_fontset_get_metrics (PangoFontset           *fontset);
void              pango_fontset_foreach     (PangoFontset           *fontset,
					     PangoFontsetForeachFunc func,
					     gpointer                data);

#ifdef PANGO_ENABLE_BACKEND

typedef struct _PangoFontsetClass   PangoFontsetClass;

#define PANGO_FONTSET_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), PANGO_TYPE_FONTSET, PangoFontsetClass))
#define PANGO_IS_FONTSET_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), PANGO_TYPE_FONTSET))
#define PANGO_FONTSET_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), PANGO_TYPE_FONTSET, PangoFontsetClass))

struct _PangoFontset
{
  GObject parent_instance;
};

struct _PangoFontsetClass
{
  GObjectClass parent_class;

  /*< public >*/

  PangoFont *       (*get_font)     (PangoFontset     *fontset,
				     guint             wc);

  PangoFontMetrics *(*get_metrics)  (PangoFontset     *fontset);
  PangoLanguage *   (*get_language) (PangoFontset     *fontset);
  void              (*foreach)      (PangoFontset           *fontset,
				     PangoFontsetForeachFunc func,
				     gpointer                data);

  /*< private >*/

  /* Padding for future expansion */
  void (*_pango_reserved1) (void);
  void (*_pango_reserved2) (void);
  void (*_pango_reserved3) (void);
  void (*_pango_reserved4) (void);
};

/*
 * PangoFontsetSimple
 */

#define PANGO_TYPE_FONTSET_SIMPLE       (pango_fontset_simple_get_type ())
#define PANGO_FONTSET_SIMPLE(object)    (G_TYPE_CHECK_INSTANCE_CAST ((object), PANGO_TYPE_FONTSET_SIMPLE, PangoFontsetSimple))
#define PANGO_IS_FONTSET_SIMPLE(object) (G_TYPE_CHECK_INSTANCE_TYPE ((object), PANGO_TYPE_FONTSET_SIMPLE))

typedef struct _PangoFontsetSimple  PangoFontsetSimple;
typedef struct _PangoFontsetSimpleClass  PangoFontsetSimpleClass;

GType pango_fontset_simple_get_type (void) G_GNUC_CONST;

PangoFontsetSimple * pango_fontset_simple_new    (PangoLanguage      *language);
void                 pango_fontset_simple_append (PangoFontsetSimple *fontset,
						  PangoFont          *font);
int                  pango_fontset_simple_size   (PangoFontsetSimple *fontset);

#endif /* PANGO_ENABLE_BACKEND */

G_END_DECLS

#endif /* __PANGO_FONTSET_H__ */
