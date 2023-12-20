cmake_minimum_required(VERSION 3.16)
project(libdom)

set(LIBDOM_CORE_SOURCES
    source/src/core/attr.c
    source/src/core/attr.h
    source/src/core/cdatasection.c
    source/src/core/cdatasection.h
    source/src/core/characterdata.c
    source/src/core/characterdata.h
    source/src/core/comment.c
    source/src/core/comment.h
    source/src/core/document.c
    source/src/core/document.h
    source/src/core/document_type.c
    source/src/core/document_type.h
    source/src/core/doc_fragment.c
    source/src/core/doc_fragment.h
    source/src/core/element.c
    source/src/core/element.h
    source/src/core/entity_ref.c
    source/src/core/entity_ref.h
    source/src/core/implementation.c
    source/src/core/namednodemap.c
    source/src/core/namednodemap.h
    source/src/core/node.c
    source/src/core/node.h
    source/src/core/nodelist.c
    source/src/core/nodelist.h
    source/src/core/pi.c
    source/src/core/pi.h
    source/src/core/string.c
    source/src/core/string.h
    source/src/core/text.c
    source/src/core/text.h
    source/src/core/typeinfo.c
)

set(LIBDOM_EVENTS_SOURCES
    source/src/events/custom_event.c
    source/src/events/custom_event.h
    source/src/events/dispatch.c
    source/src/events/dispatch.h
    source/src/events/document_event.c
    source/src/events/document_event.h
    source/src/events/event.c
    source/src/events/event.h
    source/src/events/event_listener.c
    source/src/events/event_listener.h
    source/src/events/event_target.c
    source/src/events/event_target.h
    source/src/events/keyboard_event.c
    source/src/events/keyboard_event.h
    source/src/events/mouse_event.c
    source/src/events/mouse_event.h
    source/src/events/mouse_multi_wheel_event.c
    source/src/events/mouse_multi_wheel_event.h
    source/src/events/mouse_wheel_event.c
    source/src/events/mouse_wheel_event.h
    source/src/events/mutation_event.c
    source/src/events/mutation_event.h
    source/src/events/mutation_name_event.c
    source/src/events/mutation_name_event.h
    source/src/events/text_event.c
    source/src/events/text_event.h
    source/src/events/ui_event.c
    source/src/events/ui_event.h
)

set(LIBDOM_HTML_SOURCES
    source/src/html/html_anchor_element.c
    source/src/html/html_anchor_element.h
    source/src/html/html_applet_element.c
    source/src/html/html_applet_element.h
    source/src/html/html_area_element.c
    source/src/html/html_area_element.h
    source/src/html/html_basefont_element.c
    source/src/html/html_basefont_element.h
    source/src/html/html_base_element.c
    source/src/html/html_base_element.h
    source/src/html/html_body_element.c
    source/src/html/html_body_element.h
    source/src/html/html_br_element.c
    source/src/html/html_br_element.h
    source/src/html/html_button_element.c
    source/src/html/html_button_element.h
    source/src/html/html_canvas_element.c
    source/src/html/html_canvas_element.h
    source/src/html/html_collection.c
    source/src/html/html_collection.h
    source/src/html/html_directory_element.c
    source/src/html/html_directory_element.h
    source/src/html/html_div_element.c
    source/src/html/html_div_element.h
    source/src/html/html_dlist_element.c
    source/src/html/html_dlist_element.h
    source/src/html/html_document.c
    source/src/html/html_document.h
    source/src/html/html_document_strings.h
    source/src/html/html_element.c
    source/src/html/html_element.h
    source/src/html/html_fieldset_element.c
    source/src/html/html_fieldset_element.h
    source/src/html/html_font_element.c
    source/src/html/html_font_element.h
    source/src/html/html_form_element.c
    source/src/html/html_form_element.h
    source/src/html/html_frameset_element.c
    source/src/html/html_frameset_element.h
    source/src/html/html_frame_element.c
    source/src/html/html_frame_element.h
    source/src/html/html_heading_element.c
    source/src/html/html_heading_element.h
    source/src/html/html_head_element.c
    source/src/html/html_head_element.h
    source/src/html/html_hr_element.c
    source/src/html/html_hr_element.h
    source/src/html/html_html_element.c
    source/src/html/html_html_element.h
    source/src/html/html_iframe_element.c
    source/src/html/html_iframe_element.h
    source/src/html/html_image_element.c
    source/src/html/html_image_element.h
    source/src/html/html_input_element.c
    source/src/html/html_input_element.h
    source/src/html/html_isindex_element.c
    source/src/html/html_isindex_element.h
    source/src/html/html_label_element.c
    source/src/html/html_label_element.h
    source/src/html/html_legend_element.c
    source/src/html/html_legend_element.h
    source/src/html/html_link_element.c
    source/src/html/html_link_element.h
    source/src/html/html_li_element.c
    source/src/html/html_li_element.h
    source/src/html/html_map_element.c
    source/src/html/html_map_element.h
    source/src/html/html_menu_element.c
    source/src/html/html_menu_element.h
    source/src/html/html_meta_element.c
    source/src/html/html_meta_element.h
    source/src/html/html_mod_element.c
    source/src/html/html_mod_element.h
    source/src/html/html_object_element.c
    source/src/html/html_object_element.h
    source/src/html/html_olist_element.c
    source/src/html/html_olist_element.h
    source/src/html/html_options_collection.c
    source/src/html/html_options_collection.h
    source/src/html/html_option_element.c
    source/src/html/html_option_element.h
    source/src/html/html_opt_group_element.c
    source/src/html/html_opt_group_element.h
    source/src/html/html_paragraph_element.c
    source/src/html/html_paragraph_element.h
    source/src/html/html_param_element.c
    source/src/html/html_param_element.h
    source/src/html/html_pre_element.c
    source/src/html/html_pre_element.h
    source/src/html/html_quote_element.c
    source/src/html/html_quote_element.h
    source/src/html/html_script_element.c
    source/src/html/html_script_element.h
    source/src/html/html_select_element.c
    source/src/html/html_select_element.h
    source/src/html/html_style_element.c
    source/src/html/html_style_element.h
    source/src/html/html_tablecaption_element.c
    source/src/html/html_tablecaption_element.h
    source/src/html/html_tablecell_element.c
    source/src/html/html_tablecell_element.h
    source/src/html/html_tablecol_element.c
    source/src/html/html_tablecol_element.h
    source/src/html/html_tablerow_element.c
    source/src/html/html_tablerow_element.h
    source/src/html/html_tablesection_element.c
    source/src/html/html_tablesection_element.h
    source/src/html/html_table_element.c
    source/src/html/html_table_element.h
    source/src/html/html_text_area_element.c
    source/src/html/html_text_area_element.h
    source/src/html/html_title_element.c
    source/src/html/html_title_element.h
    source/src/html/html_ulist_element.c
    source/src/html/html_ulist_element.h
)

set(LIBDOM_UTILS_SOURCES
    source/src/utils/character_valid.c
    source/src/utils/character_valid.h
    source/src/utils/hashtable.c
    source/src/utils/hashtable.h
    source/src/utils/list.h
    source/src/utils/namespace.c
    source/src/utils/namespace.h
    source/src/utils/utils.h
    source/src/utils/validate.c
    source/src/utils/validate.h
)

add_library(libdom_core STATIC ${LIBDOM_CORE_SOURCES})
add_library(libdom_events STATIC ${LIBDOM_EVENTS_SOURCES})
add_library(libdom_html STATIC ${LIBDOM_HTML_SOURCES})
add_library(libdom_utils STATIC ${LIBDOM_UTILS_SOURCES})

add_library(libdom::core ALIAS libdom_core)
add_library(libdom::events ALIAS libdom_events)
add_library(libdom::html ALIAS libdom_html)
add_library(libdom::utils ALIAS libdom_utils)

target_include_directories(libdom_core PUBLIC source/include)
target_include_directories(libdom_events PUBLIC source/include)
target_include_directories(libdom_html PUBLIC source/include)
target_include_directories(libdom_utils PUBLIC source/include)

target_compile_features(libdom_core PUBLIC c_std_99)
target_compile_features(libdom_events PUBLIC c_std_99)
target_compile_features(libdom_html PUBLIC c_std_99)
target_compile_features(libdom_utils PUBLIC c_std_99)
