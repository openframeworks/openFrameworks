


#if defined(ATK_DISABLE_SINGLE_INCLUDES) && !defined (__ATK_H_INSIDE__) && !defined (ATK_COMPILATION)
#error "Only <atk/atk.h> can be included directly."
#endif

#ifndef __ATK_ENUM_TYPES_H__
#define __ATK_ENUM_TYPES_H__

#include <glib-object.h>

G_BEGIN_DECLS
/* enumerations from "atkhyperlink.h" */
GType atk_hyperlink_state_flags_get_type (void);
#define ATK_TYPE_HYPERLINK_STATE_FLAGS (atk_hyperlink_state_flags_get_type())
/* enumerations from "atkobject.h" */
GType atk_role_get_type (void);
#define ATK_TYPE_ROLE (atk_role_get_type())
GType atk_layer_get_type (void);
#define ATK_TYPE_LAYER (atk_layer_get_type())
/* enumerations from "atkrelationtype.h" */
GType atk_relation_type_get_type (void);
#define ATK_TYPE_RELATION_TYPE (atk_relation_type_get_type())
/* enumerations from "atkstate.h" */
GType atk_state_type_get_type (void);
#define ATK_TYPE_STATE_TYPE (atk_state_type_get_type())
/* enumerations from "atktext.h" */
GType atk_text_attribute_get_type (void);
#define ATK_TYPE_TEXT_ATTRIBUTE (atk_text_attribute_get_type())
GType atk_text_boundary_get_type (void);
#define ATK_TYPE_TEXT_BOUNDARY (atk_text_boundary_get_type())
GType atk_text_clip_type_get_type (void);
#define ATK_TYPE_TEXT_CLIP_TYPE (atk_text_clip_type_get_type())
/* enumerations from "atkutil.h" */
GType atk_key_event_type_get_type (void);
#define ATK_TYPE_KEY_EVENT_TYPE (atk_key_event_type_get_type())
GType atk_coord_type_get_type (void);
#define ATK_TYPE_COORD_TYPE (atk_coord_type_get_type())
G_END_DECLS

#endif /* __ATK_ENUM_TYPES_H__ */



