


#ifndef __PANGO_ENUM_TYPES_H__
#define __PANGO_ENUM_TYPES_H__

#include <glib-object.h>

G_BEGIN_DECLS
/* enumerations from "pango-attributes.h" */
GType pango_attr_type_get_type (void);
#define PANGO_TYPE_ATTR_TYPE (pango_attr_type_get_type())
GType pango_underline_get_type (void);
#define PANGO_TYPE_UNDERLINE (pango_underline_get_type())
/* enumerations from "pango-bidi-type.h" */
GType pango_bidi_type_get_type (void);
#define PANGO_TYPE_BIDI_TYPE (pango_bidi_type_get_type())
GType pango_direction_get_type (void);
#define PANGO_TYPE_DIRECTION (pango_direction_get_type())
/* enumerations from "pango-coverage.h" */
GType pango_coverage_level_get_type (void);
#define PANGO_TYPE_COVERAGE_LEVEL (pango_coverage_level_get_type())
/* enumerations from "pango-font.h" */
GType pango_style_get_type (void);
#define PANGO_TYPE_STYLE (pango_style_get_type())
GType pango_variant_get_type (void);
#define PANGO_TYPE_VARIANT (pango_variant_get_type())
GType pango_weight_get_type (void);
#define PANGO_TYPE_WEIGHT (pango_weight_get_type())
GType pango_stretch_get_type (void);
#define PANGO_TYPE_STRETCH (pango_stretch_get_type())
GType pango_font_mask_get_type (void);
#define PANGO_TYPE_FONT_MASK (pango_font_mask_get_type())
/* enumerations from "pango-gravity.h" */
GType pango_gravity_get_type (void);
#define PANGO_TYPE_GRAVITY (pango_gravity_get_type())
GType pango_gravity_hint_get_type (void);
#define PANGO_TYPE_GRAVITY_HINT (pango_gravity_hint_get_type())
/* enumerations from "pango-layout.h" */
GType pango_alignment_get_type (void);
#define PANGO_TYPE_ALIGNMENT (pango_alignment_get_type())
GType pango_wrap_mode_get_type (void);
#define PANGO_TYPE_WRAP_MODE (pango_wrap_mode_get_type())
GType pango_ellipsize_mode_get_type (void);
#define PANGO_TYPE_ELLIPSIZE_MODE (pango_ellipsize_mode_get_type())
/* enumerations from "pango-renderer.h" */
GType pango_render_part_get_type (void);
#define PANGO_TYPE_RENDER_PART (pango_render_part_get_type())
/* enumerations from "pango-script.h" */
GType pango_script_get_type (void);
#define PANGO_TYPE_SCRIPT (pango_script_get_type())
/* enumerations from "pango-tabs.h" */
GType pango_tab_align_get_type (void);
#define PANGO_TYPE_TAB_ALIGN (pango_tab_align_get_type())
G_END_DECLS

#endif /* __PANGO_ENUM_TYPES_H__ */



