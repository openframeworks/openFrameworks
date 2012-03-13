


#ifndef __GST_AUDIO_ENUM_TYPES_H__
#define __GST_AUDIO_ENUM_TYPES_H__

#include <glib-object.h>

G_BEGIN_DECLS

/* enumerations from "multichannel.h" */
GType gst_audio_channel_position_get_type (void);
#define GST_TYPE_AUDIO_CHANNEL_POSITION (gst_audio_channel_position_get_type())

/* enumerations from "gstringbuffer.h" */
GType gst_ring_buffer_state_get_type (void);
#define GST_TYPE_RING_BUFFER_STATE (gst_ring_buffer_state_get_type())
GType gst_ring_buffer_seg_state_get_type (void);
#define GST_TYPE_RING_BUFFER_SEG_STATE (gst_ring_buffer_seg_state_get_type())
GType gst_buffer_format_type_get_type (void);
#define GST_TYPE_BUFFER_FORMAT_TYPE (gst_buffer_format_type_get_type())
GType gst_buffer_format_get_type (void);
#define GST_TYPE_BUFFER_FORMAT (gst_buffer_format_get_type())
G_END_DECLS

#endif /* __GST_AUDIO_ENUM_TYPES_H__ */



