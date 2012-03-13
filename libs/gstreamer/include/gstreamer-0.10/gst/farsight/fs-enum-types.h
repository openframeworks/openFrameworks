


#ifndef __FS_FS_ENUM_TYPES_H__
#define __FS_FS_ENUM_TYPES_H__

#include <glib-object.h>

G_BEGIN_DECLS

/* enumerations from "fs-candidate.h" */
GType fs_candidate_type_get_type (void);
#define FS_TYPE_CANDIDATE_TYPE (fs_candidate_type_get_type())
GType fs_network_protocol_get_type (void);
#define FS_TYPE_NETWORK_PROTOCOL (fs_network_protocol_get_type())
GType fs_component_type_get_type (void);
#define FS_TYPE_COMPONENT_TYPE (fs_component_type_get_type())

/* enumerations from "fs-codec.h" */
GType fs_media_type_get_type (void);
#define FS_TYPE_MEDIA_TYPE (fs_media_type_get_type())

/* enumerations from "fs-session.h" */
GType fs_dtmf_event_get_type (void);
#define FS_TYPE_DTMF_EVENT (fs_dtmf_event_get_type())
GType fs_dtmf_method_get_type (void);
#define FS_TYPE_DTMF_METHOD (fs_dtmf_method_get_type())

/* enumerations from "fs-stream.h" */
GType fs_stream_direction_get_type (void);
#define FS_TYPE_STREAM_DIRECTION (fs_stream_direction_get_type())
GType fs_stream_state_get_type (void);
#define FS_TYPE_STREAM_STATE (fs_stream_state_get_type())

/* enumerations from "fs-conference-iface.h" */
GType fs_error_get_type (void);
#define FS_TYPE_ERROR (fs_error_get_type())
G_END_DECLS

#endif /* __FS_FS_ENUM_TYPES_H__ */



