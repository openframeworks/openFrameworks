


#ifndef __GST_ENUM_TYPES_H__
#define __GST_ENUM_TYPES_H__

#include <glib-object.h>

G_BEGIN_DECLS

/* enumerations from "gstobject.h" */
GType gst_object_flags_get_type (void);
#define GST_TYPE_OBJECT_FLAGS (gst_object_flags_get_type())

/* enumerations from "gstbin.h" */
GType gst_bin_flags_get_type (void);
#define GST_TYPE_BIN_FLAGS (gst_bin_flags_get_type())

/* enumerations from "gstbuffer.h" */
GType gst_buffer_flag_get_type (void);
#define GST_TYPE_BUFFER_FLAG (gst_buffer_flag_get_type())
GType gst_buffer_copy_flags_get_type (void);
#define GST_TYPE_BUFFER_COPY_FLAGS (gst_buffer_copy_flags_get_type())

/* enumerations from "gstbufferlist.h" */
GType gst_buffer_list_item_get_type (void);
#define GST_TYPE_BUFFER_LIST_ITEM (gst_buffer_list_item_get_type())

/* enumerations from "gstbus.h" */
GType gst_bus_flags_get_type (void);
#define GST_TYPE_BUS_FLAGS (gst_bus_flags_get_type())
GType gst_bus_sync_reply_get_type (void);
#define GST_TYPE_BUS_SYNC_REPLY (gst_bus_sync_reply_get_type())

/* enumerations from "gstcaps.h" */
GType gst_caps_flags_get_type (void);
#define GST_TYPE_CAPS_FLAGS (gst_caps_flags_get_type())

/* enumerations from "gstclock.h" */
GType gst_clock_return_get_type (void);
#define GST_TYPE_CLOCK_RETURN (gst_clock_return_get_type())
GType gst_clock_entry_type_get_type (void);
#define GST_TYPE_CLOCK_ENTRY_TYPE (gst_clock_entry_type_get_type())
GType gst_clock_flags_get_type (void);
#define GST_TYPE_CLOCK_FLAGS (gst_clock_flags_get_type())

/* enumerations from "gstdebugutils.h" */
GType gst_debug_graph_details_get_type (void);
#define GST_TYPE_DEBUG_GRAPH_DETAILS (gst_debug_graph_details_get_type())

/* enumerations from "gstelement.h" */
GType gst_state_get_type (void);
#define GST_TYPE_STATE (gst_state_get_type())
GType gst_state_change_return_get_type (void);
#define GST_TYPE_STATE_CHANGE_RETURN (gst_state_change_return_get_type())
GType gst_state_change_get_type (void);
#define GST_TYPE_STATE_CHANGE (gst_state_change_get_type())
GType gst_element_flags_get_type (void);
#define GST_TYPE_ELEMENT_FLAGS (gst_element_flags_get_type())

/* enumerations from "gsterror.h" */
GType gst_core_error_get_type (void);
#define GST_TYPE_CORE_ERROR (gst_core_error_get_type())
GType gst_library_error_get_type (void);
#define GST_TYPE_LIBRARY_ERROR (gst_library_error_get_type())
GType gst_resource_error_get_type (void);
#define GST_TYPE_RESOURCE_ERROR (gst_resource_error_get_type())
GType gst_stream_error_get_type (void);
#define GST_TYPE_STREAM_ERROR (gst_stream_error_get_type())

/* enumerations from "gstevent.h" */
GType gst_event_type_flags_get_type (void);
#define GST_TYPE_EVENT_TYPE_FLAGS (gst_event_type_flags_get_type())
GType gst_event_type_get_type (void);
#define GST_TYPE_EVENT_TYPE (gst_event_type_get_type())
GType gst_seek_type_get_type (void);
#define GST_TYPE_SEEK_TYPE (gst_seek_type_get_type())
GType gst_seek_flags_get_type (void);
#define GST_TYPE_SEEK_FLAGS (gst_seek_flags_get_type())

/* enumerations from "gstformat.h" */
GType gst_format_get_type (void);
#define GST_TYPE_FORMAT (gst_format_get_type())

/* enumerations from "gstindex.h" */
GType gst_index_certainty_get_type (void);
#define GST_TYPE_INDEX_CERTAINTY (gst_index_certainty_get_type())
GType gst_index_entry_type_get_type (void);
#define GST_TYPE_INDEX_ENTRY_TYPE (gst_index_entry_type_get_type())
GType gst_index_lookup_method_get_type (void);
#define GST_TYPE_INDEX_LOOKUP_METHOD (gst_index_lookup_method_get_type())
GType gst_assoc_flags_get_type (void);
#define GST_TYPE_ASSOC_FLAGS (gst_assoc_flags_get_type())
GType gst_index_resolver_method_get_type (void);
#define GST_TYPE_INDEX_RESOLVER_METHOD (gst_index_resolver_method_get_type())
GType gst_index_flags_get_type (void);
#define GST_TYPE_INDEX_FLAGS (gst_index_flags_get_type())

/* enumerations from "gstinfo.h" */
GType gst_debug_level_get_type (void);
#define GST_TYPE_DEBUG_LEVEL (gst_debug_level_get_type())
GType gst_debug_color_flags_get_type (void);
#define GST_TYPE_DEBUG_COLOR_FLAGS (gst_debug_color_flags_get_type())

/* enumerations from "gstiterator.h" */
GType gst_iterator_result_get_type (void);
#define GST_TYPE_ITERATOR_RESULT (gst_iterator_result_get_type())
GType gst_iterator_item_get_type (void);
#define GST_TYPE_ITERATOR_ITEM (gst_iterator_item_get_type())

/* enumerations from "gstmessage.h" */
GType gst_message_type_get_type (void);
#define GST_TYPE_MESSAGE_TYPE (gst_message_type_get_type())
GType gst_structure_change_type_get_type (void);
#define GST_TYPE_STRUCTURE_CHANGE_TYPE (gst_structure_change_type_get_type())
GType gst_stream_status_type_get_type (void);
#define GST_TYPE_STREAM_STATUS_TYPE (gst_stream_status_type_get_type())

/* enumerations from "gstminiobject.h" */
GType gst_mini_object_flags_get_type (void);
#define GST_TYPE_MINI_OBJECT_FLAGS (gst_mini_object_flags_get_type())

/* enumerations from "gstpad.h" */
GType gst_pad_link_return_get_type (void);
#define GST_TYPE_PAD_LINK_RETURN (gst_pad_link_return_get_type())
GType gst_flow_return_get_type (void);
#define GST_TYPE_FLOW_RETURN (gst_flow_return_get_type())
GType gst_pad_link_check_get_type (void);
#define GST_TYPE_PAD_LINK_CHECK (gst_pad_link_check_get_type())
GType gst_activate_mode_get_type (void);
#define GST_TYPE_ACTIVATE_MODE (gst_activate_mode_get_type())
GType gst_pad_direction_get_type (void);
#define GST_TYPE_PAD_DIRECTION (gst_pad_direction_get_type())
GType gst_pad_flags_get_type (void);
#define GST_TYPE_PAD_FLAGS (gst_pad_flags_get_type())

/* enumerations from "gstpadtemplate.h" */
GType gst_pad_presence_get_type (void);
#define GST_TYPE_PAD_PRESENCE (gst_pad_presence_get_type())
GType gst_pad_template_flags_get_type (void);
#define GST_TYPE_PAD_TEMPLATE_FLAGS (gst_pad_template_flags_get_type())

/* enumerations from "gstpipeline.h" */
GType gst_pipeline_flags_get_type (void);
#define GST_TYPE_PIPELINE_FLAGS (gst_pipeline_flags_get_type())

/* enumerations from "gstplugin.h" */
GType gst_plugin_error_get_type (void);
#define GST_TYPE_PLUGIN_ERROR (gst_plugin_error_get_type())
GType gst_plugin_flags_get_type (void);
#define GST_TYPE_PLUGIN_FLAGS (gst_plugin_flags_get_type())
GType gst_plugin_dependency_flags_get_type (void);
#define GST_TYPE_PLUGIN_DEPENDENCY_FLAGS (gst_plugin_dependency_flags_get_type())

/* enumerations from "gstpluginfeature.h" */
GType gst_rank_get_type (void);
#define GST_TYPE_RANK (gst_rank_get_type())

/* enumerations from "gstquery.h" */
GType gst_query_type_get_type (void);
#define GST_TYPE_QUERY_TYPE (gst_query_type_get_type())
GType gst_buffering_mode_get_type (void);
#define GST_TYPE_BUFFERING_MODE (gst_buffering_mode_get_type())

/* enumerations from "gstsystemclock.h" */
GType gst_clock_type_get_type (void);
#define GST_TYPE_CLOCK_TYPE (gst_clock_type_get_type())

/* enumerations from "gsttaglist.h" */
GType gst_tag_merge_mode_get_type (void);
#define GST_TYPE_TAG_MERGE_MODE (gst_tag_merge_mode_get_type())
GType gst_tag_flag_get_type (void);
#define GST_TYPE_TAG_FLAG (gst_tag_flag_get_type())

/* enumerations from "gsttask.h" */
GType gst_task_state_get_type (void);
#define GST_TYPE_TASK_STATE (gst_task_state_get_type())

/* enumerations from "gsttrace.h" */
GType gst_alloc_trace_flags_get_type (void);
#define GST_TYPE_ALLOC_TRACE_FLAGS (gst_alloc_trace_flags_get_type())

/* enumerations from "gsttypefind.h" */
GType gst_type_find_probability_get_type (void);
#define GST_TYPE_TYPE_FIND_PROBABILITY (gst_type_find_probability_get_type())

/* enumerations from "gsturi.h" */
GType gst_uri_type_get_type (void);
#define GST_TYPE_URI_TYPE (gst_uri_type_get_type())

/* enumerations from "gstutils.h" */
GType gst_search_mode_get_type (void);
#define GST_TYPE_SEARCH_MODE (gst_search_mode_get_type())

/* enumerations from "gstparse.h" */
GType gst_parse_error_get_type (void);
#define GST_TYPE_PARSE_ERROR (gst_parse_error_get_type())
GType gst_parse_flags_get_type (void);
#define GST_TYPE_PARSE_FLAGS (gst_parse_flags_get_type())
G_END_DECLS

#endif /* __GST_ENUM_TYPES_H__ */



