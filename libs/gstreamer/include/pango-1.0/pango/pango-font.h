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

#ifndef __PANGO_FONT_H__
#define __PANGO_FONT_H__

#include <pango/pango-coverage.h>
#include <pango/pango-types.h>

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _PangoFontDescription PangoFontDescription;
typedef struct _PangoFontMetrics PangoFontMetrics;

/**
 * PangoStyle:
 * @PANGO_STYLE_NORMAL: the font is upright.
 * @PANGO_STYLE_OBLIQUE: the font is slanted, but in a roman style.
 * @PANGO_STYLE_ITALIC: the font is slanted in an italic style.
 *
 * An enumeration specifying the various slant styles possible for a font.
 **/
typedef enum {
  PANGO_STYLE_NORMAL,
  PANGO_STYLE_OBLIQUE,
  PANGO_STYLE_ITALIC
} PangoStyle;

typedef enum {
  PANGO_VARIANT_NORMAL,
  PANGO_VARIANT_SMALL_CAPS
} PangoVariant;

typedef enum {
  PANGO_WEIGHT_THIN = 100,
  PANGO_WEIGHT_ULTRALIGHT = 200,
  PANGO_WEIGHT_LIGHT = 300,
  PANGO_WEIGHT_BOOK = 380,
  PANGO_WEIGHT_NORMAL = 400,
  PANGO_WEIGHT_MEDIUM = 500,
  PANGO_WEIGHT_SEMIBOLD = 600,
  PANGO_WEIGHT_BOLD = 700,
  PANGO_WEIGHT_ULTRABOLD = 800,
  PANGO_WEIGHT_HEAVY = 900,
  PANGO_WEIGHT_ULTRAHEAVY = 1000
} PangoWeight;

typedef enum {
  PANGO_STRETCH_ULTRA_CONDENSED,
  PANGO_STRETCH_EXTRA_CONDENSED,
  PANGO_STRETCH_CONDENSED,
  PANGO_STRETCH_SEMI_CONDENSED,
  PANGO_STRETCH_NORMAL,
  PANGO_STRETCH_SEMI_EXPANDED,
  PANGO_STRETCH_EXPANDED,
  PANGO_STRETCH_EXTRA_EXPANDED,
  PANGO_STRETCH_ULTRA_EXPANDED
} PangoStretch;

typedef enum {
  PANGO_FONT_MASK_FAMILY  = 1 << 0,
  PANGO_FONT_MASK_STYLE   = 1 << 1,
  PANGO_FONT_MASK_VARIANT = 1 << 2,
  PANGO_FONT_MASK_WEIGHT  = 1 << 3,
  PANGO_FONT_MASK_STRETCH = 1 << 4,
  PANGO_FONT_MASK_SIZE    = 1 << 5,
  PANGO_FONT_MASK_GRAVITY = 1 << 6
} PangoFontMask;

/* CSS scale factors (1.2 factor between each size) */
#define PANGO_SCALE_XX_SMALL ((double)0.5787037037037)
#define PANGO_SCALE_X_SMALL  ((double)0.6444444444444)
#define PANGO_SCALE_SMALL    ((double)0.8333333333333)
#define PANGO_SCALE_MEDIUM   ((double)1.0)
#define PANGO_SCALE_LARGE    ((double)1.2)
#define PANGO_SCALE_X_LARGE  ((double)1.4399999999999)
#define PANGO_SCALE_XX_LARGE ((double)1.728)

/*
 * PangoFontDescription
 */

#define PANGO_TYPE_FONT_DESCRIPTION (pango_font_description_get_type ())

GType                 pango_font_description_get_type    (void) G_GNUC_CONST;
PangoFontDescription *pango_font_description_new         (void);
PangoFontDescription *pango_font_description_copy        (const PangoFontDescription  *desc);
PangoFontDescription *pango_font_description_copy_static (const PangoFontDescription  *desc);
guint                 pango_font_description_hash        (const PangoFontDescription  *desc) G_GNUC_PURE;
gboolean              pango_font_description_equal       (const PangoFontDescription  *desc1,
							  const PangoFontDescription  *desc2) G_GNUC_PURE;
void                  pango_font_description_free        (PangoFontDescription        *desc);
void                  pango_font_descriptions_free       (PangoFontDescription       **descs,
							  int                          n_descs);

void                 pango_font_description_set_family        (PangoFontDescription *desc,
							       const char           *family);
void                 pango_font_description_set_family_static (PangoFontDescription *desc,
							       const char           *family);
G_CONST_RETURN char *pango_font_description_get_family        (const PangoFontDescription *desc) G_GNUC_PURE;
void                 pango_font_description_set_style         (PangoFontDescription *desc,
							       PangoStyle            style);
PangoStyle           pango_font_description_get_style         (const PangoFontDescription *desc) G_GNUC_PURE;
void                 pango_font_description_set_variant       (PangoFontDescription *desc,
							       PangoVariant          variant);
PangoVariant         pango_font_description_get_variant       (const PangoFontDescription *desc) G_GNUC_PURE;
void                 pango_font_description_set_weight        (PangoFontDescription *desc,
							       PangoWeight           weight);
PangoWeight          pango_font_description_get_weight        (const PangoFontDescription *desc) G_GNUC_PURE;
void                 pango_font_description_set_stretch       (PangoFontDescription *desc,
							       PangoStretch          stretch);
PangoStretch         pango_font_description_get_stretch       (const PangoFontDescription *desc) G_GNUC_PURE;
void                 pango_font_description_set_size          (PangoFontDescription *desc,
							       gint                  size);
gint                 pango_font_description_get_size          (const PangoFontDescription *desc) G_GNUC_PURE;
void                 pango_font_description_set_absolute_size (PangoFontDescription *desc,
							       double                size);
gboolean             pango_font_description_get_size_is_absolute (const PangoFontDescription *desc) G_GNUC_PURE;
void                 pango_font_description_set_gravity       (PangoFontDescription *desc,
							       PangoGravity          gravity);
PangoGravity         pango_font_description_get_gravity       (const PangoFontDescription *desc) G_GNUC_PURE;

PangoFontMask pango_font_description_get_set_fields (const PangoFontDescription *desc) G_GNUC_PURE;
void          pango_font_description_unset_fields   (PangoFontDescription       *desc,
						     PangoFontMask               to_unset);

void pango_font_description_merge        (PangoFontDescription       *desc,
					  const PangoFontDescription *desc_to_merge,
					  gboolean                    replace_existing);
void pango_font_description_merge_static (PangoFontDescription       *desc,
					  const PangoFontDescription *desc_to_merge,
					  gboolean                    replace_existing);

gboolean pango_font_description_better_match (const PangoFontDescription *desc,
					      const PangoFontDescription *old_match,
					      const PangoFontDescription *new_match) G_GNUC_PURE;

PangoFontDescription *pango_font_description_from_string (const char                  *str);
char *                pango_font_description_to_string   (const PangoFontDescription  *desc);
char *                pango_font_description_to_filename (const PangoFontDescription  *desc);

/*
 * PangoFontMetrics
 */

#define PANGO_TYPE_FONT_METRICS  (pango_font_metrics_get_type ())
GType             pango_font_metrics_get_type                    (void) G_GNUC_CONST;
PangoFontMetrics *pango_font_metrics_ref                         (PangoFontMetrics *metrics);
void              pango_font_metrics_unref                       (PangoFontMetrics *metrics);
int               pango_font_metrics_get_ascent                  (PangoFontMetrics *metrics) G_GNUC_PURE;
int               pango_font_metrics_get_descent                 (PangoFontMetrics *metrics) G_GNUC_PURE;
int               pango_font_metrics_get_approximate_char_width  (PangoFontMetrics *metrics) G_GNUC_PURE;
int               pango_font_metrics_get_approximate_digit_width (PangoFontMetrics *metrics) G_GNUC_PURE;
int               pango_font_metrics_get_underline_position      (PangoFontMetrics *metrics) G_GNUC_PURE;
int               pango_font_metrics_get_underline_thickness     (PangoFontMetrics *metrics) G_GNUC_PURE;
int               pango_font_metrics_get_strikethrough_position  (PangoFontMetrics *metrics) G_GNUC_PURE;
int               pango_font_metrics_get_strikethrough_thickness (PangoFontMetrics *metrics) G_GNUC_PURE;

#ifdef PANGO_ENABLE_BACKEND

PangoFontMetrics *pango_font_metrics_new (void);

struct _PangoFontMetrics
{
  guint ref_count;

  int ascent;
  int descent;
  int approximate_char_width;
  int approximate_digit_width;
  int underline_position;
  int underline_thickness;
  int strikethrough_position;
  int strikethrough_thickness;
};

#endif /* PANGO_ENABLE_BACKEND */

/*
 * PangoFontFamily
 */

#define PANGO_TYPE_FONT_FAMILY              (pango_font_family_get_type ())
#define PANGO_FONT_FAMILY(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), PANGO_TYPE_FONT_FAMILY, PangoFontFamily))
#define PANGO_IS_FONT_FAMILY(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), PANGO_TYPE_FONT_FAMILY))

typedef struct _PangoFontFamily      PangoFontFamily;
typedef struct _PangoFontFace        PangoFontFace;

GType      pango_font_family_get_type       (void) G_GNUC_CONST;

void                 pango_font_family_list_faces (PangoFontFamily  *family,
						   PangoFontFace  ***faces,
						   int              *n_faces);
G_CONST_RETURN char *pango_font_family_get_name   (PangoFontFamily  *family) G_GNUC_PURE;
gboolean   pango_font_family_is_monospace         (PangoFontFamily  *family) G_GNUC_PURE;

#ifdef PANGO_ENABLE_BACKEND

#define PANGO_FONT_FAMILY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), PANGO_TYPE_FONT_FAMILY, PangoFontFamilyClass))
#define PANGO_IS_FONT_FAMILY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), PANGO_TYPE_FONT_FAMILY))
#define PANGO_FONT_FAMILY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), PANGO_TYPE_FONT_FAMILY, PangoFontFamilyClass))

typedef struct _PangoFontFamilyClass PangoFontFamilyClass;

struct _PangoFontFamily
{
  GObject parent_instance;
};

struct _PangoFontFamilyClass
{
  GObjectClass parent_class;

  /*< public >*/

  void  (*list_faces)      (PangoFontFamily  *family,
			    PangoFontFace  ***faces,
			    int              *n_faces);
  const char * (*get_name) (PangoFontFamily  *family);
  gboolean (*is_monospace) (PangoFontFamily *family);

  /*< private >*/

  /* Padding for future expansion */
  void (*_pango_reserved2) (void);
  void (*_pango_reserved3) (void);
  void (*_pango_reserved4) (void);
};

#endif /* PANGO_ENABLE_BACKEND */

/*
 * PangoFontFace
 */

#define PANGO_TYPE_FONT_FACE              (pango_font_face_get_type ())
#define PANGO_FONT_FACE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), PANGO_TYPE_FONT_FACE, PangoFontFace))
#define PANGO_IS_FONT_FACE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), PANGO_TYPE_FONT_FACE))

GType      pango_font_face_get_type       (void) G_GNUC_CONST;

PangoFontDescription *pango_font_face_describe       (PangoFontFace  *face);
G_CONST_RETURN char  *pango_font_face_get_face_name  (PangoFontFace  *face) G_GNUC_PURE;
void                  pango_font_face_list_sizes     (PangoFontFace  *face,
						      int           **sizes,
						      int            *n_sizes);
gboolean              pango_font_face_is_synthesized (PangoFontFace  *face) G_GNUC_PURE;

#ifdef PANGO_ENABLE_BACKEND

#define PANGO_FONT_FACE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), PANGO_TYPE_FONT_FACE, PangoFontFaceClass))
#define PANGO_IS_FONT_FACE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), PANGO_TYPE_FONT_FACE))
#define PANGO_FONT_FACE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), PANGO_TYPE_FONT_FACE, PangoFontFaceClass))

typedef struct _PangoFontFaceClass   PangoFontFaceClass;

struct _PangoFontFace
{
  GObject parent_instance;
};

struct _PangoFontFaceClass
{
  GObjectClass parent_class;

  /*< public >*/

  const char           * (*get_face_name)  (PangoFontFace *face);
  PangoFontDescription * (*describe)       (PangoFontFace *face);
  void                   (*list_sizes)     (PangoFontFace  *face,
					    int           **sizes,
					    int            *n_sizes);
  gboolean               (*is_synthesized) (PangoFontFace *face);

  /*< private >*/

  /* Padding for future expansion */
  void (*_pango_reserved3) (void);
  void (*_pango_reserved4) (void);
};

#endif /* PANGO_ENABLE_BACKEND */

/*
 * PangoFont
 */

#define PANGO_TYPE_FONT              (pango_font_get_type ())
#define PANGO_FONT(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), PANGO_TYPE_FONT, PangoFont))
#define PANGO_IS_FONT(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), PANGO_TYPE_FONT))

GType                 pango_font_get_type          (void) G_GNUC_CONST;

PangoFontDescription *pango_font_describe          (PangoFont        *font);
PangoFontDescription *pango_font_describe_with_absolute_size (PangoFont        *font);
PangoCoverage *       pango_font_get_coverage      (PangoFont        *font,
						    PangoLanguage    *language);
PangoEngineShape *    pango_font_find_shaper       (PangoFont        *font,
						    PangoLanguage    *language,
						    guint32           ch);
PangoFontMetrics *    pango_font_get_metrics       (PangoFont        *font,
						    PangoLanguage    *language);
void                  pango_font_get_glyph_extents (PangoFont        *font,
						    PangoGlyph        glyph,
						    PangoRectangle   *ink_rect,
						    PangoRectangle   *logical_rect);
PangoFontMap         *pango_font_get_font_map      (PangoFont        *font);

#ifdef PANGO_ENABLE_BACKEND

#define PANGO_FONT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), PANGO_TYPE_FONT, PangoFontClass))
#define PANGO_IS_FONT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), PANGO_TYPE_FONT))
#define PANGO_FONT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), PANGO_TYPE_FONT, PangoFontClass))

typedef struct _PangoFontClass       PangoFontClass;

struct _PangoFont
{
  GObject parent_instance;
};

struct _PangoFontClass
{
  GObjectClass parent_class;

  /*< public >*/

  PangoFontDescription *(*describe)           (PangoFont      *font);
  PangoCoverage *       (*get_coverage)       (PangoFont      *font,
					       PangoLanguage  *lang);
  PangoEngineShape *    (*find_shaper)        (PangoFont      *font,
					       PangoLanguage  *lang,
					       guint32         ch);
  void                  (*get_glyph_extents)  (PangoFont      *font,
					       PangoGlyph      glyph,
					       PangoRectangle *ink_rect,
					       PangoRectangle *logical_rect);
  PangoFontMetrics *    (*get_metrics)        (PangoFont      *font,
					       PangoLanguage  *language);
  PangoFontMap *        (*get_font_map)       (PangoFont      *font);
  PangoFontDescription *(*describe_absolute)  (PangoFont      *font);
  /*< private >*/

  /* Padding for future expansion */
  void (*_pango_reserved1) (void);
  void (*_pango_reserved2) (void);
};

/* used for very rare and miserable situtations that we cannot even
 * draw a hexbox
 */
#define PANGO_UNKNOWN_GLYPH_WIDTH  10
#define PANGO_UNKNOWN_GLYPH_HEIGHT 14

#endif /* PANGO_ENABLE_BACKEND */

#define PANGO_GLYPH_EMPTY           ((PangoGlyph)0x0FFFFFFF)
#define PANGO_GLYPH_INVALID_INPUT   ((PangoGlyph)0xFFFFFFFF)
#define PANGO_GLYPH_UNKNOWN_FLAG    ((PangoGlyph)0x10000000)
#define PANGO_GET_UNKNOWN_GLYPH(wc) ((PangoGlyph)(wc)|PANGO_GLYPH_UNKNOWN_FLAG)


G_END_DECLS

#endif /* __PANGO_FONT_H__ */
