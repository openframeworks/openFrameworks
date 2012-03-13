/* Pango
 * pango-attributes.h: Attributed text
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

#ifndef __PANGO_ATTRIBUTES_H__
#define __PANGO_ATTRIBUTES_H__

#include <pango/pango-font.h>
#include <glib-object.h>

G_BEGIN_DECLS

/* PangoColor */

typedef struct _PangoColor PangoColor;

struct _PangoColor
{
  guint16 red;
  guint16 green;
  guint16 blue;
};

#define PANGO_TYPE_COLOR pango_color_get_type ()
GType       pango_color_get_type (void) G_GNUC_CONST;

PangoColor *pango_color_copy     (const PangoColor *src);
void        pango_color_free     (PangoColor       *color);
gboolean    pango_color_parse    (PangoColor       *color,
				  const char       *spec);
gchar      *pango_color_to_string(const PangoColor *color);


/* Attributes */

typedef struct _PangoAttribute    PangoAttribute;
typedef struct _PangoAttrClass    PangoAttrClass;

typedef struct _PangoAttrString   PangoAttrString;
typedef struct _PangoAttrLanguage PangoAttrLanguage;
typedef struct _PangoAttrInt      PangoAttrInt;
typedef struct _PangoAttrSize     PangoAttrSize;
typedef struct _PangoAttrFloat    PangoAttrFloat;
typedef struct _PangoAttrColor    PangoAttrColor;
typedef struct _PangoAttrFontDesc PangoAttrFontDesc;
typedef struct _PangoAttrShape    PangoAttrShape;

#define PANGO_TYPE_ATTR_LIST pango_attr_list_get_type ()
typedef struct _PangoAttrList     PangoAttrList;
typedef struct _PangoAttrIterator PangoAttrIterator;

typedef enum
{
  PANGO_ATTR_INVALID,           /* 0 is an invalid attribute type */
  PANGO_ATTR_LANGUAGE,		/* PangoAttrLanguage */
  PANGO_ATTR_FAMILY,		/* PangoAttrString */
  PANGO_ATTR_STYLE,		/* PangoAttrInt */
  PANGO_ATTR_WEIGHT,		/* PangoAttrInt */
  PANGO_ATTR_VARIANT,		/* PangoAttrInt */
  PANGO_ATTR_STRETCH,		/* PangoAttrInt */
  PANGO_ATTR_SIZE,		/* PangoAttrSize */
  PANGO_ATTR_FONT_DESC,		/* PangoAttrFontDesc */
  PANGO_ATTR_FOREGROUND,	/* PangoAttrColor */
  PANGO_ATTR_BACKGROUND,	/* PangoAttrColor */
  PANGO_ATTR_UNDERLINE,		/* PangoAttrInt */
  PANGO_ATTR_STRIKETHROUGH,	/* PangoAttrInt */
  PANGO_ATTR_RISE,		/* PangoAttrInt */
  PANGO_ATTR_SHAPE,		/* PangoAttrShape */
  PANGO_ATTR_SCALE,             /* PangoAttrFloat */
  PANGO_ATTR_FALLBACK,          /* PangoAttrInt */
  PANGO_ATTR_LETTER_SPACING,    /* PangoAttrInt */
  PANGO_ATTR_UNDERLINE_COLOR,	/* PangoAttrColor */
  PANGO_ATTR_STRIKETHROUGH_COLOR,/* PangoAttrColor */
  PANGO_ATTR_ABSOLUTE_SIZE,	/* PangoAttrSize */
  PANGO_ATTR_GRAVITY,		/* PangoAttrInt */
  PANGO_ATTR_GRAVITY_HINT	/* PangoAttrInt */
} PangoAttrType;

typedef enum {
  PANGO_UNDERLINE_NONE,
  PANGO_UNDERLINE_SINGLE,
  PANGO_UNDERLINE_DOUBLE,
  PANGO_UNDERLINE_LOW,
  PANGO_UNDERLINE_ERROR
} PangoUnderline;

#define PANGO_ATTR_INDEX_FROM_TEXT_BEGINNING	0
#define PANGO_ATTR_INDEX_TO_TEXT_END		G_MAXUINT

struct _PangoAttribute
{
  const PangoAttrClass *klass;
  guint start_index;	/* in bytes */
  guint end_index;	/* in bytes. The character at this index is not included */
};

typedef gboolean (*PangoAttrFilterFunc) (PangoAttribute *attribute,
					 gpointer        data);

typedef gpointer (*PangoAttrDataCopyFunc) (gconstpointer data);

struct _PangoAttrClass
{
  /*< public >*/
  PangoAttrType type;
  PangoAttribute * (*copy) (const PangoAttribute *attr);
  void             (*destroy) (PangoAttribute *attr);
  gboolean         (*equal) (const PangoAttribute *attr1, const PangoAttribute *attr2);
};

struct _PangoAttrString
{
  PangoAttribute attr;
  char *value;
};

struct _PangoAttrLanguage
{
  PangoAttribute attr;
  PangoLanguage *value;
};

struct _PangoAttrInt
{
  PangoAttribute attr;
  int value;
};

struct _PangoAttrFloat
{
  PangoAttribute attr;
  double value;
};

struct _PangoAttrColor
{
  PangoAttribute attr;
  PangoColor color;
};

struct _PangoAttrSize
{
  PangoAttribute attr;
  int size;
  guint absolute : 1;
};

struct _PangoAttrShape
{
  PangoAttribute attr;
  PangoRectangle ink_rect;
  PangoRectangle logical_rect;

  gpointer              data;
  PangoAttrDataCopyFunc copy_func;
  GDestroyNotify        destroy_func;
};

struct _PangoAttrFontDesc
{
  PangoAttribute attr;
  PangoFontDescription *desc;
};

PangoAttrType         pango_attr_type_register (const gchar        *name);
G_CONST_RETURN char * pango_attr_type_get_name (PangoAttrType       type) G_GNUC_CONST;

void             pango_attribute_init        (PangoAttribute       *attr,
					      const PangoAttrClass *klass);
PangoAttribute * pango_attribute_copy        (const PangoAttribute *attr);
void             pango_attribute_destroy     (PangoAttribute       *attr);
gboolean         pango_attribute_equal       (const PangoAttribute *attr1,
					      const PangoAttribute *attr2) G_GNUC_PURE;

PangoAttribute *pango_attr_language_new      (PangoLanguage              *language);
PangoAttribute *pango_attr_family_new        (const char                 *family);
PangoAttribute *pango_attr_foreground_new    (guint16                     red,
					      guint16                     green,
					      guint16                     blue);
PangoAttribute *pango_attr_background_new    (guint16                     red,
					      guint16                     green,
					      guint16                     blue);
PangoAttribute *pango_attr_size_new          (int                         size);
PangoAttribute *pango_attr_size_new_absolute (int                         size);
PangoAttribute *pango_attr_style_new         (PangoStyle                  style);
PangoAttribute *pango_attr_weight_new        (PangoWeight                 weight);
PangoAttribute *pango_attr_variant_new       (PangoVariant                variant);
PangoAttribute *pango_attr_stretch_new       (PangoStretch                stretch);
PangoAttribute *pango_attr_font_desc_new     (const PangoFontDescription *desc);

PangoAttribute *pango_attr_underline_new           (PangoUnderline underline);
PangoAttribute *pango_attr_underline_color_new     (guint16        red,
						    guint16        green,
						    guint16        blue);
PangoAttribute *pango_attr_strikethrough_new       (gboolean       strikethrough);
PangoAttribute *pango_attr_strikethrough_color_new (guint16        red,
						    guint16        green,
						    guint16        blue);

PangoAttribute *pango_attr_rise_new          (int                         rise);
PangoAttribute *pango_attr_scale_new         (double                      scale_factor);
PangoAttribute *pango_attr_fallback_new      (gboolean                    enable_fallback);
PangoAttribute *pango_attr_letter_spacing_new (int                        letter_spacing);

PangoAttribute *pango_attr_shape_new           (const PangoRectangle       *ink_rect,
						const PangoRectangle       *logical_rect);
PangoAttribute *pango_attr_shape_new_with_data (const PangoRectangle       *ink_rect,
						const PangoRectangle       *logical_rect,
						gpointer                    data,
						PangoAttrDataCopyFunc       copy_func,
						GDestroyNotify              destroy_func);

PangoAttribute *pango_attr_gravity_new      (PangoGravity     gravity);
PangoAttribute *pango_attr_gravity_hint_new (PangoGravityHint hint);

GType              pango_attr_list_get_type      (void) G_GNUC_CONST;
PangoAttrList *    pango_attr_list_new           (void);
PangoAttrList *    pango_attr_list_ref           (PangoAttrList  *list);
void               pango_attr_list_unref         (PangoAttrList  *list);
PangoAttrList *    pango_attr_list_copy          (PangoAttrList  *list);
void               pango_attr_list_insert        (PangoAttrList  *list,
						  PangoAttribute *attr);
void               pango_attr_list_insert_before (PangoAttrList  *list,
						  PangoAttribute *attr);
void               pango_attr_list_change        (PangoAttrList  *list,
						  PangoAttribute *attr);
void               pango_attr_list_splice        (PangoAttrList  *list,
						  PangoAttrList  *other,
						  gint            pos,
						  gint            len);

PangoAttrList *pango_attr_list_filter (PangoAttrList       *list,
				       PangoAttrFilterFunc  func,
				       gpointer             data);

PangoAttrIterator *pango_attr_list_get_iterator  (PangoAttrList  *list);

void               pango_attr_iterator_range    (PangoAttrIterator     *iterator,
						 gint                  *start,
						 gint                  *end);
gboolean           pango_attr_iterator_next     (PangoAttrIterator     *iterator);
PangoAttrIterator *pango_attr_iterator_copy     (PangoAttrIterator     *iterator);
void               pango_attr_iterator_destroy  (PangoAttrIterator     *iterator);
PangoAttribute *   pango_attr_iterator_get      (PangoAttrIterator     *iterator,
						 PangoAttrType          type);
void               pango_attr_iterator_get_font (PangoAttrIterator     *iterator,
						 PangoFontDescription  *desc,
						 PangoLanguage        **language,
						 GSList               **extra_attrs);
GSList *          pango_attr_iterator_get_attrs (PangoAttrIterator     *iterator);


gboolean pango_parse_markup (const char                 *markup_text,
			     int                         length,
			     gunichar                    accel_marker,
			     PangoAttrList             **attr_list,
			     char                      **text,
			     gunichar                   *accel_char,
			     GError                    **error);

G_END_DECLS

#endif /* __PANGO_ATTRIBUTES_H__ */
